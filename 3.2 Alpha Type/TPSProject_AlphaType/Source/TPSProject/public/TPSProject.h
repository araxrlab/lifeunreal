// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All);

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define PRINT_CALLINFO() UE_LOG(TPS, Warning, TEXT("%s"), *CALLINFO)

#define PRINT_LOG(fmt, ...) UE_LOG(TPS, Warning, TEXT("%s %s"), *CALLINFO, \
*FString::Printf(fmt, ##__VA_ARGS__))