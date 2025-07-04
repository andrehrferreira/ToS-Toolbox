#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "Dom/JsonObject.h"

class TOS_API FNavMeshExporter
{
public:
	FNavMeshExporter();
	~FNavMeshExporter();

	// Main function to export NavMesh
	static void ExportNavMesh();
	
	// Export only Heightmap for anti-cheat validation
	static void ExportHeightmap();
	
	// Reset camera to optimal NavMesh overview position
	static void ResetCameraToNavMeshOrigin();

private:
	// Get world bounds for heightmap extraction
	static FBox GetWorldBounds(UWorld* World);
	
	// Create NavMesh export data (thread-safe)
	static TSharedPtr<FJsonObject> CreateNavMeshExportData(UWorld* World);
	
	// Extract NavMesh triangulation data
	static TSharedPtr<FJsonObject> ExtractNavMeshTriangulation(class ARecastNavMesh* RecastNavMesh);
	
	// Extract Heightmap for ground/height validation
	static TSharedPtr<FJsonObject> ExtractHeightmap(UWorld* World, const FBox& Bounds, TSharedPtr<class SNotificationItem> ProgressNotification = nullptr);
	
	// Save JSON file
	static bool SaveJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath);
	
	// Get current level name
	static FString GetCurrentLevelName(UWorld* World);
	
	// Generate export file path using settings
	static FString GenerateExportFilePath(const FString& LevelName, const FString& FileType);
	
	// Execute Python script to generate heightmap image
	static bool ExecutePythonHeightmapScript(const FString& JsonFilePath);
	
	// Execute Python script to generate NavMesh visualization
	static bool ExecutePythonNavMeshScript(const FString& JsonFilePath);
}; 