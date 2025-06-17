// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSEditorModeCommands.h"
#include "ToSEditorMode.h"
#include "ToSStyle.h"

#define LOCTEXT_NAMESPACE "FToSEditorModeCommands"

FToSEditorModeCommands::FToSEditorModeCommands()
	: TCommands<FToSEditorModeCommands>("ToSEditorMode",
		NSLOCTEXT("ToSEditorMode", "ToSEditorModeCommands", "ToS Editor Mode"),
		NAME_None,
		FToSStyle::GetStyleSetName())
{
}

void FToSEditorModeCommands::RegisterCommands()
{
	UI_COMMAND(ExportNavMesh, "Export NavMesh", "Export current level NavMesh to JSON", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ExportHeightmap, "Export Heightmap", "Export level heightmap for anti-cheat validation", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ResetCameraToOrigin, "Reset Camera to Origin", "Position camera for optimal NavMesh overview", EUserInterfaceActionType::Button, FInputChord());
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FToSEditorModeCommands::GetCommands()
{
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
	TArray<TSharedPtr<FUICommandInfo>> ToolCommands;
	
	const FToSEditorModeCommands& CommandsInstance = FToSEditorModeCommands::Get();
	ToolCommands.Add(CommandsInstance.ExportNavMesh);
	ToolCommands.Add(CommandsInstance.ExportHeightmap);
	ToolCommands.Add(CommandsInstance.ResetCameraToOrigin);
	Commands.Add(NAME_Default, ToolCommands);
	return Commands;
}

#undef LOCTEXT_NAMESPACE
