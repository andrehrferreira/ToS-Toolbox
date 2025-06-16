// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToS_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_ToS;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_ToS()
	{
		if (!Z_Registration_Info_UPackage__Script_ToS.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/ToS",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000040,
				0x13AF4FC9,
				0x93A8391F,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_ToS.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_ToS.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_ToS(Z_Construct_UPackage__Script_ToS, TEXT("/Script/ToS"), Z_Registration_Info_UPackage__Script_ToS, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x13AF4FC9, 0x93A8391F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
