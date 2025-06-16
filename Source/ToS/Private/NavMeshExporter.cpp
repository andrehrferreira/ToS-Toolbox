#include "NavMeshExporter.h"
#include "Engine/World.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "AI/NavigationSystemBase.h"
#include "NavMesh/RecastNavMesh.h"
#include "Json.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/MessageDialog.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "ToSSettings.h"
#include "Misc/DateTime.h"
#include "Async/Async.h"
#include "Misc/ScopedSlowTask.h"
#include "Async/ParallelFor.h"
#include <atomic>

FNavMeshExporter::FNavMeshExporter()
{
}

FNavMeshExporter::~FNavMeshExporter()
{
}

void FNavMeshExporter::ExportNavMesh()
{
	UWorld* CurrentWorld = nullptr;
	
	// Get current world on game thread
	if (GEditor && GEditor->GetEditorWorldContext().World())
	{
		CurrentWorld = GEditor->GetEditorWorldContext().World();
	}

	if (!CurrentWorld)
	{
		// Show error notification on game thread
		FNotificationInfo Info(FText::FromString("No valid world found for NavMesh export"));
		Info.bUseThrobber = false;
		Info.bUseSuccessFailIcons = true;
		Info.bUseLargeFont = true;
		Info.bFireAndForget = true;
		Info.FadeOutDuration = 4.0f;
		Info.ExpireDuration = 4.0f;
		FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Fail);
		return;
	}

	// Create progress notification on game thread and start async processing
	AsyncTask(ENamedThreads::GameThread, [CurrentWorld]()
	{
		// Create progress notification
		FNotificationInfo Info(FText::FromString("Exporting NavMesh..."));
		Info.bUseThrobber = true;
		Info.bUseSuccessFailIcons = false;
		Info.bUseLargeFont = true;
		Info.bFireAndForget = false;
		Info.FadeOutDuration = 0.0f;
		Info.ExpireDuration = 0.0f;
		TSharedPtr<SNotificationItem> ProgressNotification = FSlateNotificationManager::Get().AddNotification(Info);
		
		if (ProgressNotification.IsValid())
		{
			ProgressNotification->SetCompletionState(SNotificationItem::CS_Pending);
		}

		// Start async export with progress tracking
		Async(EAsyncExecution::Thread, [CurrentWorld, ProgressNotification]()
	{
		// Create export data in background thread
		TSharedPtr<FJsonObject> JsonObject = CreateNavMeshExportData(CurrentWorld);
		
		if (!JsonObject.IsValid())
		{
			// Show error on game thread
			AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
			{
				if (ProgressNotification.IsValid())
				{
					ProgressNotification->SetText(FText::FromString("Failed to extract NavMesh data"));
					ProgressNotification->SetCompletionState(SNotificationItem::CS_Fail);
					ProgressNotification->ExpireAndFadeout();
				}
			});
			return;
		}

		// Update progress: Data extracted, now saving
		AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				ProgressNotification->SetText(FText::FromString("Saving NavMesh file..."));
			}
		});

		// Generate file path and save
		FString LevelName = GetCurrentLevelName(CurrentWorld);
		FString FilePath = GenerateExportFilePath(LevelName, TEXT("NavMesh"));
		
		bool bSaveSuccess = SaveJsonToFile(JsonObject, FilePath);
		
		// Show final result on game thread
		AsyncTask(ENamedThreads::GameThread, [bSaveSuccess, FilePath, ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				FString Message = bSaveSuccess ? 
					FString::Printf(TEXT("NavMesh exported successfully to: %s"), *FilePath) :
					TEXT("Failed to save NavMesh export file");
				
				ProgressNotification->SetText(FText::FromString(Message));
				auto CompletionState = bSaveSuccess ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail;
				ProgressNotification->SetCompletionState(CompletionState);
				ProgressNotification->ExpireAndFadeout();
			}
		});
		});
	});
}

