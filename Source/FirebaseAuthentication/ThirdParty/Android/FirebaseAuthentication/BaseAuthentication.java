// Copyright (C) 2021. Nikita Klimov. All rights reserved.

package com.kulichin.firebaseauthentication;

import android.net.Uri;

import com.facebook.FacebookAuthorizationException;
import com.facebook.FacebookDialogException;
import com.facebook.FacebookGraphResponseException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.FacebookSdkNotInitializedException;
import com.facebook.FacebookServiceException;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.common.api.Scope;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.firebase.FirebaseApiNotAvailableException;
import com.google.firebase.FirebaseNetworkException;
import com.google.firebase.FirebaseTooManyRequestsException;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.FirebaseUserMetadata;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthException;
import com.google.firebase.auth.FirebaseAuthActionCodeException;
import com.google.firebase.auth.FirebaseAuthEmailException;
import com.google.firebase.auth.FirebaseAuthInvalidCredentialsException;
import com.google.firebase.auth.FirebaseAuthInvalidUserException;
import com.google.firebase.auth.FirebaseAuthRecentLoginRequiredException;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseAuthWeakPasswordException;
import com.google.firebase.auth.FirebaseAuthWebException;
import com.google.firebase.auth.UserInfo;

import java.util.Set;

public class BaseAuthentication
{
    private static native void NativeInitialize(
        Class GoogleSignInAccountWrapperClass,
        Class FirebaseUserWrapperClass);

    private static native void NativeFirebaseAuthenticationResult(
        FirebaseUserWrapper FirebaseUser,
        int StatusCode);

    private static native void NativePhoneAuthenticationResult(
        String CodeFromSMS,
        FirebaseUserWrapper FirebaseUser,
        int StatusCode);

    private static native void NativeFirebaseFunctionResult(int StatusCode);

    private static class AdditionalStatusCodes
    {
	    static final int TOO_MANY_REQUESTS_EXCEPTION					= 100;
	    static final int NETWORK_EXCEPTION								= 101;
	    static final int API_NOT_AVAILABLE_EXCEPTION					= 102;
	    static final int AUTH_WEAK_PASSWORD_EXCEPTION					= 103;
	    static final int AUTH_ACTION_CODE_EXCEPTION						= 104;
	    static final int AUTH_EMAIL_EXCEPTION							= 105;
	    static final int AUTH_INVALID_CREDENTIALS_EXCEPTION				= 106;
	    static final int AUTH_RECENT_LOGIN_REQUIRED_EXCEPTION			= 107;
	    
        static final int ERROR_USER_DISABLED							= 108;
	    static final int ERROR_USER_NOT_FOUND							= 109;
	    static final int ERROR_USER_TOKEN_EXPIRED						= 110;
	    static final int ERROR_INVALID_USER_TOKEN						= 111;
	    
        static final int ERROR_EMAIL_ALREADY_IN_USE						= 112;
	    static final int ERROR_ACCOUNT_EXISTS_WITH_DIFFERENT_CREDENTIAL	= 113;
	    static final int ERROR_CREDENTIAL_ALREADY_IN_USE				= 114;
	    
        static final int ERROR_WEB_CONTEXT_ALREADY_PRESENTED			= 115;
	    static final int ERROR_WEB_CONTEXT_CANCELED						= 116;
	    static final int ERROR_WEB_STORAGE_UNSUPPORTED					= 117;
	    static final int ERROR_WEB_INTERNAL_ERROR						= 118;

	    static final int FACEBOOK_AUTHORIZATION_EXCEPTION 				= 119;
	    static final int FACEBOOK_DIALOG_EXCEPTION						= 120;
	    static final int FACEBOOK_GRAPH_RESPONSE_EXCEPTION				= 121;
	    static final int FACEBOOK_OPERATION_CANCELED_EXCEPTION			= 122;
	    static final int FACEBOOK_SDK_NOT_INITIALIZED_EXCEPTION			= 123;
	    static final int FACEBOOK_SERVICE_EXCEPTION						= 124;
    }

    static public class GoogleSignInAccountWrapper
    {
        public String DisplayName;
        public String Email;
        public String FamilyName;
        public String GivenName;
        public String[] GrantedScopes;
        public String ID;
        public String IDToken;
        public String PhotoURL;
        public String ServerAuthCode;

