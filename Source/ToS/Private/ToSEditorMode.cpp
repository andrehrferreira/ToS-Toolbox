// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSEditorMode.h"
#include "ToSEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "ToSEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/ToSSimpleTool.h"
#include "Tools/ToSInteractiveTool.h"

// step 2: register a ToolBuilder in FToSEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "ToSEditorMode"

const FEditorModeID UToSEditorMode::EM_ToSEditorModeId = TEXT("EM_ToSEditorMode");

FString UToSEditorMode::SimpleToolName = TEXT("ToS_ActorInfoTool");
FString UToSEditorMode::InteractiveToolName = TEXT("ToS_MeasureDistanceTool");


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

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FToSEditorModeCommands& SampleToolCommands = FToSEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UToSSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UToSInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
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
