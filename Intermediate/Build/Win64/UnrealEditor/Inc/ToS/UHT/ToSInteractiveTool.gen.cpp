// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ToS/Private/Tools/ToSInteractiveTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToSInteractiveTool() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveTool();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolPropertySet();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveTool();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveTool_NoRegister();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveToolBuilder();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveToolBuilder_NoRegister();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveToolProperties();
TOS_API UClass* Z_Construct_UClass_UToSInteractiveToolProperties_NoRegister();
UPackage* Z_Construct_UPackage__Script_ToS();
// End Cross Module References

// Begin Class UToSInteractiveToolBuilder
void UToSInteractiveToolBuilder::StaticRegisterNativesUToSInteractiveToolBuilder()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSInteractiveToolBuilder);
UClass* Z_Construct_UClass_UToSInteractiveToolBuilder_NoRegister()
{
	return UToSInteractiveToolBuilder::StaticClass();
}
struct Z_Construct_UClass_UToSInteractiveToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Builder for UToSInteractiveTool\n */" },
#endif
		{ "IncludePath", "Tools/ToSInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Builder for UToSInteractiveTool" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSInteractiveToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::ClassParams = {
	&UToSInteractiveToolBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSInteractiveToolBuilder()
{
	if (!Z_Registration_Info_UClass_UToSInteractiveToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSInteractiveToolBuilder.OuterSingleton, Z_Construct_UClass_UToSInteractiveToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSInteractiveToolBuilder.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSInteractiveToolBuilder>()
{
	return UToSInteractiveToolBuilder::StaticClass();
}
UToSInteractiveToolBuilder::UToSInteractiveToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSInteractiveToolBuilder);
UToSInteractiveToolBuilder::~UToSInteractiveToolBuilder() {}
// End Class UToSInteractiveToolBuilder

// Begin Class UToSInteractiveToolProperties
void UToSInteractiveToolProperties::StaticRegisterNativesUToSInteractiveToolProperties()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSInteractiveToolProperties);
UClass* Z_Construct_UClass_UToSInteractiveToolProperties_NoRegister()
{
	return UToSInteractiveToolProperties::StaticClass();
}
struct Z_Construct_UClass_UToSInteractiveToolProperties_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Property set for the UToSInteractiveTool\n */" },
#endif
		{ "IncludePath", "Tools/ToSInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Property set for the UToSInteractiveTool" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StartPoint_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** First point of measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "First point of measurement" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EndPoint_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Second point of measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Second point of measurement" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Distance_MetaData[] = {
		{ "Category", "Options" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Current distance measurement */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Current distance measurement" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_StartPoint;
	static const UECodeGen_Private::FStructPropertyParams NewProp_EndPoint;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_Distance;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSInteractiveToolProperties>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_StartPoint = { "StartPoint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UToSInteractiveToolProperties, StartPoint), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StartPoint_MetaData), NewProp_StartPoint_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_EndPoint = { "EndPoint", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UToSInteractiveToolProperties, EndPoint), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EndPoint_MetaData), NewProp_EndPoint_MetaData) };
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_Distance = { "Distance", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UToSInteractiveToolProperties, Distance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Distance_MetaData), NewProp_Distance_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UToSInteractiveToolProperties_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_StartPoint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_EndPoint,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSInteractiveToolProperties_Statics::NewProp_Distance,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolProperties_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UToSInteractiveToolProperties_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolPropertySet,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolProperties_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSInteractiveToolProperties_Statics::ClassParams = {
	&UToSInteractiveToolProperties::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UToSInteractiveToolProperties_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolProperties_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveToolProperties_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSInteractiveToolProperties_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSInteractiveToolProperties()
{
	if (!Z_Registration_Info_UClass_UToSInteractiveToolProperties.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSInteractiveToolProperties.OuterSingleton, Z_Construct_UClass_UToSInteractiveToolProperties_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSInteractiveToolProperties.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSInteractiveToolProperties>()
{
	return UToSInteractiveToolProperties::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSInteractiveToolProperties);
UToSInteractiveToolProperties::~UToSInteractiveToolProperties() {}
// End Class UToSInteractiveToolProperties

// Begin Class UToSInteractiveTool
void UToSInteractiveTool::StaticRegisterNativesUToSInteractiveTool()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UToSInteractiveTool);
UClass* Z_Construct_UClass_UToSInteractiveTool_NoRegister()
{
	return UToSInteractiveTool::StaticClass();
}
struct Z_Construct_UClass_UToSInteractiveTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UToSInteractiveTool is an example Tool that allows the user to measure the \n * distance between two points. The first point is set by click-dragging the mouse, and\n * the second point is set by shift-click-dragging the mouse.\n */" },
#endif
		{ "IncludePath", "Tools/ToSInteractiveTool.h" },
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UToSInteractiveTool is an example Tool that allows the user to measure the\ndistance between two points. The first point is set by click-dragging the mouse, and\nthe second point is set by shift-click-dragging the mouse." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Properties_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Properties of the tool are stored here */" },
#endif
		{ "ModuleRelativePath", "Private/Tools/ToSInteractiveTool.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Properties of the tool are stored here" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Properties;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UToSInteractiveTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UToSInteractiveTool_Statics::NewProp_Properties = { "Properties", nullptr, (EPropertyFlags)0x0124080000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UToSInteractiveTool, Properties), Z_Construct_UClass_UToSInteractiveToolProperties_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Properties_MetaData), NewProp_Properties_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UToSInteractiveTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UToSInteractiveTool_Statics::NewProp_Properties,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveTool_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UToSInteractiveTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveTool,
	(UObject* (*)())Z_Construct_UPackage__Script_ToS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UToSInteractiveTool_Statics::ClassParams = {
	&UToSInteractiveTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UToSInteractiveTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UToSInteractiveTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UToSInteractiveTool_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UToSInteractiveTool()
{
	if (!Z_Registration_Info_UClass_UToSInteractiveTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UToSInteractiveTool.OuterSingleton, Z_Construct_UClass_UToSInteractiveTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UToSInteractiveTool.OuterSingleton;
}
template<> TOS_API UClass* StaticClass<UToSInteractiveTool>()
{
	return UToSInteractiveTool::StaticClass();
}
UToSInteractiveTool::UToSInteractiveTool() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UToSInteractiveTool);
UToSInteractiveTool::~UToSInteractiveTool() {}
// End Class UToSInteractiveTool

// Begin Registration
struct Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSInteractiveTool_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UToSInteractiveToolBuilder, UToSInteractiveToolBuilder::StaticClass, TEXT("UToSInteractiveToolBuilder"), &Z_Registration_Info_UClass_UToSInteractiveToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSInteractiveToolBuilder), 736667349U) },
		{ Z_Construct_UClass_UToSInteractiveToolProperties, UToSInteractiveToolProperties::StaticClass, TEXT("UToSInteractiveToolProperties"), &Z_Registration_Info_UClass_UToSInteractiveToolProperties, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSInteractiveToolProperties), 4030008609U) },
		{ Z_Construct_UClass_UToSInteractiveTool, UToSInteractiveTool::StaticClass, TEXT("UToSInteractiveTool"), &Z_Registration_Info_UClass_UToSInteractiveTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UToSInteractiveTool), 2729371124U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSInteractiveTool_h_4232306688(TEXT("/Script/ToS"),
	Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSInteractiveTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_NetworkSample_Plugins_ToS_Source_ToS_Private_Tools_ToSInteractiveTool_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