        GoogleSignInAccountWrapper(
            String ToDisplayName,
            String ToEmail,
            String ToFamilyName,
            String ToGivenName,
            String[] ToGrantedScopes,
            String ToID,
            String ToIDToken,
            String ToPhotoURL,
            String ToServerAuthCode)
        {
            this.DisplayName = ToDisplayName;
            this.Email = ToEmail;
            this.FamilyName = ToFamilyName;
            this.GivenName = ToGivenName;
            this.GrantedScopes = ToGrantedScopes;
            this.ID = ToID;
            this.IDToken = ToIDToken;
            this.PhotoURL = ToPhotoURL;
            this.ServerAuthCode = ToServerAuthCode;
        }
    }

    static public class FirebaseUserWrapper
    {
        String DisplayName;
        String Email;
        long CreationTimestamp;
        long LastSignInTimestamp;
        String PhoneNumber;
        String PhotoURL;
        boolean bIsEmailVerified;
        String ProviderID;
        String UID;
        boolean bIsAnonymous;

        FirebaseUserWrapper(
            String ToDisplayName,
            String ToEmail,
            long ToCreationTimestamp,
            long ToLastSignInTimestamp,
            String ToPhoneNumber,
            String ToPhotoURL,
            boolean ToIsEmailVerified,
            String ToProviderID,
            String ToUID,
            boolean ToIsAnonymous)
        {
            this.DisplayName = ToDisplayName;
            this.Email = ToEmail;
            this.CreationTimestamp = ToCreationTimestamp;
            this.LastSignInTimestamp = ToLastSignInTimestamp;
            this.PhoneNumber = ToPhoneNumber;
            this.PhotoURL = ToPhotoURL;
            this.bIsEmailVerified = ToIsEmailVerified;
            this.ProviderID = ToProviderID;
            this.UID = ToUID;
            this.bIsAnonymous = ToIsAnonymous;
        }
    }

    public BaseAuthentication()
    {
        // Update firebase user info
        ReloadFirebaseUser();

        NativeInitialize(
            GoogleSignInAccountWrapper.class, 
            FirebaseUserWrapper.class);
    }

    public static GoogleSignInAccountWrapper GetWrappedGoogleSignInAccount(GoogleSignInAccount SignInAccount)
    {
        // Wrap GoogleSignInAccount data to GoogleSignInAccountWrapper
        GoogleSignInAccountWrapper GoogleAccountWrapper = null;
        if (SignInAccount != null)
        {
            // Update PhotoURL if getPhotoUrl() valid
            String PhotoURLString = "";
            Uri PhotoURL = SignInAccount.getPhotoUrl();
            if (PhotoURL != null)
            {
                PhotoURLString = PhotoURL.toString();
            }

            // Update OutGrantedScopes
            Set<Scope> GrantedScopes = SignInAccount.getGrantedScopes();
            int GrantedScopesSize = GrantedScopes.size();

            Scope[] RawGrantedScopes = new Scope[GrantedScopesSize];
            GrantedScopes.toArray(RawGrantedScopes);

            String[] OutGrantedScopes = new String[GrantedScopesSize];
            for (int Idx = 0; Idx < GrantedScopesSize; Idx++)
            {
                OutGrantedScopes[Idx] = RawGrantedScopes[Idx].getScopeUri();
            }

            GoogleAccountWrapper = new GoogleSignInAccountWrapper(
                SignInAccount.getDisplayName(),
                SignInAccount.getEmail(),
                SignInAccount.getFamilyName(),
                SignInAccount.getGivenName(),
                OutGrantedScopes,
                SignInAccount.getId(),
                SignInAccount.getIdToken(),
                PhotoURLString,
                SignInAccount.getServerAuthCode());
        }

        return GoogleAccountWrapper;
    }

