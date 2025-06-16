#include "ToSSettings.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "ToSSettings"

UToSSettings::UToSSettings()
{
	// Set default values
	ExportDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Exported"));
	bIncludeTimestamp = false;
	HeightmapResolution = 50;
	MaxTriangles = 500;
	bIncludeHeightmapWithNavMesh = true;
}

FName UToSSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

FText UToSSettings::GetSectionText() const
{
	return LOCTEXT("ToSSettingsSection", "Tales Of Shadowland Toolbox");
}

#if WITH_EDITOR
FText UToSSettings::GetSectionDescription() const
{
	return LOCTEXT("ToSSettingsDescription", "Configure export settings for Tales Of Shadowland Toolbox plugin");
}

void UToSSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	// Validate export directory
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UToSSettings, ExportDirectory))
	{
		if (ExportDirectory.IsEmpty())
		{
			ExportDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Exported"));
		}
	}
	
	// Save settings
	SaveConfig();
}
#endif

const UToSSettings* UToSSettings::Get()
{
	return GetDefault<UToSSettings>();
}

UToSSettings* UToSSettings::GetMutable()
{
	return GetMutableDefault<UToSSettings>();
}

#undef LOCTEXT_NAMESPACE 