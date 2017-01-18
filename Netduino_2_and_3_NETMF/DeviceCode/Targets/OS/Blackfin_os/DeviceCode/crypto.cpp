////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"
#include "crypto.h"
//--//

CRYPTO_RESULT Crypto_GetFingerprint(BYTE *key, BYTE *Signature, int cbSignatureSize)
{
    return CRYPTO_SUCCESS;
}
BOOL Crypto_GetActivationStringFromSeed(char *pString, int cbStringSize, KeySeed *Seed, UINT16 region, UINT16 model)
{
    return FALSE;
}

// Encrypts a buffer using a symmetric algorithm.
BOOL Crypto_Encrypt(BYTE *Key, BYTE *IV, DWORD cbIVSize, BYTE* pPlainText, DWORD cbPlainText, BYTE *pCypherText, DWORD cbCypherText)
{
    return FALSE;
}

// Decrypts a buffer using a symmetric algorithm
BOOL Crypto_Decrypt(BYTE *Key, BYTE *IV, DWORD cbIVSize, BYTE *pCypherText, DWORD cbCypherText, BYTE* pPlainText, DWORD cbPlainText)
{
    return FALSE;
}

const RSABuffer MSPublicKeyModulus = {0};

CRYPTO_RESULT Crypto_StartRSAOperationWithKey(enum RSAOperations operation, RSAKey *pRSAKey, BYTE *pSourceText, DWORD cbSourceText, BYTE *pDestText, DWORD cbDestText,
                               void **ppHandle)
{
    return CRYPTO_SUCCESS;
}

//
// this function continues a RSA operation (using the handle returned by a previous call to  
// Crypto_StartRsaOperationWithKey or Crypto_StepRsaOperation). It returns an error code(< 0), CRYPTO_SUCCESS (= 0), or CRYPTO_CONTINUE (=1)
// For the client, the caller may either call this function in a loop itself and use the return value to check for the end of the process 
// or use the continuation mechanism and check the contents of pResult parameter at intervals.
// The continuation mechanism is stubbed out on the server, so the caller needs to keep calling the function until it stops returning CRYPTO_CONTINUE
// !!! Mar 12: removed continuation mechanism - coscor !!!

//CRYPTO_RESULT Crypto_StepRSAOperation(void** pHandle);

CRYPTO_RESULT Crypto_AbortRSAOperation(void** pHandle)
{
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT Crypto_GeneratePrivateKey(KeySeed *pSeed, RSAKey* pPrivateKey)
{
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT Crypto_PublicKeyFromPrivate(const RSAKey *privateKey, RSAKey*publicKey)
{
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT Crypto_PublicKeyFromModulus(const RSABuffer *modulus, RSAKey *key)
{
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT GetC1Value(BYTE *key, BYTE *C1, int cbC1)
{
    return CRYPTO_SUCCESS;
}

CRYPTO_RESULT Crypto_KeySeedFromLaser(const BYTE *laserData, KeySeed* keySeed)
{
    return CRYPTO_SUCCESS;
}


CRYPTO_RESULT Crypto_StepRSAOperation(void** pHandle)
{
    return CRYPTO_SUCCESS;
}
