// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ToSStyle.h"

/**
 * This class contains info about the full set of commands used in this editor mode.
 */
class FToSEditorModeCommands : public TCommands<FToSEditorModeCommands>
{
public:
	FToSEditorModeCommands();

	virtual void RegisterCommands() override;
	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

	TSharedPtr<FUICommandInfo> ExportNavMesh;
	TSharedPtr<FUICommandInfo> ExportHeightmap;
	TSharedPtr<FUICommandInfo> ResetCameraToOrigin;

protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};
