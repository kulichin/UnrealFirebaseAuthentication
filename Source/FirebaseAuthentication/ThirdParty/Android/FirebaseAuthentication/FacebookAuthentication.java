// Copyright (C) 2021. Nikita Klimov. All rights reserved.

package com.kulichin.firebaseauthentication;

import android.app.Activity;
import android.content.Intent;

import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.FacebookAuthProvider;
import com.google.firebase.auth.FirebaseAuth;

import java.util.Arrays;

public class FacebookAuthentication
{
    private static final int FACEBOOK_SIGNIN_RC = 64206;

    private final CallbackManager FacebookCallbackManager;
    private final FacebookCallback<LoginResult> LoginCallback;
    private final FirebaseAuth FirebaseAuthInstance;
    private final Activity MainActivity;

    public FacebookAuthentication(Activity MainActivity)
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
		this.MainActivity = MainActivity;

		// Initialize facebook callbacks
		this.FacebookCallbackManager = CallbackManager.Factory.create();
		this.LoginCallback = new FacebookCallback<LoginResult>()
		{
			@Override
			public void onSuccess(LoginResult Result)
			{
                AuthCredential Credential = FacebookAuthProvider.getCredential(Result.getAccessToken().getToken());
                FirebaseAuthInstance.signInWithCredential(Credential).addOnCompleteListener(Task ->
                {
                    if (Task.isSuccessful())
                    {
                        BaseAuthentication.FirebaseAuthenticationSuccessfully();
                    }
                    else
                    {
                        BaseAuthentication.FirebaseAuthenticationException(Task.getException());
                    }
                });
			}
		
			@Override
			public void onCancel()
			{
                //BaseAuthentication.FirebaseAuthenticationException(0);
			}
		
			@Override
			public void onError(FacebookException e)
			{
                BaseAuthentication.FirebaseAuthenticationException(e);
			}
		};
    }

    public void FacebookSignIn()
    {
        String[] ScopeFields = { "email" };
        LoginManager Manager = LoginManager.getInstance();
        Manager.registerCallback(FacebookCallbackManager, LoginCallback);
        Manager.logInWithReadPermissions(MainActivity, Arrays.asList(ScopeFields));
    }

    public void FacebookSignOut()
    {
        FirebaseAuthInstance.signOut();
        LoginManager.getInstance().logOut();
    }

	public void ActivityResult(int RequestCode, int ResultCode, Intent Data)
	{
		if (RequestCode == FACEBOOK_SIGNIN_RC)
        {
			FacebookCallbackManager.onActivityResult(RequestCode, ResultCode, Data);
        }
	}
}