    public static FirebaseUserWrapper GetWrappedFirebaseUser(FirebaseUser User)
    {
        // Wrap FirebaseUser data to FirebaseUserWrapper
        FirebaseUserWrapper FirebaseAccountWrapper = null;
        if (User != null)
        {
            // Update CreationTimestamp & LastSignInTimestamp if getMetadata valid
            long CreationTimestamp = 0;
            long LastSignInTimestamp = 0;
            final FirebaseUserMetadata UserMetadata = User.getMetadata();
            if (UserMetadata != null)
            {
                CreationTimestamp = UserMetadata.getCreationTimestamp();
                LastSignInTimestamp = UserMetadata.getLastSignInTimestamp();
            }

            // Update PhotoURL if getPhotoUrl() valid
            String PhotoURLString = "";
            Uri PhotoURL = User.getPhotoUrl();
            if (PhotoURL != null)
            {
                PhotoURLString = PhotoURL.toString();
            }

            // Update IsEmailVerified if getProviderData() valid
            boolean bIsEmailVerified = false;
            int ProviderDataSize = User.getProviderData().size();
            if (ProviderDataSize > 0)
            {
                UserInfo ProviderData = User.getProviderData().get(0);
                if (ProviderData != null)
                {
                    bIsEmailVerified = ProviderData.isEmailVerified();
                }
            }

            // Wrap data
            FirebaseAccountWrapper = new FirebaseUserWrapper(
                User.getDisplayName(),
                User.getEmail(),
                CreationTimestamp,
                LastSignInTimestamp,
                User.getPhoneNumber(),
                PhotoURLString,
                bIsEmailVerified,
                User.getProviderId(),
                User.getUid(),
                User.isAnonymous());
        }

        return FirebaseAccountWrapper;
    }

    /**
     *  Function results
     */
    public static void FirebaseFunctionSuccessfully()
    {
        NativeFirebaseFunctionResult(CommonStatusCodes.SUCCESS);
    }

    public static void FirebaseFunctionException(Exception e)
    {
        NativeFirebaseFunctionResult(GetFirebaseStatusCode(e));
    }

    /*
     *  Phone authentication results
     */
    public static void PhoneAuthenticationSuccessfully(String CodeFromSMS)
    {
        final FirebaseUserWrapper FirebaseAccountWrapper = 
            BaseAuthentication.GetWrappedFirebaseUser(FirebaseAuth.getInstance().getCurrentUser());

        NativePhoneAuthenticationResult(
            CodeFromSMS, 
            FirebaseAccountWrapper, 
            CommonStatusCodes.SUCCESS);
    }

    public static void PhoneAuthenticationException(Exception e)
    {
        NativePhoneAuthenticationResult(null, null, GetFirebaseStatusCode(e));
    }

    /*
     *  Firebase authentication results
     */
    public static void FirebaseAuthenticationSuccessfully()
    {
        final FirebaseUserWrapper FirebaseAccountWrapper = 
            BaseAuthentication.GetWrappedFirebaseUser(FirebaseAuth.getInstance().getCurrentUser());
        NativeFirebaseAuthenticationResult(FirebaseAccountWrapper, CommonStatusCodes.SUCCESS);
    }

    public static void FirebaseAuthenticationException(Exception e)
    {
        NativeFirebaseAuthenticationResult(null, GetFirebaseStatusCode(e));
    }

