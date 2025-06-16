// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSEditorModeCommands.h"
#include "ToSEditorMode.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "ToSEditorModeCommands"

FToSEditorModeCommands::FToSEditorModeCommands()
	: TCommands<FToSEditorModeCommands>("ToSEditorMode",
		NSLOCTEXT("ToSEditorMode", "ToSEditorModeCommands", "ToS Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FToSEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FToSEditorModeCommands::GetCommands()
{
	return FToSEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
