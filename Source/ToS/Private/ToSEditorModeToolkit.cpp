// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToSEditorModeToolkit.h"
#include "ToSEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "ToSEditorModeToolkit"

FToSEditorModeToolkit::FToSEditorModeToolkit()
{
}

void FToSEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FToSEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FToSEditorModeToolkit::GetToolkitFName() const
{
	return FName("ToSEditorMode");
}

FText FToSEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "ToSEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