void FNavMeshExporter::ExportHeightmap()
{
	UWorld* CurrentWorld = nullptr;
	
	// Get current world on game thread
	if (GEditor && GEditor->GetEditorWorldContext().World())
	{
		CurrentWorld = GEditor->GetEditorWorldContext().World();
	}

	if (!CurrentWorld)
	{
		// Show error notification on game thread
		FNotificationInfo Info(FText::FromString("No valid world found for Heightmap export"));
		Info.bUseThrobber = false;
		Info.bUseSuccessFailIcons = true;
		Info.bUseLargeFont = true;
		Info.bFireAndForget = true;
		Info.FadeOutDuration = 4.0f;
		Info.ExpireDuration = 4.0f;
		FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Fail);
		return;
	}

	// Create progress notification on game thread and start async processing
	AsyncTask(ENamedThreads::GameThread, [CurrentWorld]()
	{
		// Create progress notification
		FNotificationInfo Info(FText::FromString("Exporting Heightmap..."));
		Info.bUseThrobber = true;
		Info.bUseSuccessFailIcons = false;
		Info.bUseLargeFont = true;
		Info.bFireAndForget = false;
		Info.FadeOutDuration = 0.0f;
		Info.ExpireDuration = 0.0f;
		TSharedPtr<SNotificationItem> ProgressNotification = FSlateNotificationManager::Get().AddNotification(Info);
		
		if (ProgressNotification.IsValid())
		{
			ProgressNotification->SetCompletionState(SNotificationItem::CS_Pending);
		}

				// Get world bounds on game thread first (TActorIterator requires game thread)
		FBox WorldBounds = GetWorldBounds(CurrentWorld);
		if (!WorldBounds.IsValid)
		{
			// Show error on game thread
			if (ProgressNotification.IsValid())
			{
				ProgressNotification->SetText(FText::FromString("Invalid world bounds for heightmap export"));
				ProgressNotification->SetCompletionState(SNotificationItem::CS_Fail);
				ProgressNotification->ExpireAndFadeout();
			}
			return;
		}

		// Start async export with progress tracking
		Async(EAsyncExecution::Thread, [CurrentWorld, ProgressNotification, WorldBounds]()
		{
			// Update progress: Calculating bounds
			AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
			{
				if (ProgressNotification.IsValid())
				{
					ProgressNotification->SetText(FText::FromString("Calculated world bounds..."));
				}
			});

		// Update progress: Extracting heightmap
		AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				ProgressNotification->SetText(FText::FromString("Sampling heightmap data..."));
			}
		});

		// Create heightmap data in background thread
		TSharedPtr<FJsonObject> HeightmapData = ExtractHeightmap(CurrentWorld, WorldBounds, ProgressNotification);
		
		if (!HeightmapData.IsValid())
		{
			// Show error on game thread
			AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
			{
				if (ProgressNotification.IsValid())
				{
					ProgressNotification->SetText(FText::FromString("Failed to extract heightmap data"));
					ProgressNotification->SetCompletionState(SNotificationItem::CS_Fail);
					ProgressNotification->ExpireAndFadeout();
				}
			});
			return;
		}

		// Update progress: Building JSON structure
		AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				ProgressNotification->SetText(FText::FromString("Building JSON structure..."));
			}
		});

		// Create complete JSON structure
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		FString LevelName = GetCurrentLevelName(CurrentWorld);
		JsonObject->SetStringField(TEXT("LevelName"), LevelName);
		JsonObject->SetObjectField(TEXT("Heightmap"), HeightmapData);

		// Add level bounds
		TSharedPtr<FJsonObject> LevelBounds = MakeShareable(new FJsonObject);
		LevelBounds->SetNumberField(TEXT("MinX"), FMath::RoundToInt(WorldBounds.Min.X));
		LevelBounds->SetNumberField(TEXT("MinY"), FMath::RoundToInt(WorldBounds.Min.Y));
		LevelBounds->SetNumberField(TEXT("MinZ"), FMath::RoundToInt(WorldBounds.Min.Z));
		LevelBounds->SetNumberField(TEXT("MaxX"), FMath::RoundToInt(WorldBounds.Max.X));
		LevelBounds->SetNumberField(TEXT("MaxY"), FMath::RoundToInt(WorldBounds.Max.Y));
		LevelBounds->SetNumberField(TEXT("MaxZ"), FMath::RoundToInt(WorldBounds.Max.Z));
		JsonObject->SetObjectField(TEXT("LevelBounds"), LevelBounds);

		// Update progress: Saving file
		AsyncTask(ENamedThreads::GameThread, [ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				ProgressNotification->SetText(FText::FromString("Saving heightmap file..."));
			}
		});

		// Generate file path and save
		FString FilePath = GenerateExportFilePath(LevelName, TEXT("Heightmap"));
		bool bSaveSuccess = SaveJsonToFile(JsonObject, FilePath);
		
		// Show final result on game thread
		AsyncTask(ENamedThreads::GameThread, [bSaveSuccess, FilePath, ProgressNotification]()
		{
			if (ProgressNotification.IsValid())
			{
				FString Message = bSaveSuccess ? 
					FString::Printf(TEXT("Heightmap exported successfully to: %s"), *FilePath) :
					TEXT("Failed to save Heightmap export file");
				
				ProgressNotification->SetText(FText::FromString(Message));
				auto CompletionState = bSaveSuccess ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail;
				ProgressNotification->SetCompletionState(CompletionState);
				ProgressNotification->ExpireAndFadeout();
			}
		});
		});
	});
}

