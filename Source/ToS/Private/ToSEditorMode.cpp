// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSEditorMode.h"
#include "ToSEditorModeToolkit.h"
#include "ToSEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "ToSEditorMode"

const FEditorModeID UToSEditorMode::EM_ToSEditorModeId = TEXT("EM_ToSEditorMode");

UToSEditorMode::UToSEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UToSEditorMode::EM_ToSEditorModeId,
		LOCTEXT("ModeName", "ToS"),
		FSlateIcon(),
		true);
}

UToSEditorMode::~UToSEditorMode()
{
}

void UToSEditorMode::ActorSelectionChangeNotify()
{
}

void UToSEditorMode::Enter()
{
	UEdMode::Enter();
	// Mode functionality now handled by toolbar menu instead of interactive tools
}

void UToSEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FToSEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UToSEditorMode::GetModeCommands() const
{
	return FToSEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
