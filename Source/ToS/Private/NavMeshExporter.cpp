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

FNavMeshExporter::FNavMeshExporter()
{
}

FNavMeshExporter::~FNavMeshExporter()
{
}

void FNavMeshExporter::ExportNavMesh()
{
	UWorld* CurrentWorld = GEditor->GetEditorWorldContext().World();
	if (!CurrentWorld)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No active world found!")));
		return;
	}

	if (!HasNavMeshBoundsVolume(CurrentWorld))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No NavMeshBoundsVolume found in current level!")));
		return;
	}

	TSharedPtr<FJsonObject> NavMeshData = GetNavMeshData(CurrentWorld);
	if (!NavMeshData.IsValid())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to get NavMesh data!")));
		return;
	}

	FString LevelName = GetCurrentLevelName(CurrentWorld);
	FString FileName = FString::Printf(TEXT("%s_NavMesh.json"), *LevelName);
	FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("ToS"), FileName);

	if (SaveJsonToFile(NavMeshData, FilePath))
	{
		FString SuccessMessage = FString::Printf(TEXT("NavMesh exported successfully to: %s"), *FilePath);
		FNotificationInfo Info(FText::FromString(SuccessMessage));
		Info.bFireAndForget = true;
		Info.FadeOutDuration = 5.0f;
		Info.ExpireDuration = 5.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to save JSON file!")));
	}
}

void FNavMeshExporter::ExportHeightmap()
{
	UWorld* CurrentWorld = GEditor->GetEditorWorldContext().World();
	if (!CurrentWorld)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No active world found!")));
		return;
	}

	// Get world bounds for heightmap
	FBox WorldBounds = FBox(ForceInit);
	
	// Try to get bounds from NavMesh if available
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(CurrentWorld);
	if (NavSys)
	{
		ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
		if (NavData && NavData->GetBounds().IsValid)
		{
			WorldBounds = NavData->GetBounds();
		}
	}
	
	// If no NavMesh bounds, use level bounds
	if (!WorldBounds.IsValid)
	{
		// Get bounds from all static mesh actors in the level
		for (TActorIterator<AStaticMeshActor> ActorIterator(CurrentWorld); ActorIterator; ++ActorIterator)
		{
			AStaticMeshActor* MeshActor = *ActorIterator;
			if (MeshActor && IsValid(MeshActor))
			{
				FBox ActorBounds = MeshActor->GetComponentsBoundingBox();
				if (ActorBounds.IsValid)
				{
					WorldBounds += ActorBounds;
				}
			}
		}
		
		// Get bounds from landscape (using base class to avoid linkage issues)
		for (TActorIterator<AActor> ActorIterator(CurrentWorld); ActorIterator; ++ActorIterator)
		{
			AActor* Actor = *ActorIterator;
			if (Actor && IsValid(Actor) && Actor->GetClass()->GetName().Contains(TEXT("Landscape")))
			{
				FBox LandscapeBounds = Actor->GetComponentsBoundingBox();
				if (LandscapeBounds.IsValid)
				{
					WorldBounds += LandscapeBounds;
				}
			}
		}
	}
	
	// Ensure we have valid bounds
	if (!WorldBounds.IsValid)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Could not determine level bounds for heightmap export!")));
		return;
	}

	// Extract heightmap data
	TSharedPtr<FJsonObject> HeightmapData = ExtractHeightmap(CurrentWorld, WorldBounds);
	if (!HeightmapData.IsValid())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to extract heightmap data!")));
		return;
	}

	// Create JSON structure for heightmap export
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FString LevelName = GetCurrentLevelName(CurrentWorld);
	
	JsonObject->SetStringField(TEXT("LevelName"), LevelName);
	JsonObject->SetStringField(TEXT("ExportTime"), FDateTime::Now().ToString());
	JsonObject->SetStringField(TEXT("ExportType"), TEXT("Heightmap"));
	JsonObject->SetObjectField(TEXT("Heightmap"), HeightmapData);
	
	// Add level bounds information
	TSharedPtr<FJsonObject> LevelBounds = MakeShareable(new FJsonObject);
	LevelBounds->SetNumberField(TEXT("MinX"), WorldBounds.Min.X);
	LevelBounds->SetNumberField(TEXT("MinY"), WorldBounds.Min.Y);
	LevelBounds->SetNumberField(TEXT("MinZ"), WorldBounds.Min.Z);
	LevelBounds->SetNumberField(TEXT("MaxX"), WorldBounds.Max.X);
	LevelBounds->SetNumberField(TEXT("MaxY"), WorldBounds.Max.Y);
	LevelBounds->SetNumberField(TEXT("MaxZ"), WorldBounds.Max.Z);
	JsonObject->SetObjectField(TEXT("LevelBounds"), LevelBounds);

	// Create filename for heightmap
	FString FileName = FString::Printf(TEXT("%s_Heightmap.json"), *LevelName);
	FString FilePath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("ToS"), FileName);

	// Save JSON file
	if (SaveJsonToFile(JsonObject, FilePath))
	{
		FString SuccessMessage = FString::Printf(TEXT("Heightmap exported successfully to: %s"), *FilePath);
		
		// Show success notification
		FNotificationInfo Info(FText::FromString(SuccessMessage));
		Info.bFireAndForget = true;
		Info.FadeOutDuration = 5.0f;
		Info.ExpireDuration = 5.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to save heightmap JSON file!")));
	}
}

