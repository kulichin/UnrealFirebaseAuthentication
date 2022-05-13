// Copyright (C) 2021. Nikita Klimov. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FirebaseAuthenticationSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FGoogleSignInAccount
{
	GENERATED_BODY()

	/**
	 *	Returns the display name of the signed in user if you built your configuration
	 *	starting from new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)}
	 *	or with GoogleSignInOptions.Builder.requestProfile() configured; "" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString DisplayName;

	/*
	 *	Returns the email address of the signed in user if GoogleSignInOptions.Builder.requestEmail()
	 *	was configured; "" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString Email;

	/**
	 *	Returns the family name of the signed in user if you built your configuration
	 *	starting from new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)}
	 *	or with GoogleSignInOptions.Builder.requestProfile() configured; "" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString FamilyName;

	/**
	 *	Returns the given name of the signed in user if you built your configuration
	 *	starting from new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)}
	 *	or with GoogleSignInOptions.Builder.requestProfile() configured; "" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString GivenName;

	/**
	 *	Returns all scopes that have been authorized to your application.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	TArray<FString> GrantedScopes;

	/**
	 *	Returns the unique ID for the Google account if you built your configuration
	 *	starting from new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)}
	 *	or with GoogleSignInOptions.Builder.requestId() configured; "" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString ID;

	/**
	 *	Returns an ID token that you can send to your server if
	 *	GoogleSignInOptions.Builder.requestIdToken(String) was configured;
	 *	"" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString IDToken;

	/**
	 *	Returns the photo url of the signed in user if the user has a profile picture
	 *	and you built your configuration either starting from
	 *	new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)}
	 *	or with GoogleSignInOptions.Builder.requestProfile() configured;
	 *	"" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString PhotoURL;

	/**
	 *	Returns a one-time server auth code to send to your web server which can be
	 *	exchanged for access token and sometimes refresh token if
	 *	GoogleSignInOptions.Builder.requestServerAuthCode(String) is configured;
	 *	"" otherwise.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GoogleSignInAccount")
	FString ServerAuthCode;
};

USTRUCT(BlueprintType)
struct FFirebaseUser
{
	GENERATED_BODY()

	/**
	 *	Returns the main display name of this user from the
	 *	Firebase project's user database.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString DisplayName;

	/**
	 *	Returns the main email address of the user, as stored in
	 *	the Firebase project's user database.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString Email;

	/**
	 *	FirebaseUserMetadata
	 *	Returns the timestamp at which this account was created as dictated by the
	 *	server clock in milliseconds since epoch.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	int64 CreationTimestamp = 0;

	/**
	 *	FirebaseUserMetadata
	 *	Returns the last signin timestamp as dictated by
	 *	the server clock in milliseconds since epoch.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	int64 LastSignInTimestamp = 0;

	/**
	 *	Returns the phone number of the user, as stored in
	 *	the Firebase project's user database, or "" if none exists.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString PhoneNumber;

	/**
	 *	Returns the URL of this user's main profile picture, as stored in
	 *	the Firebase project's user database.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString PhotoURL;

	/** Returns true if the user's email is verified. */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	bool bIsEmailVerified = false;

	/** Always returns FirebaseAuthProvider.PROVIDER_ID */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString ProviderID;

	/**
	 *	Returns a string used to uniquely identify your user in
	 *	your Firebase project's user database.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	FString UID;

	/**
	 *	Returns true if the user is anonymous;
	 *	that is, the user account was created with FirebaseAuth.signInAnonymously()
	 *	and has not been linked to another account with linkWithCredential(AuthCredential).
	 */
	UPROPERTY(BlueprintReadOnly, Category = "FirebaseUser")
	bool bIsAnonymous = false;
};

/**
 * Common status codes
 * https://developers.google.com/android/reference/com/google/android/gms/common/api/CommonStatusCodes
 */
UENUM(BlueprintType)
enum class ECommonStatusCodes : uint8
{
	/**
	 *	The operation was successful, but was used the device's cache.
	 */
	//SUCCESS_CACHE									= -1	UMETA(DisplayName = "SuccessCache"),

	/**
	 *  The operation was successful.
	 */
	SUCCESS											= 0		UMETA(DisplayName = "Success"),
	
