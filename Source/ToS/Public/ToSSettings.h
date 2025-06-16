#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ToSSettings.generated.h"

/**
 * Settings for Tales Of Shadowland Toolbox Plugin
 */
UCLASS(config = Editor, defaultconfig, meta = (DisplayName = "Tales Of Shadowland Toolbox"))
class TOS_API UToSSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UToSSettings();

	// Directory where exported files will be saved
	UPROPERTY(config, EditAnywhere, Category = "Export Settings", meta = (DisplayName = "Export Directory"))
	FString ExportDirectory;

	// Whether to include timestamp in filename
	UPROPERTY(config, EditAnywhere, Category = "Export Settings", meta = (DisplayName = "Include Timestamp"))
	bool bIncludeTimestamp;

	// Heightmap sampling resolution (in Unreal units)
	UPROPERTY(config, EditAnywhere, Category = "Heightmap Settings", meta = (DisplayName = "Sampling Resolution", ClampMin = "10", ClampMax = "200"))
	int32 HeightmapResolution;

	// Maximum number of triangles to export
	UPROPERTY(config, EditAnywhere, Category = "NavMesh Settings", meta = (DisplayName = "Max Triangles", ClampMin = "100", ClampMax = "2000"))
	int32 MaxTriangles;

	// Whether to export heightmap with NavMesh
	UPROPERTY(config, EditAnywhere, Category = "Export Settings", meta = (DisplayName = "Include Heightmap with NavMesh"))
	bool bIncludeHeightmapWithNavMesh;

public:
	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;

#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Static getter for easy access
	static const UToSSettings* Get();
	static UToSSettings* GetMutable();
}; 