// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#include "FirebaseAuthenticationSubsystem.h"
#include "FirebaseAuthentication.h"

#if PLATFORM_ANDROID

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

// Authentication methods
static jmethodID AnonymousSignIn_MethodID;
static jmethodID LinkAnonymousAccountWithEmail_MethodID;
static jmethodID EmailSignIn_MethodID;
static jmethodID CreateAccountWithEmail_MethodID;
static jmethodID SendEmailVerification_MethodID;
static jmethodID SendPasswordResetLinkOnEmail_MethodID;
static jmethodID UpdatePassword_MethodID;
static jmethodID FacebookSignIn_MethodID;
static jmethodID FacebookSignOut_MethodID;
static jmethodID GoogleSignIn_MethodID;
static jmethodID GoogleSignOut_MethodID;
static jmethodID GoogleRevokeAccess_MethodID;
static jmethodID OAuthSignIn_MethodID;
static jmethodID SignInWithSMSCode_MethodID;
static jmethodID GetFirebaseUser_MethodID;
static jmethodID ReloadFirebaseUser_MethodID;
static jmethodID FirebaseSignOut_MethodID;

// GoogleSignInAccountWrapper class fields
static jfieldID GoogleSignInAccount_DisplayName_FieldID;
static jfieldID GoogleSignInAccount_Email_FieldID;
static jfieldID GoogleSignInAccount_FamilyName_FieldID;
static jfieldID GoogleSignInAccount_GivenName_FieldID;
static jfieldID GoogleSignInAccount_GrantedScopes_FieldID;
static jfieldID GoogleSignInAccount_ID_FieldID;
static jfieldID GoogleSignInAccount_IDToken_FieldID;
static jfieldID GoogleSignInAccount_PhotoURL_FieldID;
static jfieldID GoogleSignInAccount_ServerAuthCode_FieldID;

// FirebaseUserWrapper class fields
static jfieldID FirebaseUser_DisplayName_FieldID;
static jfieldID FirebaseUser_Email_FieldID;
static jfieldID FirebaseUser_CreationTimestamp_FieldID;
static jfieldID FirebaseUser_LastSignInTimestamp_FieldID;
static jfieldID FirebaseUser_PhoneNumber_FieldID;
static jfieldID FirebaseUser_PhotoURL_FieldID;
static jfieldID FirebaseUser_bIsEmailVerified_FieldID;
static jfieldID FirebaseUser_ProviderID_FieldID;
static jfieldID FirebaseUser_UID_FieldID;
static jfieldID FirebaseUser_bIsAnonymous_FieldID;

static TArray<FString> GetStringsArrayFromObject(
	JNIEnv* Env,
	jobject Object,
	jfieldID FieldID)
{
	if (FieldID == NULL || Object == NULL || Env == NULL)
	{
		return {};
	}

	jobject RawStringsArray = Env->GetObjectField(Object, FieldID);
	jobjectArray* StringsArray = reinterpret_cast<jobjectArray*>(&RawStringsArray);
	jsize StringsArraySize = Env->GetArrayLength(*StringsArray);

	// Convert java strings to ue strings
	TArray<FString> UEStringsArray = {};
	for (int Idx = 0; Idx < StringsArraySize; Idx++)
	{
		jstring JString = (jstring)Env->GetObjectArrayElement(*StringsArray, Idx);
		UEStringsArray.Add(FJavaHelper::FStringFromParam(Env, JString));

		// Cleanup temporary java string
		Env->DeleteLocalRef(JString);
	}

	// Cleanup references
	Env->DeleteLocalRef(RawStringsArray);
	return UEStringsArray;
}

static FScopedJavaObject<jstring> GetScopedStringFromObject(
	JNIEnv* Env, 
	jobject Object, 
	jfieldID FieldID)
{
	return NewScopedJavaObject(Env, (jstring) Env->GetObjectField(Object, FieldID));
}

static jmethodID FindMethod(JNIEnv* Env, const char* Name, const char* Signature)
{
	if (Env && Name && Signature)
	{
		return Env->GetMethodID(FJavaWrapper::GameActivityClassID, Name, Signature);
	}

	return nullptr;
}

