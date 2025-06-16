#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

class FToSStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();

private:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleInstance;
}; 