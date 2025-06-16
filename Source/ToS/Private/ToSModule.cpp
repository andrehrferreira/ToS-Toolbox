// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSModule.h"
#include "ToSEditorModeCommands.h"
#include "NavMeshExporter.h"
#include "ToSStyle.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "Framework/Commands/UIAction.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "ToSModule"

void FToSModule::StartupModule()
{
	FToSStyle::Initialize();
	FToSEditorModeCommands::Register();
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FToSModule::RegisterMenus));
}

void FToSModule::ShutdownModule()
{
	FToSStyle::Shutdown();
	FToSEditorModeCommands::Unregister();
	UToolMenus::UnRegisterStartupCallback(this);
}

void FToSModule::RegisterMenus()
{
	// Make sure we have the tool menus available
	if (!UToolMenus::IsToolMenuUIEnabled())
	{
		return;
	}

	// Get the level editor assets toolbar (following official documentation)
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.LevelEditorToolBar.AssetsToolBar"));
	if (!ToolbarMenu)
	{
		return;
	}

	// Add ToS section to assets toolbar
	FToolMenuSection& ToSSection = ToolbarMenu->FindOrAddSection(TEXT("ToSTools"));
	
	// Add ToS dropdown button (like the documentation shows)
	ToSSection.AddEntry(FToolMenuEntry::InitComboButton(
		TEXT("ToSToolsCombo"),
		FUIAction(),
		FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
		{
			// NavMesh Tools section
			FToolMenuSection& NavMeshSection = InMenu->AddSection(TEXT("NavMeshTools"), LOCTEXT("NavMeshToolsLabel", "NavMesh Tools"));
			
			NavMeshSection.AddMenuEntry(
				TEXT("ExportNavMesh"),
				LOCTEXT("ExportNavMeshLabel", "Export NavMesh"),
				LOCTEXT("ExportNavMeshTooltip", "Export current level NavMesh to JSON"),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"),
				FUIAction(FExecuteAction::CreateStatic(&FNavMeshExporter::ExportNavMesh))
			);
			
			NavMeshSection.AddMenuEntry(
				TEXT("ExportHeightmap"),
				LOCTEXT("ExportHeightmapLabel", "Export Heightmap"),
				LOCTEXT("ExportHeightmapTooltip", "Export level heightmap for anti-cheat validation"),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Layers"),
				FUIAction(FExecuteAction::CreateStatic(&FNavMeshExporter::ExportHeightmap))
			);
			
			// Future tools section placeholder
			FToolMenuSection& FutureSection = InMenu->AddSection(TEXT("FutureTools"), LOCTEXT("FutureToolsLabel", "More Tools"));
			
			FutureSection.AddMenuEntry(
				TEXT("PlaceholderTool"),
				LOCTEXT("PlaceholderLabel", "More Tools Coming Soon"),
				LOCTEXT("PlaceholderTooltip", "Additional tools will be added here"),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings"),
				FUIAction()
			);
		}),
		LOCTEXT("ToSToolsLabel", "ToS"),
		LOCTEXT("ToSToolsTooltip", "Tales Of Shadowland Toolbox"),
		FSlateIcon(FToSStyle::GetStyleSetName(), "ToS.ToolbarIcon")
	));

	// Refresh the menu
	UToolMenus::Get()->RefreshAllWidgets();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToSModule, ToS)