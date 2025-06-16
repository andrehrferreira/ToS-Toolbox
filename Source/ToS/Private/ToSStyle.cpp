#include "ToSStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

TSharedPtr<FSlateStyleSet> FToSStyle::StyleInstance = nullptr;

void FToSStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FToSStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FToSStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ToSStyle"));
	return StyleSetName;
}

const ISlateStyle& FToSStyle::Get()
{
	return *StyleInstance;
}

TSharedRef<FSlateStyleSet> FToSStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("ToS");
	check(Plugin.IsValid());

	FString ResourcesDir = Plugin->GetBaseDir() / TEXT("Resources");
	Style->SetContentRoot(ResourcesDir);

	Style->Set(
		"ToS.PluginIcon",
		new FSlateImageBrush(
			Style->RootToContentDir(TEXT("Icon128"), TEXT(".png")),
			FVector2D(128.0f, 128.0f)
		)
	);

	Style->Set(
		"ToS.ToolbarIcon",
		new FSlateImageBrush(
			Style->RootToContentDir(TEXT("Icon128"), TEXT(".png")),
			FVector2D(16.0f, 16.0f)
		)
	);

	return Style;
}

// Macros are already defined in SlateStyleMacros.h, no need to undefine them 