static jfieldID FindField(JNIEnv* Env, jclass Class, const char* Name, const char* Type)
{
	if (Env && Name && Type && Class)
	{
		return Env->GetFieldID(Class, Name, Type);
	}

	return nullptr;
}

static void CallVoidMethod(JNIEnv* Env, jmethodID Method, ...)
{
    // make sure the function exists
	jobject Object = FJavaWrapper::GameActivityThis;
	if (Method == NULL || Object == NULL || Env == NULL)
	{
		return;
	}
	

	va_list Args;
	va_start(Args, Method);
	Env->CallVoidMethodV(Object, Method, Args);
	va_end(Args);
}

static jobject CallObjectMethod(JNIEnv* Env, jmethodID Method)
{
    // make sure the function exists
	jobject Object = FJavaWrapper::GameActivityThis;
	if (Method == NULL || Object == NULL || Env == NULL)
	{
		return NULL;
	}

	return Env->CallObjectMethod(Object, Method);
}

static FGoogleSignInAccount GetGoogleSignInAccountStructFromJavaObject(
	JNIEnv* Env, 
	jobject Object)
{
	if (Object == NULL || Env == NULL)
	{
		return {};
	}
	
	// GoogleSignInAccountWrapper class fields
	auto JDisplayName_Google	= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_DisplayName_FieldID);
	auto JEmail_Google			= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_Email_FieldID);
	auto JFamilyName_Google		= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_FamilyName_FieldID);
	auto JGivenName_Google		= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_GivenName_FieldID);
	auto GrantedScopes_Google	= GetStringsArrayFromObject(Env, Object, GoogleSignInAccount_GrantedScopes_FieldID);
	auto JID_Google				= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_ID_FieldID);
	auto JIDToken_Google		= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_IDToken_FieldID);
	auto JPhotoURL_Google		= GetScopedStringFromObject(Env, Object, GoogleSignInAccount_PhotoURL_FieldID);
	auto JServerAuthCode_Google = GetScopedStringFromObject(Env, Object, GoogleSignInAccount_ServerAuthCode_FieldID);
	
	// Fill in GoogleSignInAccount out structure
	FGoogleSignInAccount OutGoogleSignInAccount
	{
		FJavaHelper::FStringFromParam(Env, *JDisplayName_Google),
		FJavaHelper::FStringFromParam(Env, *JEmail_Google),
		FJavaHelper::FStringFromParam(Env, *JFamilyName_Google),
		FJavaHelper::FStringFromParam(Env, *JGivenName_Google),
		GrantedScopes_Google,
		FJavaHelper::FStringFromParam(Env, *JID_Google),
		FJavaHelper::FStringFromParam(Env, *JIDToken_Google),
		FJavaHelper::FStringFromParam(Env, *JPhotoURL_Google),
		FJavaHelper::FStringFromParam(Env, *JServerAuthCode_Google),
	};

	return OutGoogleSignInAccount;
}

static FFirebaseUser GetFirebaseUserStructFromJavaObject(
	JNIEnv* Env, 
	jobject Object)
{
	if (Object == NULL || Env == NULL)
	{
		return {};
	}

	// Get data from FirebaseUser object
	auto JDisplayName			= GetScopedStringFromObject(Env, Object, FirebaseUser_DisplayName_FieldID);
	auto JEmail					= GetScopedStringFromObject(Env, Object, FirebaseUser_Email_FieldID);
	auto JPhoneNumber			= GetScopedStringFromObject(Env, Object, FirebaseUser_PhoneNumber_FieldID);
	auto JPhotoURL				= GetScopedStringFromObject(Env, Object, FirebaseUser_PhotoURL_FieldID);
	auto JProviderID			= GetScopedStringFromObject(Env, Object, FirebaseUser_ProviderID_FieldID);
	auto JUID					= GetScopedStringFromObject(Env, Object, FirebaseUser_UID_FieldID);
	int64 CreationTimestamp		= (int64)Env->GetLongField(Object, FirebaseUser_CreationTimestamp_FieldID);
	int64 LastSignInTimestamp	= (int64)Env->GetLongField(Object, FirebaseUser_LastSignInTimestamp_FieldID);
	bool bIsEmailVerified		= (bool)Env->GetBooleanField(Object, FirebaseUser_bIsEmailVerified_FieldID);
	bool bIsAnonymous			= (bool)Env->GetBooleanField(Object, FirebaseUser_bIsAnonymous_FieldID);

	// Fill in FirebaseUser out structure
	FFirebaseUser OutFirebaseUser = 
	{
		FJavaHelper::FStringFromParam(Env, *JDisplayName),
		FJavaHelper::FStringFromParam(Env, *JEmail),
		CreationTimestamp,
		LastSignInTimestamp,
		FJavaHelper::FStringFromParam(Env, *JPhoneNumber),
		FJavaHelper::FStringFromParam(Env, *JPhotoURL),
		bIsEmailVerified,
		FJavaHelper::FStringFromParam(Env, *JProviderID),
		FJavaHelper::FStringFromParam(Env, *JUID),
		bIsAnonymous
	};

	return OutFirebaseUser;
}