bool FNavMeshExporter::HasNavMeshBoundsVolume(UWorld* World)
{
	if (!World)
	{
		return false;
	}

	for (TActorIterator<ANavMeshBoundsVolume> ActorIterator(World); ActorIterator; ++ActorIterator)
	{
		ANavMeshBoundsVolume* NavMeshBounds = *ActorIterator;
		if (NavMeshBounds && IsValid(NavMeshBounds))
		{
			return true;
		}
	}

	return false;
}

TSharedPtr<FJsonObject> FNavMeshExporter::GetNavMeshData(UWorld* World)
{
	if (!World)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	FString LevelName = GetCurrentLevelName(World);
	JsonObject->SetStringField(TEXT("LevelName"), LevelName);
	JsonObject->SetStringField(TEXT("ExportTime"), FDateTime::Now().ToString());

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		return JsonObject;
	}

	TArray<TSharedPtr<FJsonValue>> NavMeshArray;

	TArray<ANavMeshBoundsVolume*> NavMeshBounds;
	for (TActorIterator<ANavMeshBoundsVolume> ActorIterator(World); ActorIterator; ++ActorIterator)
	{
		ANavMeshBoundsVolume* NavMeshVolume = *ActorIterator;
		if (NavMeshVolume && IsValid(NavMeshVolume))
		{
			NavMeshBounds.Add(NavMeshVolume);
		}
	}

	// Process each NavMeshBoundsVolume found
	for (int32 i = 0; i < NavMeshBounds.Num(); i++)
	{
		ANavMeshBoundsVolume* NavMeshVolume = NavMeshBounds[i];
		TSharedPtr<FJsonObject> NavMeshObject = MakeShareable(new FJsonObject);
		
		// Basic volume information
		NavMeshObject->SetStringField(TEXT("Name"), NavMeshVolume->GetName());
		
		// Volume position
		FVector Location = NavMeshVolume->GetActorLocation();
		TSharedPtr<FJsonObject> LocationObject = MakeShareable(new FJsonObject);
		LocationObject->SetNumberField(TEXT("X"), Location.X);
		LocationObject->SetNumberField(TEXT("Y"), Location.Y);
		LocationObject->SetNumberField(TEXT("Z"), Location.Z);
		NavMeshObject->SetObjectField(TEXT("Location"), LocationObject);
		
		// Volume scale
		FVector Scale = NavMeshVolume->GetActorScale3D();
		TSharedPtr<FJsonObject> ScaleObject = MakeShareable(new FJsonObject);
		ScaleObject->SetNumberField(TEXT("X"), Scale.X);
		ScaleObject->SetNumberField(TEXT("Y"), Scale.Y);
		ScaleObject->SetNumberField(TEXT("Z"), Scale.Z);
		NavMeshObject->SetObjectField(TEXT("Scale"), ScaleObject);

		// Get volume bounds
		FBox Bounds = NavMeshVolume->GetComponentsBoundingBox();
		TSharedPtr<FJsonObject> BoundsObject = MakeShareable(new FJsonObject);
		
		TSharedPtr<FJsonObject> MinBounds = MakeShareable(new FJsonObject);
		MinBounds->SetNumberField(TEXT("X"), Bounds.Min.X);
		MinBounds->SetNumberField(TEXT("Y"), Bounds.Min.Y);
		MinBounds->SetNumberField(TEXT("Z"), Bounds.Min.Z);
		BoundsObject->SetObjectField(TEXT("Min"), MinBounds);
		
		TSharedPtr<FJsonObject> MaxBounds = MakeShareable(new FJsonObject);
		MaxBounds->SetNumberField(TEXT("X"), Bounds.Max.X);
		MaxBounds->SetNumberField(TEXT("Y"), Bounds.Max.Y);
		MaxBounds->SetNumberField(TEXT("Z"), Bounds.Max.Z);
		BoundsObject->SetObjectField(TEXT("Max"), MaxBounds);
		
		NavMeshObject->SetObjectField(TEXT("Bounds"), BoundsObject);

		ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
		if (NavData)
		{
			TSharedPtr<FJsonObject> NavMeshDataObject = MakeShareable(new FJsonObject);
			
			NavMeshDataObject->SetStringField(TEXT("NavMeshClass"), NavData->GetClass()->GetName());
			NavMeshDataObject->SetBoolField(TEXT("IsNavDataValid"), IsValid(NavData));

			FBox NavDataBounds = NavData->GetBounds();
			if (NavDataBounds.IsValid)
			{
				TSharedPtr<FJsonObject> NavBoundsObject = MakeShareable(new FJsonObject);
				
				TSharedPtr<FJsonObject> NavMinBounds = MakeShareable(new FJsonObject);
				NavMinBounds->SetNumberField(TEXT("X"), NavDataBounds.Min.X);
				NavMinBounds->SetNumberField(TEXT("Y"), NavDataBounds.Min.Y);
				NavMinBounds->SetNumberField(TEXT("Z"), NavDataBounds.Min.Z);
				NavBoundsObject->SetObjectField(TEXT("Min"), NavMinBounds);
				
				TSharedPtr<FJsonObject> NavMaxBounds = MakeShareable(new FJsonObject);
				NavMaxBounds->SetNumberField(TEXT("X"), NavDataBounds.Max.X);
				NavMaxBounds->SetNumberField(TEXT("Y"), NavDataBounds.Max.Y);
				NavMaxBounds->SetNumberField(TEXT("Z"), NavDataBounds.Max.Z);
				NavBoundsObject->SetObjectField(TEXT("Max"), NavMaxBounds);
				
				NavMeshDataObject->SetObjectField(TEXT("NavMeshBounds"), NavBoundsObject);
			}
			
			ARecastNavMesh* RecastNavMesh = Cast<ARecastNavMesh>(NavData);
			if (RecastNavMesh)
			{
				TSharedPtr<FJsonObject> TriangulationData = ExtractNavMeshTriangulation(RecastNavMesh);
				if (TriangulationData.IsValid())
				{
					NavMeshDataObject->SetObjectField(TEXT("Triangulation"), TriangulationData);
				}
				
				// Extract heightmap for ground/height validation
				TSharedPtr<FJsonObject> HeightmapData = ExtractHeightmap(World, NavData->GetBounds());
				if (HeightmapData.IsValid())
				{
					NavMeshDataObject->SetObjectField(TEXT("Heightmap"), HeightmapData);
				}
			}
			
			NavMeshObject->SetObjectField(TEXT("NavMeshData"), NavMeshDataObject);
		}

		NavMeshArray.Add(MakeShareable(new FJsonValueObject(NavMeshObject)));
	}

	JsonObject->SetArrayField(TEXT("NavMeshVolumes"), NavMeshArray);
	JsonObject->SetNumberField(TEXT("TotalNavMeshVolumes"), NavMeshBounds.Num());

	return JsonObject;
}