    public static int GetFirebaseStatusCode(Exception e)
    {
        if (e instanceof FacebookAuthorizationException)
        {
            return AdditionalStatusCodes.FACEBOOK_AUTHORIZATION_EXCEPTION;
        }
        else if (e instanceof FacebookDialogException)
        {
            return AdditionalStatusCodes.FACEBOOK_DIALOG_EXCEPTION;
        }
        else if (e instanceof FacebookGraphResponseException)
        {
            return AdditionalStatusCodes.FACEBOOK_GRAPH_RESPONSE_EXCEPTION;
        }
        else if (e instanceof FacebookOperationCanceledException)
        {
            return AdditionalStatusCodes.FACEBOOK_OPERATION_CANCELED_EXCEPTION;
        }
        else if (e instanceof FacebookSdkNotInitializedException)
        {
            return AdditionalStatusCodes.FACEBOOK_SDK_NOT_INITIALIZED_EXCEPTION;
        }
        else if (e instanceof FacebookServiceException)
        {
            return AdditionalStatusCodes.FACEBOOK_SERVICE_EXCEPTION;
        }
        else if (e instanceof FirebaseTooManyRequestsException)
        {
            return AdditionalStatusCodes.TOO_MANY_REQUESTS_EXCEPTION;
        }
        else if (e instanceof FirebaseNetworkException)
        {
            return AdditionalStatusCodes.NETWORK_EXCEPTION;
        }
        else if (e instanceof FirebaseApiNotAvailableException)
        {
            return AdditionalStatusCodes.API_NOT_AVAILABLE_EXCEPTION;
        }
        else if (e instanceof FirebaseAuthWeakPasswordException)
        {
            return AdditionalStatusCodes.AUTH_WEAK_PASSWORD_EXCEPTION;
        }
        else if (e instanceof FirebaseAuthActionCodeException)
        {
            return AdditionalStatusCodes.AUTH_ACTION_CODE_EXCEPTION;
        }
        else if (e instanceof FirebaseAuthEmailException)
        {
            return AdditionalStatusCodes.AUTH_EMAIL_EXCEPTION;
        }
        else if (e instanceof FirebaseAuthInvalidCredentialsException)
        {
            return AdditionalStatusCodes.AUTH_INVALID_CREDENTIALS_EXCEPTION;
        }
        else if (e instanceof FirebaseAuthRecentLoginRequiredException)
        {
            return AdditionalStatusCodes.AUTH_RECENT_LOGIN_REQUIRED_EXCEPTION;
        }
        else if (e instanceof ApiException)
        {
            return ((ApiException)e).getStatusCode();
        }
        else if (
            e instanceof FirebaseAuthInvalidUserException || 
            e instanceof FirebaseAuthUserCollisionException || 
            e instanceof FirebaseAuthWebException)
        {
            switch (((FirebaseAuthException) e).getErrorCode())
            {
                // FirebaseAuthInvalidUserException
                case "ERROR_USER_DISABLED":
                {
                    return AdditionalStatusCodes.ERROR_USER_DISABLED;
                }
                case "ERROR_USER_NOT_FOUND":
                {
                    return AdditionalStatusCodes.ERROR_USER_NOT_FOUND;
                }
                case "ERROR_USER_TOKEN_EXPIRED":
                {
                    return AdditionalStatusCodes.ERROR_USER_TOKEN_EXPIRED;
                }
                case "ERROR_INVALID_USER_TOKEN": 
                {
                    return AdditionalStatusCodes.ERROR_INVALID_USER_TOKEN;
                }

                // FirebaseAuthUserCollisionException
                case "ERROR_EMAIL_ALREADY_IN_USE":
                {
                    return AdditionalStatusCodes.ERROR_EMAIL_ALREADY_IN_USE;
                }
                case "ERROR_ACCOUNT_EXISTS_WITH_DIFFERENT_CREDENTIAL":
                {
                    return AdditionalStatusCodes.ERROR_ACCOUNT_EXISTS_WITH_DIFFERENT_CREDENTIAL;
                }
                case "ERROR_CREDENTIAL_ALREADY_IN_USE":
                {
                    return AdditionalStatusCodes.ERROR_CREDENTIAL_ALREADY_IN_USE;
                }

                // FirebaseAuthWebException
                case "ERROR_WEB_CONTEXT_ALREADY_PRESENTED":
                {
                    return AdditionalStatusCodes.ERROR_WEB_CONTEXT_ALREADY_PRESENTED;
                }
                case "ERROR_WEB_CONTEXT_CANCELED":
                {
                    return AdditionalStatusCodes.ERROR_WEB_CONTEXT_CANCELED;
                }
                case "ERROR_WEB_STORAGE_UNSUPPORTED":
                {
                    return AdditionalStatusCodes.ERROR_WEB_STORAGE_UNSUPPORTED;
                }
                case "ERROR_WEB_INTERNAL_ERROR":
                {
                    return AdditionalStatusCodes.ERROR_WEB_INTERNAL_ERROR;
                }
            }
        }

        return CommonStatusCodes.ERROR;
    }

    public static FirebaseUserWrapper GetFirebaseUser()
    {
        return GetWrappedFirebaseUser(FirebaseAuth.getInstance().getCurrentUser());
    }

    public static void ReloadFirebaseUser()
    {
        FirebaseUser User = FirebaseAuth.getInstance().getCurrentUser();
        if (User != null)
        {
            User.reload()
                .addOnCompleteListener(Task -> FirebaseAuthenticationSuccessfully())
                .addOnFailureListener(BaseAuthentication::FirebaseAuthenticationException);
        }
    }

    public static void FirebaseAccountSignOut()
    {
        FirebaseAuth.getInstance().signOut();
    }
}