#endif

FFirebaseUser UFirebaseAuthenticationSubsystem::GetFirebaseUser()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Call Java method
			auto JFirebaseUser = NewScopedJavaObject(Env, CallObjectMethod(Env, GetFirebaseUser_MethodID));

			// Wrap java data to ue struct
			return GetFirebaseUserStructFromJavaObject(Env, *JFirebaseUser);
		}
	}
#endif
	
	return {};
}

void UFirebaseAuthenticationSubsystem::ReloadFirebaseUser(
	FOnFirebaseAuthenticationCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseAuthenticationCompleted_Handle(OnOperationResult);

			// Call Java method
			CallVoidMethod(Env, ReloadFirebaseUser_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::AnonymousSignIn(
	FOnFirebaseAuthenticationCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseAuthenticationCompleted_Handle(OnOperationResult);

			// Call Java method
			CallVoidMethod(Env, AnonymousSignIn_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::AnonymousSignOut()
{
#if PLATFORM_ANDROID
	FirebaseSignOut();
#endif
}

void UFirebaseAuthenticationSubsystem::LinkAnonymousAccountWithEmail(
	FOnFirebaseFunctionCompleted OnOperationResult,
	const FString& Email, 
	const FString& Password)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Init Java strings
			auto JEmail = FJavaHelper::ToJavaString(Env, Email);
			auto JPassword = FJavaHelper::ToJavaString(Env, Password);

			// Call Java method
			CallVoidMethod(Env, LinkAnonymousAccountWithEmail_MethodID, *JEmail, *JPassword);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::EmailSignIn(
	FOnFirebaseAuthenticationCompleted OnOperationResult,
	const FString& Email, 
	const FString& Password)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseAuthenticationCompleted_Handle(OnOperationResult);
			
			// Init Java strings
			auto JEmail = FJavaHelper::ToJavaString(Env, Email);
			auto JPassword = FJavaHelper::ToJavaString(Env, Password);

			// Call Java method
			CallVoidMethod(Env, EmailSignIn_MethodID, *JEmail, *JPassword);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::CreateAccountWithEmail(
	FOnFirebaseFunctionCompleted OnOperationResult,
	const FString& Email, 
	const FString& Password)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Init Java strings
			auto JEmail = FJavaHelper::ToJavaString(Env, Email);
			auto JPassword = FJavaHelper::ToJavaString(Env, Password);

			// Call Java method
			CallVoidMethod(Env, CreateAccountWithEmail_MethodID, *JEmail, *JPassword);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::SendEmailVerification(
	FOnFirebaseFunctionCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Call Java method
			CallVoidMethod(Env, SendEmailVerification_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::SendPasswordResetLinkOnEmail(
	FOnFirebaseFunctionCompleted OnOperationResult, 
	const FString& Email)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Init Java strings
			auto JEmail = FJavaHelper::ToJavaString(Env, Email);

			// Call Java method
			CallVoidMethod(Env, SendPasswordResetLinkOnEmail_MethodID, *JEmail);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::UpdatePassword(
	FOnFirebaseFunctionCompleted OnOperationResult, 
	const FString& NewPassword)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Init Java strings
			auto JNewPassword = FJavaHelper::ToJavaString(Env, NewPassword);

			// Call Java method
			CallVoidMethod(Env, UpdatePassword_MethodID, *JNewPassword);
		}
	}
#endif

}

void UFirebaseAuthenticationSubsystem::EmailSignOut()
{
#if PLATFORM_ANDROID
	FirebaseSignOut();
#endif
}

void UFirebaseAuthenticationSubsystem::FacebookSignIn(
	FOnFirebaseAuthenticationCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseAuthenticationCompleted_Handle(OnOperationResult);

			// Call Java method
			CallVoidMethod(Env, FacebookSignIn_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::FacebookSignOut()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Call Java method
		CallVoidMethod(Env, FacebookSignOut_MethodID);
	}
#endif
}

void UFirebaseAuthenticationSubsystem::GoogleSignIn(
	FOnGoogleAuthenticationCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnGoogleAuthenticationCompleted_Handle(OnOperationResult);

			// Call Java method
			CallVoidMethod(Env, GoogleSignIn_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::GoogleSignOut(
	FOnFirebaseFunctionCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);
			
			// Call Java method
			CallVoidMethod(Env, GoogleSignOut_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::GoogleRevokeAccess(
	FOnFirebaseFunctionCompleted OnOperationResult)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseFunctionCompleted_Handle(OnOperationResult);

			// Call Java method
			CallVoidMethod(Env, GoogleRevokeAccess_MethodID);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::OAuthSignIn(
	FOnFirebaseAuthenticationCompleted OnOperationResult,
	const EOAuthProvider OAuthProvider)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callback
			Module->AddOnFirebaseAuthenticationCompleted_Handle(OnOperationResult);

			// Init Java string
			jstring JProviderURL;
			switch (OAuthProvider)
			{
				case EOAuthProvider::Apple:
					JProviderURL = Env->NewStringUTF("apple.com");
					break;
				case EOAuthProvider::Github:
					JProviderURL = Env->NewStringUTF("github.com");
					break;
				case EOAuthProvider::Microsoft:
					JProviderURL = Env->NewStringUTF("microsoft.com");
					break;
				case EOAuthProvider::Yahoo:
					JProviderURL = Env->NewStringUTF("yahoo.com");
					break;
				case EOAuthProvider::Twitter:
					JProviderURL = Env->NewStringUTF("twitter.com");
					break;
			}

			// Call Java method
			CallVoidMethod(Env, OAuthSignIn_MethodID, JProviderURL);

			// Remove Java reference
			Env->DeleteLocalRef(JProviderURL);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::OAuthSignOut()
{
#if PLATFORM_ANDROID
	FirebaseSignOut();
#endif
}

void UFirebaseAuthenticationSubsystem::SignInWithSMSCode(
	FOnPhoneAuthenticationCompleted OnAuthenticationResult,
	FOnFirebaseFunctionCompleted OnCodeSentResult,
	const FString& PhoneNumber, 
	const int Timeout)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
		{
			// Bind callbacks
			Module->AddOnPhoneAuthenticationCompleted_Handle(OnAuthenticationResult);
			Module->AddOnFirebaseFunctionCompleted_Handle(OnCodeSentResult);

			// Init Java string
			auto JPhoneNumber = FJavaHelper::ToJavaString(Env, PhoneNumber);

			// Call Java method
			CallVoidMethod(Env, SignInWithSMSCode_MethodID, *JPhoneNumber, Timeout);
		}
	}
#endif
}

void UFirebaseAuthenticationSubsystem::PhoneSignOut()
{
#if PLATFORM_ANDROID
	FirebaseSignOut();
#endif
}

void UFirebaseAuthenticationSubsystem::FirebaseSignOut()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		// Call Java method
		CallVoidMethod(Env, FirebaseSignOut_MethodID);
	}
#endif
}

#if PLATFORM_ANDROID

JNI_METHOD void Java_com_kulichin_firebaseauthentication_BaseAuthentication_NativeInitialize(
	JNIEnv* Env, 
	jobject Thiz,
	jclass GoogleSignInAccountWrapperClass,
	jclass FirebaseUserWrapperClass)
{
	// Find methods ids in game activity
    AnonymousSignIn_MethodID				= FindMethod(Env, "AndroidThunkJava_AnonymousSignIn",				"()V");
    LinkAnonymousAccountWithEmail_MethodID	= FindMethod(Env, "AndroidThunkJava_LinkAnonymousAccountWithEmail", "(Ljava/lang/String;Ljava/lang/String;)V");
    EmailSignIn_MethodID					= FindMethod(Env, "AndroidThunkJava_EmailSignIn",					"(Ljava/lang/String;Ljava/lang/String;)V");
    CreateAccountWithEmail_MethodID			= FindMethod(Env, "AndroidThunkJava_CreateAccountWithEmail",		"(Ljava/lang/String;Ljava/lang/String;)V");
    SendEmailVerification_MethodID			= FindMethod(Env, "AndroidThunkJava_SendEmailVerification",			"()V");
	SendPasswordResetLinkOnEmail_MethodID	= FindMethod(Env, "AndroidThunkJava_SendPasswordResetLinkOnEmail",	"(Ljava/lang/String;)V");
	UpdatePassword_MethodID					= FindMethod(Env, "AndroidThunkJava_UpdatePassword",				"(Ljava/lang/String;)V");
    FacebookSignIn_MethodID					= FindMethod(Env, "AndroidThunkJava_FacebookSignIn",				"()V");
    FacebookSignOut_MethodID				= FindMethod(Env, "AndroidThunkJava_FacebookSignOut",				"()V");
    GoogleSignIn_MethodID					= FindMethod(Env, "AndroidThunkJava_GoogleSignIn",					"()V");
    GoogleSignOut_MethodID					= FindMethod(Env, "AndroidThunkJava_GoogleSignOut",					"()V");
    GoogleRevokeAccess_MethodID				= FindMethod(Env, "AndroidThunkJava_GoogleRevokeAccess",			"()V");
    OAuthSignIn_MethodID					= FindMethod(Env, "AndroidThunkJava_OAuthSignIn",					"(Ljava/lang/String;)V");
    SignInWithSMSCode_MethodID				= FindMethod(Env, "AndroidThunkJava_SignInWithSMSCode",				"(Ljava/lang/String;I)V");
	GetFirebaseUser_MethodID				= FindMethod(Env, "AndroidThunkJava_GetFirebaseUser",				"()Lcom/kulichin/firebaseauthentication/BaseAuthentication$FirebaseUserWrapper;");
    ReloadFirebaseUser_MethodID				= FindMethod(Env, "AndroidThunkJava_ReloadFirebaseUser",			"()V");
    FirebaseSignOut_MethodID				= FindMethod(Env, "AndroidThunkJava_FirebaseSignOut",				"()V");

	// Find fields ids from GoogleSignInAccountWrapper class
	GoogleSignInAccount_DisplayName_FieldID		= FindField(Env, GoogleSignInAccountWrapperClass, "DisplayName",	"Ljava/lang/String;");
	GoogleSignInAccount_Email_FieldID			= FindField(Env, GoogleSignInAccountWrapperClass, "Email",			"Ljava/lang/String;");
	GoogleSignInAccount_FamilyName_FieldID		= FindField(Env, GoogleSignInAccountWrapperClass, "FamilyName",		"Ljava/lang/String;");
	GoogleSignInAccount_GivenName_FieldID		= FindField(Env, GoogleSignInAccountWrapperClass, "GivenName",		"Ljava/lang/String;");
	GoogleSignInAccount_GrantedScopes_FieldID	= FindField(Env, GoogleSignInAccountWrapperClass, "GrantedScopes",	"[Ljava/lang/String;");
	GoogleSignInAccount_ID_FieldID				= FindField(Env, GoogleSignInAccountWrapperClass, "ID",				"Ljava/lang/String;");
	GoogleSignInAccount_IDToken_FieldID			= FindField(Env, GoogleSignInAccountWrapperClass, "IDToken",		"Ljava/lang/String;");
	GoogleSignInAccount_PhotoURL_FieldID		= FindField(Env, GoogleSignInAccountWrapperClass, "PhotoURL",		"Ljava/lang/String;");
	GoogleSignInAccount_ServerAuthCode_FieldID	= FindField(Env, GoogleSignInAccountWrapperClass, "ServerAuthCode", "Ljava/lang/String;");
	
	// Find fields ids from FirebaseUserWrapper class
	FirebaseUser_DisplayName_FieldID			= FindField(Env, FirebaseUserWrapperClass, "DisplayName",			"Ljava/lang/String;");
	FirebaseUser_Email_FieldID					= FindField(Env, FirebaseUserWrapperClass, "Email",					"Ljava/lang/String;");
	FirebaseUser_CreationTimestamp_FieldID		= FindField(Env, FirebaseUserWrapperClass, "CreationTimestamp",		"J");
	FirebaseUser_LastSignInTimestamp_FieldID	= FindField(Env, FirebaseUserWrapperClass, "LastSignInTimestamp",	"J");
	FirebaseUser_PhoneNumber_FieldID			= FindField(Env, FirebaseUserWrapperClass, "PhoneNumber",			"Ljava/lang/String;");
	FirebaseUser_PhotoURL_FieldID				= FindField(Env, FirebaseUserWrapperClass, "PhotoURL",				"Ljava/lang/String;");
	FirebaseUser_bIsEmailVerified_FieldID		= FindField(Env, FirebaseUserWrapperClass, "bIsEmailVerified",		"Z");
	FirebaseUser_ProviderID_FieldID				= FindField(Env, FirebaseUserWrapperClass, "ProviderID",			"Ljava/lang/String;");
	FirebaseUser_UID_FieldID					= FindField(Env, FirebaseUserWrapperClass, "UID",					"Ljava/lang/String;");
	FirebaseUser_bIsAnonymous_FieldID			= FindField(Env, FirebaseUserWrapperClass, "bIsAnonymous",			"Z");
}

JNI_METHOD void Java_com_kulichin_firebaseauthentication_BaseAuthentication_NativeFirebaseFunctionResult(
	JNIEnv* Env, 
	jobject Thiz, 
	jint StatusCode)
{
	if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
	{
		Module->TriggerOnFirebaseFunctionCompleted((ECommonStatusCodes)StatusCode);
	}
}

JNI_METHOD void Java_com_kulichin_firebaseauthentication_BaseAuthentication_NativeFirebaseAuthenticationResult(
	JNIEnv* Env, 
	jobject Thiz,
	jobject FirebaseUser,
	jint StatusCode)
{
	if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
	{
		FFirebaseUser OutFirebaseUser = GetFirebaseUserStructFromJavaObject(
			Env, 
			FirebaseUser);
		
		Module->TriggerOnFirebaseAuthenticationCompleted(
			(ECommonStatusCodes)StatusCode,
			OutFirebaseUser);
	}
}

JNI_METHOD void Java_com_kulichin_firebaseauthentication_BaseAuthentication_NativePhoneAuthenticationResult(
	JNIEnv* Env, 
	jobject Thiz,
	jstring CodeFromSMS,
	jobject FirebaseUser,
	jint StatusCode)
{
	if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
	{
		FFirebaseUser OutFirebaseUser = GetFirebaseUserStructFromJavaObject(
			Env, 
			FirebaseUser);

		const FString UECodeFromSMS = FJavaHelper::FStringFromParam(Env, CodeFromSMS);
		
		Module->TriggerOnPhoneAuthenticationCompleted(
			(ECommonStatusCodes)StatusCode,
			OutFirebaseUser,
			UECodeFromSMS);
	}
}

JNI_METHOD void Java_com_kulichin_firebaseauthentication_GoogleAuthentication_NativeGoogleAuthenticationResult(
	JNIEnv* Env, 
	jobject Thiz,
	jobject GoogleSignInAccount,
	jobject FirebaseUser,
	jint StatusCode)
{
	if (FFirebaseAuthenticationModule* Module = FFirebaseAuthenticationModule::GetModule())
	{
		FFirebaseUser OutFirebaseUser = GetFirebaseUserStructFromJavaObject(
			Env, 
			FirebaseUser);

		FGoogleSignInAccount OutGoogleSignInAccount = GetGoogleSignInAccountStructFromJavaObject(
			Env, 
			GoogleSignInAccount);
		
		Module->TriggerOnGoogleAuthenticationCompleted(
			(ECommonStatusCodes)StatusCode, 
			OutGoogleSignInAccount, 
			OutFirebaseUser);
	}
}

#endif
