/**
 *	Copyright (C) 2021. Nikita Klimov. All rights reserved.
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the Apache License Version 2.0.
 * 
 *	See file LICENSE.md for further informations on licensing terms.
 */

package com.kulichin.firebaseauthentication;

import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.EmailAuthProvider;
import com.google.firebase.auth.FirebaseAuth;

public class AnonymousAuthentication
{
    private final FirebaseAuth FirebaseAuthInstance;

    public AnonymousAuthentication()
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
    }

    public void AnonymousSignIn()
    {
        FirebaseAuthInstance.signInAnonymously().addOnCompleteListener(Task ->
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

    public void LinkAnonymousAccountWithEmail(String Email, String Password)
    {
        AuthCredential Credential = EmailAuthProvider.getCredential(Email, Password);
        try
        {
            FirebaseAuthInstance.getCurrentUser().linkWithCredential(Credential)
                .addOnSuccessListener(Task -> BaseAuthentication.FirebaseFunctionSuccessfully())
                .addOnFailureListener(BaseAuthentication::FirebaseFunctionException);
        }
        catch (Exception e)
        {
            BaseAuthentication.FirebaseFunctionException(e);
        }
    }
}