	/**
	 * 	The client attempted to connect to the service but
	 * 	the user is not signed in.
	 */
	SIGN_IN_REQUIRED								= 4		UMETA(DisplayName = "SignInRequired"),

	/**
	 *	The client attempted to connect to the service
	 *	with an invalid account name specified.
	 */
	INVALID_ACCOUNT									= 5		UMETA(DisplayName = "InvalidAccount"),

	/**
	 *	Completing the operation requires some form of resolution.
	 */
	RESOLUTION_REQUIRED								= 6		UMETA(DisplayName = "ResolutionRequired"),

	/**
	 *	A network error occurred.
	 */
	NETWORK_ERROR									= 7		UMETA(DisplayName = "NetworkError"),

	/**
	 *	An internal error occurred.
	 */
	INTERNAL_ERROR									= 8		UMETA(DisplayName = "InternalError"),

	/**
	 *	The application is misconfigured.
	 */
	DEVELOPER_ERROR									= 10	UMETA(DisplayName = "DeveloperError"),

	/**
	 *	The operation failed with no more detailed information.
	 */
	ERROR											= 13	UMETA(DisplayName = "Error"),

	/**
	 *	A blocking call was interrupted while waiting and did not run to completion.
	 */
	INTERRUPTED										= 14	UMETA(DisplayName = "Interrupted"),

	/**
	 *	Timed out while awaiting the result.
	 */
	TIMEOUT											= 15	UMETA(DisplayName = "Timeout"),

	/**
	 *	The result was canceled either due to client disconnect
	 *	or PendingResult.cancel().
	 */
	CANCELED										= 16	UMETA(DisplayName = "Canceled"),

	/**
	 *	The client attempted to call a method from an API that failed to connect.
	 */
	API_NOT_CONNECTED								= 17	UMETA(DisplayName = "APINotConnected"),

	/**
	 *	There was a non-DeadObjectException RemoteException while calling a
	 *	connected service.
	 */
	REMOTE_EXCEPTION								= 19	UMETA(DisplayName = "RemoteException"),

	/**
	 *	The connection was suspended while the call was in-flight.
	 */
	CONNECTION_SUSPENDED_DURING_CALL				= 20	UMETA(DisplayName = "ConnectionSuspendedDuringCall"),

	/**
	 *	The connection timed-out while waiting for Google Play services to update.
	 */
	RECONNECTION_TIMED_OUT_DURING_UPDATE			= 21	UMETA(DisplayName = "ReconnectionTimedOutDuringUpdate"),

	/**
	 *	The connection timed-out while attempting to re-connect.
	 */
	RECONNECTION_TIMED_OUT							= 22	UMETA(DisplayName = "ReconnectionTimedOut"),

	// Additional firebase exception codes
	
