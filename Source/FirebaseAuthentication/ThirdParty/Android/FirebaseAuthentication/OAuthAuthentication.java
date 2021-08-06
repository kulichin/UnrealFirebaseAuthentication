/**
 *	Copyright (C) 2021. Nikita Klimov. All rights reserved.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the Apache License Version 2.0.
 * 
 *	See file LICENSE.md for further informations on licensing terms.
 */

package com.kulichin.firebaseauthentication;

import android.app.Activity;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.OAuthProvider;

public class OAuthAuthentication
{
    private final FirebaseAuth FirebaseAuthInstance;
    private final Activity MainActivity;

    public OAuthAuthentication(Activity MainActivity)
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
        this.MainActivity = MainActivity;
    }

    public void OAuthSignIn(String ProviderID)
    {
        FirebaseAuthInstance.startActivityForSignInWithProvider(
            MainActivity,
            OAuthProvider.newBuilder(ProviderID, FirebaseAuthInstance).build())
                .addOnSuccessListener(Task -> BaseAuthentication.FirebaseAuthenticationSuccessfully())
                .addOnFailureListener(BaseAuthentication::FirebaseAuthenticationException);
    }
}
