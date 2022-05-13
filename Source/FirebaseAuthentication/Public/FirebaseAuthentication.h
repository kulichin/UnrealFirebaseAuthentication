// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "FirebaseAuthenticationSubsystem.h"
#include "Engine.h"

#define INITIALIZE_DELEGATE_HANDLER_Base(DelegateClass, DelegateName)	\
public:																	\
	DelegateClass DelegateName;											\
	void Add##DelegateName##_Handle(const DelegateClass& Delegate)		\
	{																	\
		ClearAll##DelegateName();										\
		DelegateName = Delegate;										\
	}																	\
	void ClearAll##DelegateName()										\
	{																	\
		if (DelegateName.IsBound())										\
		{																\
			DelegateName.Clear();										\
		}																\
	}

#define INITIALIZE_DELEGATE_HANDLER(DelegateClass, DelegateName)															\
		INITIALIZE_DELEGATE_HANDLER_Base(DelegateClass, DelegateName)														\
	void Trigger##DelegateName()																							\
	{																														\
		if (DelegateName.IsBound())																							\
		{																													\
			FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(FSimpleDelegateGraphTask::FDelegate::CreateLambda([this]()	\
			{																												\
				DelegateName.Execute();																						\
				ClearAll##DelegateName();																					\
			}),																												\
			TStatId(), nullptr, ENamedThreads::GameThread);																	\
		}																													\
	}

#define INITIALIZE_DELEGATE_HANDLER_OneParam(DelegateClass, DelegateName, FirstParamType, FirstParamName)	\
		INITIALIZE_DELEGATE_HANDLER_Base(DelegateClass, DelegateName)										\
	void Trigger##DelegateName(FirstParamType FirstParamName)												\
	{																										\
		if (DelegateName.IsBound())																			\
		{																									\
			FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(											\
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([FirstParamName, this]()						\
			{																								\
				DelegateName.Execute(FirstParamName);														\
				ClearAll##DelegateName();																	\
			}),																								\
			TStatId(), nullptr, ENamedThreads::GameThread);													\
		}																									\
	}

#define INITIALIZE_DELEGATE_HANDLER_TwoParams(															\
		DelegateClass, DelegateName,																	\
		FirstParamType, FirstParamName,	SecondParamType, SecondParamName)								\
		INITIALIZE_DELEGATE_HANDLER_Base(DelegateClass, DelegateName)									\
	void Trigger##DelegateName(FirstParamType FirstParamName, SecondParamType SecondParamName)			\
	{																									\
		if (DelegateName.IsBound())																		\
		{																								\
			FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(										\
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([FirstParamName, SecondParamName, this]()	\
			{																							\
				DelegateName.Execute(FirstParamName, SecondParamName);									\
				ClearAll##DelegateName();																\
			}),																							\
			TStatId(), nullptr, ENamedThreads::GameThread);												\
		}																								\
	}

#define INITIALIZE_DELEGATE_HANDLER_ThreeParams(																				\
		DelegateClass, DelegateName,																							\
		FirstParamType, FirstParamName,	SecondParamType, SecondParamName, ThirdParamType, ThirdParamName)						\
		INITIALIZE_DELEGATE_HANDLER_Base(DelegateClass, DelegateName)															\
	void Trigger##DelegateName(FirstParamType FirstParamName, SecondParamType SecondParamName, ThirdParamType ThirdParamName)	\
	{																															\
		if (DelegateName.IsBound())																								\
		{																														\
			FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(																\
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([FirstParamName, SecondParamName, ThirdParamName, this]()			\
			{																													\
				DelegateName.Execute(FirstParamName, SecondParamName, ThirdParamName);											\
				ClearAll##DelegateName();																						\
			}),																													\
			TStatId(), nullptr, ENamedThreads::GameThread);																		\
		}																														\
	}

class FFirebaseAuthenticationModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    static FFirebaseAuthenticationModule* GetModule();

#if PLATFORM_ANDROID
	/** Generic firebase delegates handlers */
	INITIALIZE_DELEGATE_HANDLER_OneParam(FOnFirebaseFunctionCompleted, OnFirebaseFunctionCompleted,	const ECommonStatusCodes, StatusCode)
	INITIALIZE_DELEGATE_HANDLER_TwoParams(FOnFirebaseAuthenticationCompleted, OnFirebaseAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FFirebaseUser&, FirebaseUser)

	/** Phone delegates handlers */
	INITIALIZE_DELEGATE_HANDLER_ThreeParams(FOnPhoneAuthenticationCompleted, OnPhoneAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FFirebaseUser&, FirebaseUser, const FString&, CodeFromSMS)
	
	/** Google delegates handlers */
	INITIALIZE_DELEGATE_HANDLER_ThreeParams(FOnGoogleAuthenticationCompleted, OnGoogleAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FGoogleSignInAccount&, GoogleSignInAccount, const FFirebaseUser&, FirebaseUser)
#endif
};
