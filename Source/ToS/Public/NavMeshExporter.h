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

private:
	// Check if NavMeshBoundsVolume exists in current level
	static bool HasNavMeshBoundsVolume(UWorld* World);
	
	// Get NavMesh data
	static TSharedPtr<FJsonObject> GetNavMeshData(UWorld* World);
	
	// Extract NavMesh triangulation data
	static TSharedPtr<FJsonObject> ExtractNavMeshTriangulation(class ARecastNavMesh* RecastNavMesh);
	
	// Extract Heightmap for ground/height validation
	static TSharedPtr<FJsonObject> ExtractHeightmap(UWorld* World, const FBox& Bounds);
	
	// Save JSON file
	static bool SaveJsonToFile(const TSharedPtr<FJsonObject>& JsonObject, const FString& FilePath);
	
	// Get current level name
	static FString GetCurrentLevelName(UWorld* World);
	
	// Generate export file path using settings
	static FString GenerateExportFilePath(const FString& LevelName, const FString& FileType);
}; 