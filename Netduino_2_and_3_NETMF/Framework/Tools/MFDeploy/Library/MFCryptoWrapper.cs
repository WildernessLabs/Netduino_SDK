using System;
using System.Text;
using System.Runtime.InteropServices;

namespace dotNetMFCrypto
{
    public sealed class CryptoWrapper
    {
        public enum CRYPTO_RESULT
        {
            CONTINUE = 1,	// operation should continue
            SUCCESS = 0,		// alles gute
            SIGNATUREFAIL = -1,
            BADPARMS = -2,
            KEYEXPIRED = -3,
            UNKNOWNKEY = -4,
            UNKNOWNERROR = -5,
            NOMEMORY = -6,
            ACTIVATIONBADSYNTAX = -7,
            ACTIVATIONBADCONTROLCHAR = -8,
            FAILURE = -9
        }

        public enum RSA_OPS
        {
            ENCRYPT,
            DECRYPT,
            VERIFYSIGNATURE
        }

        [DllImport(@"Crypto.dll")]
        static public extern bool Crypto_Encrypt([In] byte[] Key, [In, Out] byte[] IV, int cbIVSize, [In] byte[] pPlainText, int cbPlainText, [Out] byte[] pCypherText, int cbCypherText);
        [DllImport(@"Crypto.dll")]
        static public extern bool Crypto_Decrypt([In] byte[] Key, [In, Out] byte[] IV, int cbIVSize, [In] byte[] pCypherText, int cbCypherText, [Out] byte[] pPlainText, int cbPlainText);
        [DllImport(@"Crypto.dll")]
        static public extern int Crypto_CreateZenithKey([In] byte[] seed, out ushort delta1, out ushort delta2);
        [DllImport(@"Crypto.dll")]
        static public extern int Crypto_SignBuffer([In] byte[] buffer, int bufLen, [In] byte[] key, [Out] byte[] signature, int siglen);
        [DllImport(@"Crypto.dll")]
        static public extern int Crypto_GeneratePrivateKey([In] byte[] seed, [Out] byte[] privateKey);
        [DllImport(@"Crypto.dll")]
        static public extern int Crypto_PublicKeyFromPrivate([In] byte[] privkey, [Out] byte[] pubkey);
    }
}

