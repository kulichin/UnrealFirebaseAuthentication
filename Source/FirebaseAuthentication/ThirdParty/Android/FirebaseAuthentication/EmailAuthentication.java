// Copyright (C) 2021. Nikita Klimov. All rights reserved.

package com.kulichin.firebaseauthentication;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class EmailAuthentication
{
    private final FirebaseAuth FirebaseAuthInstance;

    public EmailAuthentication()
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
    }

    public void CreateAccountWithEmail(String Email, String Password)
    {
        FirebaseAuthInstance.createUserWithEmailAndPassword(Email, Password)
        .addOnCompleteListener(Task ->
        {
            if (Task.isSuccessful())
            {
                BaseAuthentication.FirebaseFunctionSuccessfully();
            }
            else
            {
                BaseAuthentication.FirebaseFunctionException(Task.getException());
            }
        });
    }

    public void EmailSignIn(String Email, String Password)
    {
        FirebaseAuthInstance.signInWithEmailAndPassword(Email, Password)
        .addOnCompleteListener(Task -> 
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

    public void SendEmailVerification()
    {
        final FirebaseUser User = FirebaseAuthInstance.getCurrentUser();
        if (User != null)
        {
            User.sendEmailVerification().addOnCompleteListener(Task ->
            {
                if (Task.isSuccessful())
                {
                    BaseAuthentication.FirebaseFunctionSuccessfully();
                }
                else
                {
                    BaseAuthentication.FirebaseFunctionException(Task.getException());
                }
            });
        }
    }

        public void SendPasswordResetLinkOnEmail(String Email)
	{
        FirebaseAuthInstance.sendPasswordResetEmail(Email)
        .addOnCompleteListener(Task ->
        {
            if (Task.isSuccessful())
            {
                BaseAuthentication.FirebaseFunctionSuccessfully();
            }
            else
            {
                BaseAuthentication.FirebaseFunctionException(Task.getException());
            }
        });
	}
	
    public void UpdatePassword(String NewPassword)
	{
        final FirebaseUser User = FirebaseAuthInstance.getCurrentUser();
        if (User != null)
        {
            User.updatePassword(NewPassword).addOnCompleteListener(Task ->
            {
                if (Task.isSuccessful())
                {
                    BaseAuthentication.FirebaseFunctionSuccessfully();
                }
                else
                {
                    BaseAuthentication.FirebaseFunctionException(Task.getException());
                }
            });
        }
	}
}
