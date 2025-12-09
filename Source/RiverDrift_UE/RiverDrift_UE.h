// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"


// declare a log called InitLog
DECLARE_LOG_CATEGORY_EXTERN(InitLog, Log, All);
// declare a log called AILog
DECLARE_LOG_CATEGORY_EXTERN(AiLog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(QuestLog, Log, All);




//#define LOG(msg, ...) UE_LOG(InitLog, Log, TEXT(msg), ##__VA_ARGS__)
//#define WARN(msg, ...) UE_LOG(InitLog, Warning, TEXT(msg), ##__VA_ARGS__)
//#define ERROR(msg, ...) UE_LOG(InitLog, Error, TEXT(msg), ##__VA_ARGS__)
//
//#define LOG(msg, ...) UE_LOGFMT(InitLog, Log, TEXT(msg), ##__VA_ARGS__)
//#define WARN(msg, ...) UE_LOG(InitLog, Warning, TEXT(msg), ##__VA_ARGS__)
//#define ERROR(msg, ...) UE_LOG(InitLog, Error, TEXT(msg), ##__VA_ARGS__)