FBox FNavMeshExporter::GetWorldBounds(UWorld* World)
{
	if (!World || !IsValid(World))
	{
		return FBox(ForceInit);
	}

	// Ensure we're on the game thread for actor iteration
	check(IsInGameThread());

	FBox WorldBounds(ForceInit);

	// Use a safer approach - get bounds from navigation system if available
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (NavSys)
	{
		ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
		if (NavData && IsValid(NavData))
		{
			FBox NavBounds = NavData->GetBounds();
			if (NavBounds.IsValid)
			{
				// Expand nav bounds slightly for heightmap sampling
				WorldBounds = NavBounds.ExpandBy(500.0f);
				return WorldBounds;
			}
		}
	}

	// Fallback: iterate through actors safely
	try
	{
		// Include static mesh actors
		for (TActorIterator<AStaticMeshActor> ActorIterator(World); ActorIterator; ++ActorIterator)
		{
			AStaticMeshActor* StaticMeshActor = *ActorIterator;
			if (IsValid(StaticMeshActor))
			{
				WorldBounds += StaticMeshActor->GetActorLocation();
			}
		}

		// Include landscape actors using safer iteration
		for (TActorIterator<AActor> ActorIterator(World); ActorIterator; ++ActorIterator)
		{
			AActor* Actor = *ActorIterator;
			if (IsValid(Actor) && Actor->GetClass()->GetName().Contains(TEXT("Landscape")))
			{
				WorldBounds += Actor->GetActorLocation();
			}
		}
	}
	catch (...)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exception during world bounds calculation, using default bounds"));
	}

	// Ensure bounds are valid and have minimum size
	if (!WorldBounds.IsValid)
	{
		WorldBounds = FBox(FVector(-5000, -5000, -1000), FVector(5000, 5000, 1000));
	}

	return WorldBounds;
}

TSharedPtr<FJsonObject> FNavMeshExporter::CreateNavMeshExportData(UWorld* World)
{
	if (!World || !IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid world for NavMesh export"));
		return nullptr;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FString LevelName = GetCurrentLevelName(World);
	JsonObject->SetStringField(TEXT("LevelName"), LevelName);

	// Get Navigation System
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		UE_LOG(LogTemp, Error, TEXT("No Navigation System found"));
		return nullptr;
	}

	TArray<TSharedPtr<FJsonValue>> NavMeshArray;

	// Get NavMesh data - ultra-compact format
	ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
	if (NavData)
	{
		ARecastNavMesh* RecastNavMesh = Cast<ARecastNavMesh>(NavData);
		if (RecastNavMesh)
		{
			// Direct triangulation data - no wrapper objects
			TSharedPtr<FJsonObject> TriangulationData = ExtractNavMeshTriangulation(RecastNavMesh);
			if (TriangulationData.IsValid())
			{
				NavMeshArray.Add(MakeShareable(new FJsonValueObject(TriangulationData)));
			}
			
			// Optionally add heightmap data based on settings
			const UToSSettings* ExportSettings = UToSSettings::Get();
			if (ExportSettings->bIncludeHeightmapWithNavMesh)
			{
				TSharedPtr<FJsonObject> HeightmapData = ExtractHeightmap(World, NavData->GetBounds(), nullptr);
				if (HeightmapData.IsValid())
				{
					JsonObject->SetObjectField(TEXT("H"), HeightmapData);
				}
			}
		}
	}

	JsonObject->SetArrayField(TEXT("N"), NavMeshArray);

	return JsonObject;
}

