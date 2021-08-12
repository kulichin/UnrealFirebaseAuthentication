// Copyright (C) 2021. Nikita Klimov. All rights reserved.

package com.kulichin.firebaseauthentication;

import android.app.Activity;
import android.support.annotation.NonNull;

import com.google.firebase.FirebaseException;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.PhoneAuthCredential;
import com.google.firebase.auth.PhoneAuthProvider;

import java.util.concurrent.TimeUnit;

public class PhoneAuthentication
{
    private String VerificationID;
    private PhoneAuthProvider.ForceResendingToken ResendToken;

    private final PhoneAuthProvider.OnVerificationStateChangedCallbacks AuthCallbacks;
    private final FirebaseAuth FirebaseAuthInstance;
    private final Activity MainActivity;

    public PhoneAuthentication(Activity MainActivity)
    {
        this.FirebaseAuthInstance = FirebaseAuth.getInstance();
        this.MainActivity = MainActivity;
		
		// Initialize phone auth callbacks
		this.AuthCallbacks = new PhoneAuthProvider.OnVerificationStateChangedCallbacks()
		{
			@Override
			public void onVerificationCompleted(PhoneAuthCredential Credential)
			{
				// This callback will be invoked in two situations:
				// 1 - Instant verification. In some cases the phone number can be instantly
				//     verified without needing to send or enter a verification code.
				// 2 - Auto-retrieval. On some devices Google Play services can automatically
				//     detect the incoming verification SMS and perform verification without
				//     user action.
				SignInWithPhoneAuthCredential(Credential);
			}
		
			@Override
			public void onVerificationFailed(FirebaseException e)
			{
				// This callback is invoked in an invalid request for verification is made,
				// for instance if the the phone number format is not valid.
                BaseAuthentication.PhoneAuthenticationException(e);
			}
		
			@Override
			public void onCodeSent(
                @NonNull String AuthVerificationID, 
                @NonNull PhoneAuthProvider.ForceResendingToken Token)
			{
				// The SMS verification code has been sent to the provided phone number, we
				// now need to ask the user to enter the code and then construct a credential
				// by combining the code with a verification ID.
		
				// Save verification ID and resending token so we can use them later
				VerificationID = AuthVerificationID;
				ResendToken = Token;

                BaseAuthentication.FirebaseFunctionSuccessfully();
			}
		};
    }

    public void SignInWithSMSCode(String PhoneNumber, int Timeout)
    {
        try
        {
            if (ResendToken == null)
            {
                PhoneAuthProvider.getInstance().verifyPhoneNumber
                (
                    PhoneNumber,        // Phone number to verify
                    Timeout,            // Timeout duration
                    TimeUnit.SECONDS,   // Unit of timeout
                    MainActivity,       // Activity (for callback binding)
                    AuthCallbacks       // OnVerificationStateChangedCallbacks
                );
            }
            else
            {
                PhoneAuthProvider.getInstance().verifyPhoneNumber
                (
                    PhoneNumber,        // Phone number to verify
                    Timeout,            // Timeout duration
                    TimeUnit.SECONDS,   // Unit of timeout
                    MainActivity,       // Activity (for callback binding)
                    AuthCallbacks,      // OnVerificationStateChangedCallbacks
                    ResendToken         // ForceResendingToken from callbacks
                );
            }
        }
        catch (Exception e)
        {
            BaseAuthentication.FirebaseFunctionException(e);
        }
    }

    private void SignInWithPhoneAuthCredential(PhoneAuthCredential Credential)
    {
        FirebaseAuthInstance.signInWithCredential(Credential).addOnCompleteListener(Task ->
        {
            if (Task.isSuccessful())
            {
                BaseAuthentication.PhoneAuthenticationSuccessfully(Credential.getSmsCode());
            }
            else
            {
                BaseAuthentication.PhoneAuthenticationException(Task.getException());
            }
        });
    }
}
