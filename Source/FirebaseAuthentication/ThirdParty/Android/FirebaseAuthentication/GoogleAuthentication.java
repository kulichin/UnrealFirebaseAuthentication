// Copyright (C) 2021. Nikita Klimov. All rights reserved.

package com.kulichin.firebaseauthentication;

import android.app.Activity;
import android.content.Intent;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.GoogleAuthProvider;

import com.kulichin.firebaseauthentication.BaseAuthentication.GoogleSignInAccountWrapper;
import com.kulichin.firebaseauthentication.BaseAuthentication.FirebaseUserWrapper;

public class GoogleAuthentication
{
    private static native void NativeGoogleAuthenticationResult(
        GoogleSignInAccountWrapper GoogleSignInAccount,
        FirebaseUserWrapper FirebaseUser,
        int GoogleStatusResult);

    private final GoogleSignInClient SignInClient;
    private final FirebaseAuth FirebaseAuthInstance;
    private final Activity MainActivity;

    private static final int GOOGLE_SIGNIN_RC = 9001;

    public GoogleAuthentication(Activity MainActivity)
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
        this.MainActivity = MainActivity;

        // Get Web Client ID
        int ResourceID = this.MainActivity.getResources().getIdentifier(
            "default_web_client_id", 
            "string", 
            this.MainActivity.getPackageName());
        String WebClientID = this.MainActivity.getString(ResourceID);

        // Initialize Google Sign In
		GoogleSignInOptions SignInOptions = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
            .requestIdToken(WebClientID)
            .requestServerAuthCode(WebClientID)
			.requestEmail()
            .requestProfile()
		    .build();
		this.SignInClient = GoogleSignIn.getClient(this.MainActivity, SignInOptions);
    }

    public void GoogleSignIn()
    {
        Intent SignInIntent = SignInClient.getSignInIntent();
        MainActivity.startActivityForResult(SignInIntent, GOOGLE_SIGNIN_RC);
    }

    public void GoogleSignOut()
    {
        FirebaseAuthInstance.signOut();
        SignInClient.signOut()
            .addOnCompleteListener(Task -> BaseAuthentication.FirebaseFunctionSuccessfully())
            .addOnFailureListener(BaseAuthentication::FirebaseFunctionException);
    }

    public void GoogleRevokeAccess()
    {
        FirebaseAuthInstance.signOut();
        SignInClient.revokeAccess()
            .addOnCompleteListener(Task -> BaseAuthentication.FirebaseFunctionSuccessfully())
            .addOnFailureListener(BaseAuthentication::FirebaseFunctionException);
    }

	public void ActivityResult(int RequestCode, int ResultCode, Intent Data)
	{
		if (RequestCode == GOOGLE_SIGNIN_RC)
		{
			Task<GoogleSignInAccount> Task = GoogleSignIn.getSignedInAccountFromIntent(Data);
			try
			{
				GoogleSignInAccount SignInAccount = Task.getResult(ApiException.class);
                AuthCredential Credential = GoogleAuthProvider.getCredential(SignInAccount.getIdToken(), null);

                FirebaseAuthInstance.signInWithCredential(Credential).addOnCompleteListener(SignInTask ->
                {
                    if (SignInTask.isSuccessful())
                    {
                        final GoogleSignInAccountWrapper GoogleAccountWrapper =
                            BaseAuthentication.GetWrappedGoogleSignInAccount(SignInAccount);
                        final FirebaseUserWrapper FirebaseAccountWrapper =
                            BaseAuthentication.GetWrappedFirebaseUser(FirebaseAuthInstance.getCurrentUser());
                        
                        NativeGoogleAuthenticationResult(
                            GoogleAccountWrapper,
                            FirebaseAccountWrapper,
                            CommonStatusCodes.SUCCESS);
                    }
                    else
                    {
                        NativeGoogleAuthenticationResult(null, null, CommonStatusCodes.ERROR);
                    }
                });
			}
			catch (ApiException e)
			{
				NativeGoogleAuthenticationResult(null, null, e.getStatusCode());
			}
		}
	}
}