TSharedPtr<FJsonObject> FNavMeshExporter::ExtractNavMeshTriangulation(ARecastNavMesh* RecastNavMesh)
{
	if (!RecastNavMesh || !IsValid(RecastNavMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid RecastNavMesh for triangulation"));
		return nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting NavMesh triangulation extraction..."));

	TSharedPtr<FJsonObject> TriangulationObject = MakeShareable(new FJsonObject);
	
	TArray<TSharedPtr<FJsonValue>> VerticesArray;
	TArray<TSharedPtr<FJsonValue>> TrianglesArray;
	
	// Get settings for triangle limits and grid size
	const UToSSettings* TriangleSettings = UToSSettings::Get();
	
	FBox NavBounds = RecastNavMesh->GetBounds();
	if (!NavBounds.IsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid NavMesh bounds"));
		return nullptr;
	}

	// Use configurable grid size from settings (default to reasonable value if not set)
	int32 GridSize = FMath::Clamp(TriangleSettings->MaxTriangles / 10, 50, 200);
	const FVector BoundsSize = NavBounds.GetSize();
	const FVector StepSize = BoundsSize / GridSize;
	
	UE_LOG(LogTemp, Log, TEXT("Grid size: %d, Bounds: %s"), GridSize, *NavBounds.ToString());

	// Thread-safe containers for parallel processing
	TArray<FVector> ValidNavPoints;
	FCriticalSection ValidNavPointsLock;

	// Create progress tracker for UI thread updates
	std::atomic<bool> bCancelled{false};
	
	// Show progress on game thread
	AsyncTask(ENamedThreads::GameThread, [GridSize]()
	{
		FScopedSlowTask SlowTask(100.0f, FText::FromString(TEXT("Extracting NavMesh triangulation...")));
		SlowTask.MakeDialog();
	});

	// Phase 1: Sample NavMesh points in parallel
	{
		const int32 TotalSamples = (GridSize + 1) * (GridSize + 1);
		TArray<TPair<int32, int32>> GridIndices;
		GridIndices.Reserve(TotalSamples);
		
		// Pre-generate grid indices for parallel processing
		for (int32 X = 0; X <= GridSize; X++)
		{
			for (int32 Y = 0; Y <= GridSize; Y++)
			{
				GridIndices.Add(TPair<int32, int32>(X, Y));
			}
		}

		// Parallel sampling of NavMesh points
		ParallelFor(GridIndices.Num(), [&](int32 Index)
		{
			if (bCancelled.load())
			{
				return;
			}

			const TPair<int32, int32>& GridIndex = GridIndices[Index];
			int32 X = GridIndex.Key;
			int32 Y = GridIndex.Value;
			
			// Project from above the NavMesh bounds for better accuracy
			FVector TestLocation = NavBounds.Min + FVector(X * StepSize.X, Y * StepSize.Y, NavBounds.Max.Z + 100.0f);
			FNavLocation NavLoc;
			FVector ProjectExtent(25.0f, 25.0f, NavBounds.GetSize().Z + 200.0f);

			if (RecastNavMesh->ProjectPoint(TestLocation, NavLoc, ProjectExtent))
			{
				// Round coordinates to reduce duplicate vertices
				FVector RoundedLoc = FVector(
					FMath::RoundToFloat(NavLoc.Location.X * 10.0f) / 10.0f,
					FMath::RoundToFloat(NavLoc.Location.Y * 10.0f) / 10.0f,
					FMath::RoundToFloat(NavLoc.Location.Z * 10.0f) / 10.0f
				);
				
				// Thread-safe addition to valid points
				{
					FScopeLock Lock(&ValidNavPointsLock);
					ValidNavPoints.AddUnique(RoundedLoc);
				}
			}
		});
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d valid NavMesh points"), ValidNavPoints.Num());

	if (ValidNavPoints.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Insufficient NavMesh points for triangulation"));
		return nullptr;
	}

	// Phase 2: Convert vertices to JSON format (compact array format)
	for (int32 i = 0; i < ValidNavPoints.Num(); i++)
	{
		TArray<TSharedPtr<FJsonValue>> VertexArray;
		VertexArray.Add(MakeShareable(new FJsonValueNumber(FMath::RoundToInt(ValidNavPoints[i].X))));
		VertexArray.Add(MakeShareable(new FJsonValueNumber(FMath::RoundToInt(ValidNavPoints[i].Y))));
		VertexArray.Add(MakeShareable(new FJsonValueNumber(FMath::RoundToInt(ValidNavPoints[i].Z))));
		
		VerticesArray.Add(MakeShareable(new FJsonValueArray(VertexArray)));
	}

	// Phase 3: Optimized Grid-Based Triangulation (O(N²) instead of O(N³))
	{
		TArray<TArray<int32>> GridPointMap;
		GridPointMap.SetNum(GridSize + 1);
		for (int32 i = 0; i <= GridSize; i++)
		{
			GridPointMap[i].SetNum(GridSize + 1);
			for (int32 j = 0; j <= GridSize; j++)
			{
				GridPointMap[i][j] = -1; // Invalid index
			}
		}

		// Map valid points back to grid positions
		for (int32 PointIndex = 0; PointIndex < ValidNavPoints.Num(); PointIndex++)
		{
			const FVector& Point = ValidNavPoints[PointIndex];
			int32 GridX = FMath::RoundToInt((Point.X - NavBounds.Min.X) / StepSize.X);
			int32 GridY = FMath::RoundToInt((Point.Y - NavBounds.Min.Y) / StepSize.Y);
			
			if (GridX >= 0 && GridX <= GridSize && GridY >= 0 && GridY <= GridSize)
			{
				GridPointMap[GridX][GridY] = PointIndex;
			}
		}

		// Generate triangles using grid topology (quad to tri conversion)
		int32 TotalTriangles = 0;
		const int32 MaxTriangles = TriangleSettings->MaxTriangles;

		for (int32 X = 0; X < GridSize && TotalTriangles < MaxTriangles; X++)
		{
			for (int32 Y = 0; Y < GridSize && TotalTriangles < MaxTriangles; Y++)
			{
				// Get four corners of current grid cell
				int32 P0 = GridPointMap[X][Y];         // Bottom-left
				int32 P1 = GridPointMap[X+1][Y];       // Bottom-right
				int32 P2 = GridPointMap[X][Y+1];       // Top-left
				int32 P3 = GridPointMap[X+1][Y+1];     // Top-right

				// Only create triangles if all four points are valid
				if (P0 >= 0 && P1 >= 0 && P2 >= 0 && P3 >= 0)
				{
					// Create two triangles from quad: (P0,P1,P2) and (P1,P3,P2)
					
					// Triangle 1: P0 -> P1 -> P2
					{
						TArray<TSharedPtr<FJsonValue>> TriangleArray;
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P0)));
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P1)));
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P2)));
						
						TrianglesArray.Add(MakeShareable(new FJsonValueArray(TriangleArray)));
						TotalTriangles++;
					}

					// Triangle 2: P1 -> P3 -> P2
					if (TotalTriangles < MaxTriangles)
					{
						TArray<TSharedPtr<FJsonValue>> TriangleArray;
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P1)));
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P3)));
						TriangleArray.Add(MakeShareable(new FJsonValueNumber(P2)));
						
						TrianglesArray.Add(MakeShareable(new FJsonValueArray(TriangleArray)));
						TotalTriangles++;
					}
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Generated %d triangles from grid topology"), TotalTriangles);
	}

	// Ultra-compact NavMesh data - minimal structure
	TriangulationObject->SetArrayField(TEXT("V"), VerticesArray);
	TriangulationObject->SetArrayField(TEXT("T"), TrianglesArray);
	
	UE_LOG(LogTemp, Log, TEXT("NavMesh triangulation extraction completed"));
	return TriangulationObject;
}

