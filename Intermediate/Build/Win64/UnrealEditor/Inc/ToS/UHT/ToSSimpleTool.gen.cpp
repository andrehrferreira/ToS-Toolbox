// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ToS/Private/Tools/ToSSimpleTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToSSimpleTool() {}

// Begin Cross Module References
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolPropertySet();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_USingleClickTool();
TOS_API UClass* Z_Construct_UClass_UToSSimpleTool();
TOS_API UClass* Z_Construct_UClass_UToSSimpleTool_NoRegister();
TOS_API UClass* Z_Construct_UClass_UToSSimpleToolBuilder();
TOS_API UClass* Z_Construct_UClass_UToSSimpleToolBuilder_NoRegister();
TOS_API UClass* Z_Construct_UClass_UToSSimpleToolProperties();
TOS_API UClass* Z_Construct_UClass_UToSSimpleToolProperties_NoRegister();
UPackage* Z_Construct_UPackage__Script_ToS();
// End Cross Module References

// Begin Class UToSSimpleToolBuilder
void UToSSimpleToolBuilder::StaticRegisterNativesUToSSimpleToolBuilder()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSSimpleToolBuilder);
UClass* Z_Construct_UClass_UToSSimpleToolBuilder_NoRegister()
{
	return UToSSimpleToolBuilder::StaticClass();
}
struct Z_Construct_UClass_UToSSimpleToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Builder for UToSSimpleTool\n */" },
#endif
		{ "IncludePath", "Tools/ToSSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Builder for UToSSimpleTool" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSSimpleToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UToSSimpleToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSSimpleToolBuilder_Statics::ClassParams = {
	&UToSSimpleToolBuilder::StaticClass,
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
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSSimpleToolBuilder_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSSimpleToolBuilder()
{
	if (!Z_Registration_Info_UClass_UToSSimpleToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSSimpleToolBuilder.OuterSingleton, Z_Construct_UClass_UToSSimpleToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSSimpleToolBuilder.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSSimpleToolBuilder>()
{
	return UToSSimpleToolBuilder::StaticClass();
}
UToSSimpleToolBuilder::UToSSimpleToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSSimpleToolBuilder);
UToSSimpleToolBuilder::~UToSSimpleToolBuilder() {}
// End Class UToSSimpleToolBuilder

// Begin Class UToSSimpleToolProperties
void UToSSimpleToolProperties::StaticRegisterNativesUToSSimpleToolProperties()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSSimpleToolProperties);
UClass* Z_Construct_UClass_UToSSimpleToolProperties_NoRegister()
{
	return UToSSimpleToolProperties::StaticClass();
}
struct Z_Construct_UClass_UToSSimpleToolProperties_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Settings UObject for UToSSimpleTool. This UClass inherits from UInteractiveToolPropertySet,\n * which provides an OnModified delegate that the Tool will listen to for changes in property values.\n */" },
#endif
		{ "IncludePath", "Tools/ToSSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Settings UObject for UToSSimpleTool. This UClass inherits from UInteractiveToolPropertySet,\nwhich provides an OnModified delegate that the Tool will listen to for changes in property values." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowExtendedInfo_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** If enabled, dialog should display extended information about the actor clicked on. Otherwise, only basic info will be shown. */" },
#endif
		{ "DisplayName", "Show Extended Info" },
		{ "ModuleRelativePath", "Private/Tools/ToSSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "If enabled, dialog should display extended information about the actor clicked on. Otherwise, only basic info will be shown." },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_ShowExtendedInfo_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowExtendedInfo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSSimpleToolProperties>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UToSSimpleToolProperties_Statics::NewProp_ShowExtendedInfo_SetBit(void* Obj)
{
	((UToSSimpleToolProperties*)Obj)->ShowExtendedInfo = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UToSSimpleToolProperties_Statics::NewProp_ShowExtendedInfo = { "ShowExtendedInfo", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UToSSimpleToolProperties), &Z_Construct_UClass_UToSSimpleToolProperties_Statics::NewProp_ShowExtendedInfo_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowExtendedInfo_MetaData), NewProp_ShowExtendedInfo_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UToSSimpleToolProperties_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSSimpleToolProperties_Statics::NewProp_ShowExtendedInfo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolProperties_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UToSSimpleToolProperties_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolPropertySet,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolProperties_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSSimpleToolProperties_Statics::ClassParams = {
	&UToSSimpleToolProperties::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UToSSimpleToolProperties_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolProperties_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleToolProperties_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSSimpleToolProperties_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSSimpleToolProperties()
{
	if (!Z_Registration_Info_UClass_UToSSimpleToolProperties.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSSimpleToolProperties.OuterSingleton, Z_Construct_UClass_UToSSimpleToolProperties_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSSimpleToolProperties.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSSimpleToolProperties>()
{
	return UToSSimpleToolProperties::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSSimpleToolProperties);
UToSSimpleToolProperties::~UToSSimpleToolProperties() {}
// End Class UToSSimpleToolProperties

// Begin Class UToSSimpleTool
void UToSSimpleTool::StaticRegisterNativesUToSSimpleTool()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSSimpleTool);
UClass* Z_Construct_UClass_UToSSimpleTool_NoRegister()
{
	return UToSSimpleTool::StaticClass();
}
struct Z_Construct_UClass_UToSSimpleTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UToSSimpleTool is an example Tool that opens a message box displaying info about an actor that the user\n * clicks left mouse button. All the action is in the ::OnClicked handler.\n */" },
#endif
		{ "IncludePath", "Tools/ToSSimpleTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSSimpleTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UToSSimpleTool is an example Tool that opens a message box displaying info about an actor that the user\nclicks left mouse button. All the action is in the ::OnClicked handler." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Properties_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tools/ToSSimpleTool.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Properties;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSSimpleTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UToSSimpleTool_Statics::NewProp_Properties = { "Properties", nullptr, (EPropertyFlags)0x0124080000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UToSSimpleTool, Properties), Z_Construct_UClass_UToSSimpleToolProperties_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Properties_MetaData), NewProp_Properties_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UToSSimpleTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSSimpleTool_Statics::NewProp_Properties,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleTool_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UToSSimpleTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_USingleClickTool,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSSimpleTool_Statics::ClassParams = {
	&UToSSimpleTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UToSSimpleTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSSimpleTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSSimpleTool_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSSimpleTool()
{
	if (!Z_Registration_Info_UClass_UToSSimpleTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSSimpleTool.OuterSingleton, Z_Construct_UClass_UToSSimpleTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSSimpleTool.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSSimpleTool>()
{
	return UToSSimpleTool::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSSimpleTool);
UToSSimpleTool::~UToSSimpleTool() {}
// End Class UToSSimpleTool

// Begin Registration
struct Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSSimpleTool_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UToSSimpleToolBuilder, UToSSimpleToolBuilder::StaticClass, TEXT("UToSSimpleToolBuilder"), &Z_Registration_Info_UClass_UToSSimpleToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSSimpleToolBuilder), 1752006754U) },
		{ Z_Construct_UClass_UToSSimpleToolProperties, UToSSimpleToolProperties::StaticClass, TEXT("UToSSimpleToolProperties"), &Z_Registration_Info_UClass_UToSSimpleToolProperties, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSSimpleToolProperties), 303858150U) },
		{ Z_Construct_UClass_UToSSimpleTool, UToSSimpleTool::StaticClass, TEXT("UToSSimpleTool"), &Z_Registration_Info_UClass_UToSSimpleTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSSimpleTool), 1588950074U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSSimpleTool_h_3774397588(TEXT("/Script/ToS"),
	Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSSimpleTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSSimpleTool_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