	/** https://firebase.google.com/docs/reference/android/com/google/firebase/FirebaseTooManyRequestsException
	 *	Exception thrown when a request to a Firebase service has been blocked
	 *	due to having received too many consecutive requests from the same device.
	 *	Retry the request later to resolve.
	 */
	TOO_MANY_REQUESTS_EXCEPTION						= 100	UMETA(DisplayName = "TooManyRequestsException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/FirebaseNetworkException
	 *	Exception thrown when a request to a Firebase service has failed due
	 *	to a network error. Inspect the device's network connectivity
	 *	state or retry later to resolve.
	 */
	NETWORK_EXCEPTION								= 101	UMETA(DisplayName = "NetworkException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/FirebaseApiNotAvailableException
	 *	Indicates that a requested API is not available.
	 */
	API_NOT_AVAILABLE_EXCEPTION						= 102	UMETA(DisplayName = "APINotAvailableException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthWeakPasswordException
	 *	Thrown when using a weak password (less than 6 chars) to create a
	 *	new account or to update an existing account's password.
	 *	Use getReason() to get a message with the reason the validation failed
	 *	that you can display to your users.
	 */
	AUTH_WEAK_PASSWORD_EXCEPTION					= 103	UMETA(DisplayName = "WeakPasswordException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthActionCodeException
	 *	Represents the exception which is a result of an expired or an
	 *	invalid out of band code.
	 */
	AUTH_ACTION_CODE_EXCEPTION						= 104	UMETA(DisplayName = "ActionCodeException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthEmailException
	 *	Represents the exception which is a result of an attempt to send an email via
	 *	Firebase Auth (e.g. a password reset email)
	 */
	AUTH_EMAIL_EXCEPTION							= 105	UMETA(DisplayName = "EmailException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthInvalidCredentialsException
	 *	Thrown when one or more of the credentials passed to a method fail to identify
	 *	and/or authenticate the user subject of that operation. Inspect the
	 *	error code and message to find out the specific cause.
	 */
	AUTH_INVALID_CREDENTIALS_EXCEPTION				= 106	UMETA(DisplayName = "InvalidCredentialsException"),	

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthRecentLoginRequiredException
	 *	Thrown on security sensitive operations on a FirebaseUser instance that
	 *	require the user to have signed in recently,
	 *	when the requirement isn't met.
	 */
	AUTH_RECENT_LOGIN_REQUIRED_EXCEPTION			= 107	UMETA(DisplayName = "RecentLoginRequiredException"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthInvalidUserException
	 *	User has been disabled (for example, in the Firebase console).
	 */
	ERROR_USER_DISABLED								= 108	UMETA(DisplayName = "UserDisabled"),

	/**
	 *	User has been deleted (for example, in the Firebase console,
	 *	or in another instance of this app)
	 */
	ERROR_USER_NOT_FOUND							= 109	UMETA(DisplayName = "UserNotFound"),

	/**
	 *	User's token has been revoked in the backend.
	 *	This happens automatically if the user's credentials change
	 *	in another device (for example, on a password change event).
	 */
	ERROR_USER_TOKEN_EXPIRED						= 110	UMETA(DisplayName = "UserTokenExpired"),

	/**
	 *	User's token is malformed. This should not happen
	 *	under normal circumstances.
	 */
	ERROR_INVALID_USER_TOKEN						= 111	UMETA(DisplayName = "InvalidUserToken"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthUserCollisionException
	 *	When trying to create a new account with
	 *	FirebaseAuth.createUserWithEmailAndPassword(String, String)
	 *	or to change a user's email address,
	 *	if the email is already in use by a different account
	 */
	ERROR_EMAIL_ALREADY_IN_USE						= 112	UMETA(DisplayName = "EmailAlreadyInUse"),

	/**
	 * When calling FirebaseAuth.signInWithCredential(AuthCredential) with
	 * a credential that asserts an email address in use by another account.
	 * This error will only be thrown if the
	 * "One account per email address" setting is enabled
	 * in the Firebase console (recommended).
	 */
	ERROR_ACCOUNT_EXISTS_WITH_DIFFERENT_CREDENTIAL	= 113	UMETA(DisplayName = "AccountExistsWithDifferentCredential"),

	/**
	 * When trying to link a user with an AuthCredential corresponding to
	 * another account already in use.
	 */
	ERROR_CREDENTIAL_ALREADY_IN_USE					= 114	UMETA(DisplayName = "CredentialAlreadyInUse"),

	/** https://firebase.google.com/docs/reference/android/com/google/firebase/auth/FirebaseAuthWebException
	 *	Thrown when another web operation is still in progress.
	 */
	ERROR_WEB_CONTEXT_ALREADY_PRESENTED				= 115	UMETA(DisplayName = "WebContextAlreadyPresented"),

	/**
	 *	Thrown when the pending operation was canceled by the user.
	 */
	ERROR_WEB_CONTEXT_CANCELED						= 116	UMETA(DisplayName = "WebContextCanceled"),

	/**
	 *	Thrown when the browser is not supported, or when
	 *	3rd party cookies or data are disabled in the browser.
	 */
	ERROR_WEB_STORAGE_UNSUPPORTED					= 117	UMETA(DisplayName = "WebStorageUnsupported"),

	/**
	 *	When there was a problem that occurred inside of
	 *	the web widget that hosts the operation.
	 *	Details should always accompany this message.
	 */
	ERROR_WEB_INTERNAL_ERROR						= 118	UMETA(DisplayName = "WebInternalError"),


	/**
	 *	An Exception indicating that Login failed.
	 */
	FACEBOOK_AUTHORIZATION_EXCEPTION 				= 119	UMETA(DisplayName = "FacebookAuthorizationException"),

	/**
	 *	Represents an error condition relating to displaying a
	 *	Facebook Web dialog.
	 */
	FACEBOOK_DIALOG_EXCEPTION						= 120	UMETA(DisplayName = "FacebookDialogException"),

	/**
	 *	Represents an issue that's returned by the Graph API.
	 */
	FACEBOOK_GRAPH_RESPONSE_EXCEPTION				= 121	UMETA(DisplayName = "FacebookGraphResponseException"),

	/**
	 *	An Exception indicating that an operation was canceled
	 *	before it completed.
	 */
	FACEBOOK_OPERATION_CANCELED_EXCEPTION			= 122	UMETA(DisplayName = "FacebookOperationCanceledException"),

	/**
	 *	An Exception indicating that the Facebook SDK has not been
	 *	correctly initialized.
	 */
	FACEBOOK_SDK_NOT_INITIALIZED_EXCEPTION			= 123	UMETA(DisplayName = "FacebookSDKNotInitiailizedException"),

	/**
	 *	Represents an error returned from the Facebook service in
	 *	response to a request.
	 */
	FACEBOOK_SERVICE_EXCEPTION						= 124	UMETA(DisplayName = "FacebookServiceException"),
};

UENUM(BlueprintType)
enum class EOAuthProvider : uint8
{
	Apple,
	Github,
	Microsoft,
	Yahoo,
	Twitter	
};

/** Generic firebase delegates */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFirebaseFunctionCompleted, const ECommonStatusCodes, StatusCode);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnFirebaseAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FFirebaseUser&, FirebaseUser);

/** Phone authentication delegates */
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnPhoneAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FFirebaseUser&, FirebaseUser, const FString&, CodeFromSMS);

/** Google authentication delegates */
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnGoogleAuthenticationCompleted, const ECommonStatusCodes, StatusCode, const FGoogleSignInAccount&, GoogleSignInAccount, const FFirebaseUser&, FirebaseUser);

UCLASS()
class UFirebaseAuthenticationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 *	Returns the currently signed-in FFirebaseUser or {} if there is none.
	 */
	UFUNCTION(BlueprintPure, Category = "Firebase")
	static FFirebaseUser GetFirebaseUser();
	
	/**
	 *	Manually refreshes the data of the current user (for example, attached
	 *	providers, display name, and so on).
	 *	
	 *	@param OnOperationResult callback which returns updated user info.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase")
	static void ReloadFirebaseUser(
		FOnFirebaseAuthenticationCompleted OnOperationResult);
	
	/**
	 *	Signs in the user anonymously without requiring any credential.
	 *	
	 *	@param OnOperationResult callback when authentication operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | AnonymousAuthentication")
	static void AnonymousSignIn(
		FOnFirebaseAuthenticationCompleted OnOperationResult);
	
	/**
	 *	Signs out the current user and clears it from the disk cache.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | AnonymousAuthentication")
	static void AnonymousSignOut();
	
	/**
	 *	Attaches the Email/Password credentials to the current user.
	 *	
	 *	@param OnOperationResult callback when operation has been completed.
	 *	@param Email Attaches the given Email to the user. This allows the user to
	 *				 sign in to this account in the future with
	 *				 Email/Password credentials.
	 *	@param Password
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | AnonymousAuthentication")
	static void LinkAnonymousAccountWithEmail(
		FOnFirebaseFunctionCompleted OnOperationResult,
		const FString& Email, 
		const FString& Password);
	
	/**
	 *	Tries to sign in a user with the given email address and password.
	 *
	 *	@param OnOperationResult callback when authentication operation has been completed.
	 *	@param Email
	 *	@param Password
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void EmailSignIn(
		FOnFirebaseAuthenticationCompleted OnOperationResult,
		const FString& Email, 
		const FString& Password);
	
	/**
	 *	Tries to create a new user account with the given email address and password.
	 *	If successful, it also signs the user in into the app.
	 *
	 *	@param OnOperationResult callback when operation has been completed.
	 *	@param Email
	 *	@param Password
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void CreateAccountWithEmail(
		FOnFirebaseFunctionCompleted OnOperationResult,
		const FString& Email, 
		const FString& Password);

	/**
	 *	Initiates email verification for the user. 
	 *	
	 *	@param OnOperationResult callback when operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void SendEmailVerification(
		FOnFirebaseFunctionCompleted OnOperationResult);
	
	/**
	 *	Triggers the Firebase Authentication backend to send a password-reset
	 *	email to the given email address, which must correspond to
	 *	an existing user of your app.
	 *
	 *	@param OnOperationResult callback when operation has been completed.
	 *	@param Email
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void SendPasswordResetLinkOnEmail(
		FOnFirebaseFunctionCompleted OnOperationResult,
		const FString& Email);
	
	/**
	 *	Updates the password of the user.
	 *	Important: this is a security sensitive operation that requires the user to have recently signed in.
	 *
	 *	@param OnOperationResult callback when operation has been completed.
	 *	@param NewPassword
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void UpdatePassword(
		FOnFirebaseFunctionCompleted OnOperationResult,
		const FString& NewPassword);
	
	/**
	 *	Signs out the current user and clears it from the disk cache.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | EmailAuthentication")
	static void EmailSignOut();
	
	/**
	 *	Prompts the user to authorize your application using the Login Dialog.
	 *
	 *	@param OnOperationResult callback when authentication operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | FacebookAuthentication")
	static void FacebookSignIn(
		FOnFirebaseAuthenticationCompleted OnOperationResult);
	
	/**
	 *	Signs out the current user and clears it from the disk cache.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | FacebookAuthentication")
	static void FacebookSignOut();

	/**
	 *	@param OnOperationResult callback when authentication operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | GoogleAuthentication")
	static void GoogleSignIn(
		FOnGoogleAuthenticationCompleted OnOperationResult);
	
	/**
	 *	Signs out the current user and clears it from the disk cache.
	 *	
	 *	@param OnOperationResult callback when operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | GoogleAuthentication")
	static void GoogleSignOut(
		FOnFirebaseFunctionCompleted OnOperationResult);
	
	/**
	 *	Revokes access given to the current application.
	 *
	 *	@param OnOperationResult callback when operation has been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | GoogleAuthentication")
	static void GoogleRevokeAccess(
		FOnFirebaseFunctionCompleted OnOperationResult);
	
	/**
	 *	Signs in the user using the mobile browser (either a Custom Chrome Tab
	 *	or the device's default browser) for the given provider.
	 *
	 *	@param OnOperationResult callback when authentication operation has been completed.
	 *	@param OAuthProvider representation of an arbitrary federated authentication provider.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | OAuthAuthentication")
	static void OAuthSignIn(
		FOnFirebaseAuthenticationCompleted OnOperationResult,
		const EOAuthProvider OAuthProvider);

	/**
	 *	Signs out the current user and clears it from the disk cache.
	 */
    UFUNCTION(BlueprintCallable, Category = "Firebase | OAuthAuthentication")
	static void OAuthSignOut();

	/**
	 *	Starts the phone verification process.
	 *
	 *	@param OnAuthenticationResult callback when authentication operation has been completed.
	 *	@param OnCodeSentResult callback when code has been sent.
	 *	@param PhoneNumber the phone number for the account the user is signing up for or signing into.
	 *					   Make sure to pass in a phone number with country code prefixed
	 *					   with plus sign ('+').
	 *	@param Timeout the maximum amount of time you are willing to wait for SMS auto-retrieval to
	 *				   be completed by the library. Maximum allowed value is 2 minutes.
	 */
    UFUNCTION(BlueprintCallable, Category = "Firebase | PhoneAuthentication")
	static void SignInWithSMSCode(
		FOnPhoneAuthenticationCompleted OnAuthenticationResult,
		FOnFirebaseFunctionCompleted OnCodeSentResult,
		const FString& PhoneNumber, 
		const int Timeout);
	
	/**
	 *	Signs out the current user and clears it from the disk cache.
	 */
	UFUNCTION(BlueprintCallable, Category = "Firebase | PhoneAuthentication")
	static void PhoneSignOut();

private:
	static void FirebaseSignOut();
};