TSharedPtr<FJsonObject> FNavMeshExporter::ExtractHeightmap(UWorld* World, const FBox& Bounds, TSharedPtr<SNotificationItem> ProgressNotification)
{
	if (!World || !IsValid(World) || !Bounds.IsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid parameters for heightmap extraction"));
		return nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("Starting heightmap extraction..."));

	TSharedPtr<FJsonObject> HeightmapObject = MakeShareable(new FJsonObject);
	
	// Thread-safe containers for heightmap data
	TMap<FString, TArray<int32>> HeightGrid; // Key: "X,Y", Value: Array of Z heights as int32
	FCriticalSection HeightGridLock;
	
	// Configure raycast parameters (thread-safe read-only data)
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.TraceTag = FName("HeightmapTrace");
	
	// Set collision channels to trace
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	
	// Calculate bounds for heightmap sampling
	FVector BoundsMin = Bounds.Min;
	FVector BoundsMax = Bounds.Max;
	
	// Use absolute positions without decimals (round to integers)
	int32 MinX = FMath::FloorToInt(BoundsMin.X);
	int32 MaxX = FMath::CeilToInt(BoundsMax.X);
	int32 MinY = FMath::FloorToInt(BoundsMin.Y);
	int32 MaxY = FMath::CeilToInt(BoundsMax.Y);
	
	// Add buffer zone around bounds for better coverage
	const int32 BufferSize = 1000;
	MinX -= BufferSize;
	MinY -= BufferSize;
	MaxX += BufferSize;
	MaxY += BufferSize;
	
	// Get resolution from settings
	const UToSSettings* Settings = UToSSettings::Get();
	int32 Resolution = Settings->HeightmapResolution;
	
	// Define raycast height range
	float RaycastStartZ = BoundsMax.Z + 2000.0f;
	float RaycastEndZ = BoundsMin.Z - 2000.0f;
	
	// Calculate total samples for progress tracking
	const int32 XSamples = (MaxX - MinX) / Resolution + 1;
	const int32 YSamples = (MaxY - MinY) / Resolution + 1;
	const int32 TotalSamples = XSamples * YSamples;
	
	UE_LOG(LogTemp, Log, TEXT("Heightmap sampling: %d x %d = %d total samples"), XSamples, YSamples, TotalSamples);

	// Thread-safe counters
	std::atomic<int32> ProcessedSamples(0);
	std::atomic<int32> ValidHits(0);
	std::atomic<bool> bCancelled{false};

	// Pre-generate sample coordinates for parallel processing
	TArray<TPair<int32, int32>> SampleCoords;
	SampleCoords.Reserve(TotalSamples);
	
	for (int32 X = MinX; X <= MaxX; X += Resolution)
	{
		for (int32 Y = MinY; Y <= MaxY; Y += Resolution)
		{
			SampleCoords.Add(TPair<int32, int32>(X, Y));
		}
	}

	// Parallel heightmap sampling
	ParallelFor(SampleCoords.Num(), [&](int32 Index)
	{
		if (bCancelled.load())
		{
			return;
		}

		const TPair<int32, int32>& Coord = SampleCoords[Index];
		int32 X = Coord.Key;
		int32 Y = Coord.Value;
		
		// Create raycast from above to below
		FVector StartLocation(X, Y, RaycastStartZ);
		FVector EndLocation(X, Y, RaycastEndZ);
		
		TArray<FHitResult> HitResults;
		
		// Multi-line trace to get ALL surfaces at this X,Y position
		bool bHit = World->LineTraceMultiByObjectType(
			HitResults,
			StartLocation,
			EndLocation,
			ObjectQueryParams,
			QueryParams
		);
		
		ProcessedSamples.fetch_add(1);
		
		if (bHit && HitResults.Num() > 0)
		{
			ValidHits.fetch_add(1);
			
			// Collect all hit heights for this X,Y position
			FString GridKey = FString::Printf(TEXT("%d,%d"), X, Y);
			TArray<int32> Heights;
			
			for (const FHitResult& Hit : HitResults)
			{
				if (Hit.bBlockingHit)
				{
					// Convert Z to int32 (rounded)
					int32 HitZ = FMath::RoundToInt(Hit.Location.Z);
					Heights.AddUnique(HitZ);
				}
			}
			
			// Sort heights and store in thread-safe manner
			Heights.Sort();
			{
				FScopeLock Lock(&HeightGridLock);
				HeightGrid.Add(GridKey, Heights);
			}
		}
		
		// Update progress periodically on game thread
		if (Index % 1000 == 0)
		{
			int32 CurrentProgress = ProcessedSamples.load();
			float ProgressPercent = (float)CurrentProgress / TotalSamples * 100.0f;
			
			AsyncTask(ENamedThreads::GameThread, [ProgressPercent, ProgressNotification]()
			{
				if (ProgressNotification.IsValid())
				{
					FString ProgressText = FString::Printf(TEXT("Sampling heightmap data... %.1f%%"), ProgressPercent);
					ProgressNotification->SetText(FText::FromString(ProgressText));
				}
				UE_LOG(LogTemp, Log, TEXT("Heightmap progress: %.1f%%"), ProgressPercent);
			});
		}
	});

	UE_LOG(LogTemp, Log, TEXT("Heightmap sampling completed: %d/%d valid hits"), ValidHits.load(), ProcessedSamples.load());

	// Convert height grid to JSON format
	TArray<TSharedPtr<FJsonValue>> HeightmapArray;
	for (auto& Entry : HeightGrid)
	{
		// Parse X,Y from key
		FString GridKey = Entry.Key;
		TArray<FString> Coordinates;
		GridKey.ParseIntoArray(Coordinates, TEXT(","));
		
		if (Coordinates.Num() == 2)
		{
			int32 GridX = FCString::Atoi(*Coordinates[0]);
			int32 GridY = FCString::Atoi(*Coordinates[1]);
			TArray<int32>& Heights = Entry.Value;
			
			// Create compact JSON entry - simplified format
			TSharedPtr<FJsonObject> HeightEntry = MakeShareable(new FJsonObject);
			HeightEntry->SetNumberField(TEXT("X"), GridX);
			HeightEntry->SetNumberField(TEXT("Y"), GridY);
			
			// Only Z heights array (removed MinZ, MaxZ, HeightCount duplication)
			TArray<TSharedPtr<FJsonValue>> ZHeights;
			for (int32 Height : Heights)
			{
				ZHeights.Add(MakeShareable(new FJsonValueNumber(Height)));
			}
			HeightEntry->SetArrayField(TEXT("Z"), ZHeights);
			
			HeightmapArray.Add(MakeShareable(new FJsonValueObject(HeightEntry)));
		}
	}

	// Set minimal heightmap data - only essential information
	HeightmapObject->SetArrayField(TEXT("Data"), HeightmapArray);
	HeightmapObject->SetNumberField(TEXT("SampleCount"), ProcessedSamples.load());
	HeightmapObject->SetNumberField(TEXT("ValidHits"), ValidHits.load());
	
	UE_LOG(LogTemp, Log, TEXT("Heightmap extraction completed"));
	return HeightmapObject;
}