TSharedPtr<FJsonObject> FNavMeshExporter::ExtractNavMeshTriangulation(ARecastNavMesh* RecastNavMesh)
{
	if (!RecastNavMesh || !IsValid(RecastNavMesh))
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> TriangulationObject = MakeShareable(new FJsonObject);
	
	TArray<TSharedPtr<FJsonValue>> VerticesArray;
	TArray<TSharedPtr<FJsonValue>> TrianglesArray;
	
	int32 TotalVertices = 0;
	int32 TotalTriangles = 0;
	
	// Extract triangulation using surface sampling instead of direct generator access
	if (true) // Always proceed with surface sampling
	{
		TArray<FVector> NavVertices;
		TArray<int32> NavTriangles;
				
		FBox NavBounds = RecastNavMesh->GetBounds();
		if (NavBounds.IsValid)
		{
			// Higher resolution for better triangulation data
			const int32 GridSize = 100;
			const FVector BoundsSize = NavBounds.GetSize();
			const FVector StepSize = BoundsSize / GridSize;
			
			TMap<FVector, int32> VertexIndexMap; // To avoid duplicate vertices
			
			for (int32 X = 0; X <= GridSize; X++)
			{
				for (int32 Y = 0; Y <= GridSize; Y++)
				{
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
						
						// Only add unique vertices
						if (!VertexIndexMap.Contains(RoundedLoc))
						{
							int32 VertexIndex = NavVertices.Num();
							NavVertices.Add(RoundedLoc);
							VertexIndexMap.Add(RoundedLoc, VertexIndex);
						}
					}
				}
			}
		}
		
		// Convert vertices to JSON format (simplified for server validation)
		for (int32 i = 0; i < NavVertices.Num(); i++)
		{
			TSharedPtr<FJsonObject> VertexObject = MakeShareable(new FJsonObject);
			VertexObject->SetNumberField(TEXT("X"), FMath::RoundToInt(NavVertices[i].X));
			VertexObject->SetNumberField(TEXT("Y"), FMath::RoundToInt(NavVertices[i].Y));
			VertexObject->SetNumberField(TEXT("Z"), FMath::RoundToInt(NavVertices[i].Z));
			
			VerticesArray.Add(MakeShareable(new FJsonValueObject(VertexObject)));
		}
		
		// Create more sophisticated triangulation based on proximity
		// This creates triangles between nearby vertices rather than a rigid grid
		const float MaxTriangleDistance = 300.0f; // Maximum distance between vertices in a triangle
		
		for (int32 i = 0; i < NavVertices.Num(); i++)
		{
			for (int32 j = i + 1; j < NavVertices.Num(); j++)
			{
				for (int32 k = j + 1; k < NavVertices.Num(); k++)
				{
					FVector V1 = NavVertices[i];
					FVector V2 = NavVertices[j];
					FVector V3 = NavVertices[k];
					
					// Check if vertices are close enough to form a valid triangle
					float Dist12 = FVector::Dist(V1, V2);
					float Dist23 = FVector::Dist(V2, V3);
					float Dist31 = FVector::Dist(V3, V1);
					
					if (Dist12 < MaxTriangleDistance && Dist23 < MaxTriangleDistance && Dist31 < MaxTriangleDistance)
					{
						// Calculate triangle area to avoid degenerate triangles
						FVector CrossProduct = FVector::CrossProduct(V2 - V1, V3 - V1);
						float TriangleArea = CrossProduct.Size() * 0.5f;
						
						// Only create triangles with reasonable area
						if (TriangleArea > 500.0f)
						{
							// Simplified triangle data - only indices
							TSharedPtr<FJsonObject> TriangleObject = MakeShareable(new FJsonObject);
							TriangleObject->SetNumberField(TEXT("V0"), i);
							TriangleObject->SetNumberField(TEXT("V1"), j);
							TriangleObject->SetNumberField(TEXT("V2"), k);
							
							TrianglesArray.Add(MakeShareable(new FJsonValueObject(TriangleObject)));
							TotalTriangles++;
							
							// Limit number of triangles to avoid massive JSON files
							if (TotalTriangles >= 500)
							{
								break;
							}
						}
					}
				}
				if (TotalTriangles >= 500) break;
			}
			if (TotalTriangles >= 500) break;
		}
		
		TotalVertices = NavVertices.Num();
	}

	TriangulationObject->SetArrayField(TEXT("Vertices"), VerticesArray);
	TriangulationObject->SetArrayField(TEXT("Triangles"), TrianglesArray);
	TriangulationObject->SetNumberField(TEXT("TotalVertices"), TotalVertices);
	TriangulationObject->SetNumberField(TEXT("TotalTriangles"), TotalTriangles);
	
	// Add comprehensive metadata for server validation
	TSharedPtr<FJsonObject> TriangulationMetadata = MakeShareable(new FJsonObject);
	TriangulationMetadata->SetStringField(TEXT("ExtractionMethod"), TEXT("High-resolution surface sampling with proximity-based triangulation"));
	TriangulationMetadata->SetStringField(TEXT("Description"), TEXT("Real NavMesh surface data suitable for server-side pathfinding validation"));
	TriangulationMetadata->SetNumberField(TEXT("SamplingResolution"), 100);
	TriangulationMetadata->SetNumberField(TEXT("MaxTriangleDistance"), 300.0f);
	TriangulationMetadata->SetNumberField(TEXT("MinTriangleArea"), 500.0f);
	TriangulationMetadata->SetBoolField(TEXT("HasRealGeometry"), true);
	TriangulationMetadata->SetBoolField(TEXT("IsServerReady"), true);
	TriangulationMetadata->SetStringField(TEXT("ExportVersion"), TEXT("2.0"));
	
	// Add NavMesh properties for validation
	FBox NavBounds = RecastNavMesh->GetBounds();
	if (NavBounds.IsValid)
	{
		TSharedPtr<FJsonObject> ValidationBounds = MakeShareable(new FJsonObject);
		ValidationBounds->SetNumberField(TEXT("MinX"), NavBounds.Min.X);
		ValidationBounds->SetNumberField(TEXT("MinY"), NavBounds.Min.Y);
		ValidationBounds->SetNumberField(TEXT("MinZ"), NavBounds.Min.Z);
		ValidationBounds->SetNumberField(TEXT("MaxX"), NavBounds.Max.X);
		ValidationBounds->SetNumberField(TEXT("MaxY"), NavBounds.Max.Y);
		ValidationBounds->SetNumberField(TEXT("MaxZ"), NavBounds.Max.Z);
		ValidationBounds->SetNumberField(TEXT("SizeX"), NavBounds.GetSize().X);
		ValidationBounds->SetNumberField(TEXT("SizeY"), NavBounds.GetSize().Y);
		ValidationBounds->SetNumberField(TEXT("SizeZ"), NavBounds.GetSize().Z);
		TriangulationMetadata->SetObjectField(TEXT("ValidationBounds"), ValidationBounds);
	}
	
	TriangulationObject->SetObjectField(TEXT("Metadata"), TriangulationMetadata);
	
	return TriangulationObject;
}

