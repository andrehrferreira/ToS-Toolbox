// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ToS/Public/ToSEditorMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToSEditorMode() {}

// Begin Cross Module References
TOS_API UClass* Z_Construct_UClass_UToSEditorMode();
TOS_API UClass* Z_Construct_UClass_UToSEditorMode_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UEdMode();
UPackage* Z_Construct_UPackage__Script_ToS();
// End Cross Module References

// Begin Class UToSEditorMode
void UToSEditorMode::StaticRegisterNativesUToSEditorMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSEditorMode);
UClass* Z_Construct_UClass_UToSEditorMode_NoRegister()
{
	return UToSEditorMode::StaticClass();
}
struct Z_Construct_UClass_UToSEditorMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * This class provides an example of how to extend a UEdMode to add some simple tools\n * using the InteractiveTools framework. The various UEdMode input event handlers (see UEdMode.h)\n * forward events to a UEdModeInteractiveToolsContext instance, which\n * has all the logic for interacting with the InputRouter, ToolManager, etc.\n * The functions provided here are the minimum to get started inserting some custom behavior.\n * Take a look at the UEdMode markup for more extensibility options.\n */" },
#endif
		{ "IncludePath", "ToSEditorMode.h" },
		{ "ModuleRelativePath", "Public/ToSEditorMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This class provides an example of how to extend a UEdMode to add some simple tools\nusing the InteractiveTools framework. The various UEdMode input event handlers (see UEdMode.h)\nforward events to a UEdModeInteractiveToolsContext instance, which\nhas all the logic for interacting with the InputRouter, ToolManager, etc.\nThe functions provided here are the minimum to get started inserting some custom behavior.\nTake a look at the UEdMode markup for more extensibility options." },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSEditorMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UToSEditorMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEdMode,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSEditorMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSEditorMode_Statics::ClassParams = {
	&UToSEditorMode::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSEditorMode_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSEditorMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSEditorMode()
{
	if (!Z_Registration_Info_UClass_UToSEditorMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSEditorMode.OuterSingleton, Z_Construct_UClass_UToSEditorMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSEditorMode.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSEditorMode>()
{
	return UToSEditorMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSEditorMode);
// End Class UToSEditorMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Public_ToSEditorMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UToSEditorMode, UToSEditorMode::StaticClass, TEXT("UToSEditorMode"), &Z_Registration_Info_UClass_UToSEditorMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSEditorMode), 3397157007U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Public_ToSEditorMode_h_2839329355(TEXT("/Script/ToS"),
	Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Public_ToSEditorMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Public_ToSEditorMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