bool FNavMeshExporter::SaveJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath)
{
	if (!JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid JSON object for saving"));
		return false;
	}

	FString Directory = FPaths::GetPath(FilePath);
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory))
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*Directory))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create directory: %s"), *Directory);
			return false;
		}
	}

	FString OutputString;
	// Use compact JSON writer to minimize file size (no indentation, minimal spaces)
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize JSON"));
		return false;
	}

	if (!FFileHelper::SaveStringToFile(OutputString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save file: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully saved JSON to: %s"), *FilePath);
	return true;
}

FString FNavMeshExporter::GetCurrentLevelName(UWorld* World)
{
	if (!World || !IsValid(World))
	{
		return TEXT("UnknownLevel");
	}

	FString LevelName = World->GetMapName();
	
	// Remove PIE prefixes
	LevelName = LevelName.Replace(TEXT("UEDPIE_0_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_1_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_2_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_3_"), TEXT(""));
	
	// Extract filename from full path
	if (LevelName.Contains(TEXT("/")))
	{
		FString Path, Filename, Extension;
		FPaths::Split(LevelName, Path, Filename, Extension);
		LevelName = Filename;
	}

	return LevelName.IsEmpty() ? TEXT("UnknownLevel") : LevelName;
}

FString FNavMeshExporter::GenerateExportFilePath(const FString& LevelName, const FString& FileType)
{
	const UToSSettings* Settings = UToSSettings::Get();
	FString ExportDir = Settings->ExportDirectory;
	
	// Ensure export directory exists
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*ExportDir))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ExportDir);
	}
	
	// Generate filename
	FString FileName;
	if (Settings->bIncludeTimestamp)
	{
		FString Timestamp = FDateTime::Now().ToString(TEXT("_%Y%m%d_%H%M%S"));
		FileName = FString::Printf(TEXT("%s_%s%s.json"), *LevelName, *FileType, *Timestamp);
	}
	else
	{
		FileName = FString::Printf(TEXT("%s_%s.json"), *LevelName, *FileType);
	}
	
	return FPaths::Combine(ExportDir, FileName);
}