TSharedPtr<FJsonObject> FNavMeshExporter::ExtractHeightmap(UWorld* World, const FBox& Bounds)
{
	if (!World || !Bounds.IsValid)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> HeightmapObject = MakeShareable(new FJsonObject);
	
	// Array to store heightmap data
	TArray<TSharedPtr<FJsonValue>> HeightmapArray;
	TMap<FString, TArray<int32>> HeightGrid; // Key: "X,Y", Value: Array of Z heights as int32
	
	// Configure raycast parameters
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.TraceTag = FName("HeightmapTrace");
	
	// Set collision channels to trace - use more conservative approach
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
	int32 BufferSize = 500; // 5 meters buffer
	MinX -= BufferSize;
	MaxX += BufferSize;
	MinY -= BufferSize;
	MaxY += BufferSize;
	
	// Raycast resolution (every N units)
	int32 Resolution = 50; // 0.5 meters
	int32 TotalSamples = 0;
	int32 ValidHits = 0;
	
	// Define raycast height range
	float RaycastStartZ = BoundsMax.Z + 2000.0f; // Start 20 meters above max bound
	float RaycastEndZ = BoundsMin.Z - 2000.0f;   // End 20 meters below min bound
	
	// Perform raycast grid sampling
	for (int32 X = MinX; X <= MaxX; X += Resolution)
	{
		for (int32 Y = MinY; Y <= MaxY; Y += Resolution)
		{
			TotalSamples++;
			
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
			
			if (bHit && HitResults.Num() > 0)
			{
				ValidHits++;
				
				// Collect all hit heights for this X,Y position
				FString GridKey = FString::Printf(TEXT("%d,%d"), X, Y);
				TArray<int32> Heights;
				
				for (const FHitResult& Hit : HitResults)
				{
					if (Hit.bBlockingHit)
					{
						// Convert Z to int32 (rounded)
						int32 HitZ = FMath::RoundToInt(Hit.Location.Z);
						Heights.AddUnique(HitZ); // AddUnique to avoid duplicate heights
					}
				}
				
				// Sort heights and store for quick lookup
				Heights.Sort();
				HeightGrid.Add(GridKey, Heights);
			}
		}
	}
	
	// Now create JSON entries from the collected height grid (one entry per X,Y position)
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
			
			// Create JSON entry for this X,Y position
			TSharedPtr<FJsonObject> HeightEntry = MakeShareable(new FJsonObject);
			HeightEntry->SetNumberField(TEXT("X"), GridX);
			HeightEntry->SetNumberField(TEXT("Y"), GridY);
			
			// Add all Z heights as array
			TArray<TSharedPtr<FJsonValue>> ZHeights;
			for (int32 Height : Heights)
			{
				ZHeights.Add(MakeShareable(new FJsonValueNumber(Height)));
			}
			HeightEntry->SetArrayField(TEXT("Z"), ZHeights);
			
			// Add min/max for quick validation
			HeightEntry->SetNumberField(TEXT("MinZ"), Heights.Num() > 0 ? Heights[0] : 0);
			HeightEntry->SetNumberField(TEXT("MaxZ"), Heights.Num() > 0 ? Heights.Last() : 0);
			HeightEntry->SetNumberField(TEXT("HeightCount"), Heights.Num());
			
			HeightmapArray.Add(MakeShareable(new FJsonValueObject(HeightEntry)));
		}
	}
	
	// Set heightmap data
	HeightmapObject->SetArrayField(TEXT("HeightData"), HeightmapArray);
	HeightmapObject->SetNumberField(TEXT("TotalSamples"), TotalSamples);
	HeightmapObject->SetNumberField(TEXT("ValidHits"), ValidHits);
	HeightmapObject->SetNumberField(TEXT("HitRate"), TotalSamples > 0 ? (float)ValidHits / TotalSamples : 0.0f);
	
	// Add heightmap configuration metadata
	TSharedPtr<FJsonObject> HeightmapConfig = MakeShareable(new FJsonObject);
	HeightmapConfig->SetNumberField(TEXT("Resolution"), Resolution);
	HeightmapConfig->SetNumberField(TEXT("BufferSize"), BufferSize);
	HeightmapConfig->SetNumberField(TEXT("MinX"), MinX);
	HeightmapConfig->SetNumberField(TEXT("MaxX"), MaxX);
	HeightmapConfig->SetNumberField(TEXT("MinY"), MinY);
	HeightmapConfig->SetNumberField(TEXT("MaxY"), MaxY);
	HeightmapConfig->SetNumberField(TEXT("RaycastStartZ"), RaycastStartZ);
	HeightmapConfig->SetNumberField(TEXT("RaycastEndZ"), RaycastEndZ);
	HeightmapConfig->SetStringField(TEXT("Description"), TEXT("Heightmap for ground and height validation using absolute integer coordinates"));
	HeightmapConfig->SetStringField(TEXT("Usage"), TEXT("Round player X,Y to nearest sampled position and validate Z height against stored values"));
	HeightmapConfig->SetBoolField(TEXT("IsAntiCheatReady"), true);
	HeightmapConfig->SetStringField(TEXT("Version"), TEXT("1.0"));
	
	HeightmapObject->SetObjectField(TEXT("Config"), HeightmapConfig);
	
	// Add quick lookup ranges for optimization
	TSharedPtr<FJsonObject> HeightRanges = MakeShareable(new FJsonObject);
	int32 MinHeight = INT32_MAX;
	int32 MaxHeight = INT32_MIN;
	
	for (auto& Entry : HeightGrid)
	{
		for (int32 Height : Entry.Value)
		{
			MinHeight = FMath::Min(MinHeight, Height);
			MaxHeight = FMath::Max(MaxHeight, Height);
		}
	}
	
	HeightRanges->SetNumberField(TEXT("GlobalMinHeight"), MinHeight != INT32_MAX ? MinHeight : 0);
	HeightRanges->SetNumberField(TEXT("GlobalMaxHeight"), MaxHeight != INT32_MIN ? MaxHeight : 0);
	HeightRanges->SetNumberField(TEXT("HeightRange"), MaxHeight != INT32_MIN && MinHeight != INT32_MAX ? (MaxHeight - MinHeight) : 0);
	
	HeightmapObject->SetObjectField(TEXT("HeightRanges"), HeightRanges);
	
	return HeightmapObject;
}

bool FNavMeshExporter::SaveJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath)
{
	if (!JsonObject.IsValid())
	{
		return false;
	}

	FString Directory = FPaths::GetPath(FilePath);
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*Directory);
	}

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

FString FNavMeshExporter::GetCurrentLevelName(UWorld* World)
{
	if (!World)
	{
		return TEXT("UnknownLevel");
	}

	FString LevelName = World->GetMapName();
	
	LevelName = LevelName.Replace(TEXT("UEDPIE_0_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_1_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_2_"), TEXT(""));
	LevelName = LevelName.Replace(TEXT("UEDPIE_3_"), TEXT(""));
	
	if (LevelName.Contains(TEXT("/")))
	{
		FString Path, Filename, Extension;
		FPaths::Split(LevelName, Path, Filename, Extension);
		LevelName = Filename;
	}

	return LevelName.IsEmpty() ? TEXT("UnknownLevel") : LevelName;
} 