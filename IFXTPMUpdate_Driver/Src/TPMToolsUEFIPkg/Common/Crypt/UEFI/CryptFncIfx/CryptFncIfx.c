/** @file
  Implements additional cryptographic functions

  This file implements additional cryptographic functions not available in CryptoPkg as wrapper over the OpenSSL library.

  Copyright 2014 - 2022 Infineon Technologies AG ( www.infineon.com )

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "InternalCryptLib.h"

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/objects.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

//
// Default public exponent: 0x10001 = 65537
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 DefaultPublicExponent[] = {
    0x01, 0x00, 0x01
};

/// Size of PSS padding salt in bytes
#define CRYPT_PSS_PADDING_SALT_SIZE 32

/// Size of RSA2048 public key in bytes
#define RSA2048_MODULUS_SIZE 256

/// Size of SHA1 digest in bytes
#define SHA1_DIGEST_SIZE 20

/**
  Verify the given RSA PKCS#1 RSASSA-PSS signature.

  This function verifies the given RSA PKCS#1 RSASSA-PSS signature with a RSA 2048-bit public key.

  If MessageHash is NULL, then return FALSE.
  If HashSize is not equal to the size of SHA-256 digest, then return FALSE.
  If Signature is NULL, then return FALSE.
  If SigSize is not equal to the size of RSA2048 signature, then return FALSE.
  If Modulus is NULL, then return FALSE.
  If ModulusSize is not equal to the size of RSA2048 public key, then return FALSE.

  @param[in]       MessageHash      Pointer to octet message hash to be used for RSASSA-PSS verification.
  @param[in]       MessageHashSize  Size of the message hash in bytes.
  @param[in]       Signature        Pointer to buffer that carries the RSASSA-PSS signature.
  @param[in]       SignatureSize    Size of the Signature buffer in bytes.
  @param[in]       Modulus          Pointer to buffer that carries the RSA public key.
  @param[in]       ModulusSize      Size of the Modulus buffer in bytes.

  @retval  TRUE   Signature verification succeeded.
  @retval  FALSE  Signature verification failed, Buffer size is invalid or pointer is NULL.
*/
BOOLEAN
EFIAPI
RsaPssVerifyIfx (
    const UINT8*  MessageHash,
    const UINT32  MessageHashSize,
    const UINT8*  Signature,
    const UINT32  SignatureSize,
    const UINT8*  Modulus,
    const UINT32  ModulusSize)
{
    BOOLEAN result = FALSE;
    RSA* RsaPublicKey = NULL;

    // Check input parameters
    if (NULL == MessageHash || 32 != MessageHashSize)
        goto _Exit;
    if (NULL == Signature || RSA2048_MODULUS_SIZE != SignatureSize)
        goto _Exit;
    if (NULL == Modulus || RSA2048_MODULUS_SIZE != ModulusSize)
        goto _Exit;

    // Initialize RSA Public Key object
    RsaPublicKey = RsaNew();
    if (NULL == RsaPublicKey)
        goto _Exit;

    // Set public key
    if (!RsaSetKey(RsaPublicKey, RsaKeyN, Modulus, ModulusSize))
        goto _Exit;

    // Set public exponent
    if (!RsaSetKey(RsaPublicKey, RsaKeyE, DefaultPublicExponent, sizeof(DefaultPublicExponent)))
        goto _Exit;

    {
        UINT8 DecryptedDigest[RSA2048_MODULUS_SIZE] = {0};
        if (-1 == RSA_public_decrypt(SignatureSize, Signature, DecryptedDigest, RsaPublicKey, RSA_NO_PADDING))
            goto _Exit;

        // Verify the signature
        if (1 != RSA_verify_PKCS1_PSS(RsaPublicKey, MessageHash, EVP_sha256(), DecryptedDigest, CRYPT_PSS_PADDING_SALT_SIZE))
            goto _Exit;

        // Success
        result = TRUE;
    }

_Exit:
    // Free RSA object and its components (BIGNUM)
    if (NULL != RsaPublicKey)
        RSA_free(RsaPublicKey);

    return result;
}

/// OAEP Pad
static const UINT8 g_OAEPPad[] = { 'T', 'C', 'P', 'A' };

/**
  Encrypt a byte array with a RSA 2048-bit public key

  This function encrypts the given data stream with RSA 2048-bit.

  If MessageHash is NULL, then return FALSE.
  If HashSize is not equal to the size of SHA-256 digest, then return FALSE.
  If Signature is NULL, then return FALSE.
  If SigSize is not equal to the size of RSA2048 signature, then return FALSE.
  If Modulus is NULL, then return FALSE.
  If ModulusSize is not equal to the size of RSA2048 public key, then return FALSE.

  @param[in]       InputDataSize       Size of the InputData buffer in bytes.
  @param[in]       InputData           Pointer to input data buffer.
  @param[in]       PublicModulusSize   Size of the PublicModulus buffer in bytes.
  @param[in]       PublicModulus       Pointer to modulus buffer.
  @param[in]       PublicExponentSize  Size of the PublicExponent buffer in bytes.
  @param[in]       PublicExponent      Pointer to public exponent buffer.
  @param[in,out]   EncryptedDataSize   Size of the EncryptedData buffer in bytes.
  @param[in,out]   EncryptedData       Pointer to encrypted data buffer.

  @retval  TRUE   Signature verification succeeded.
  @retval  FALSE  Signature verification failed, Buffer size is invalid or pointer is NULL.
*/
BOOLEAN
EFIAPI
RsaEncryptOaepIfx(
    UINT32        InputDataSize,
    const UINT8*  InputData,
    UINT32        PublicModulusSize,
    const UINT8*  PublicModulus,
    UINT32        PublicExponentSize,
    const UINT8*  PublicExponent,
    UINT32*       EncryptedDataSize,
    UINT8*        EncryptedData)
{
    BOOLEAN result = FALSE;
    RSA* RsaPublicKey = NULL;

    // Check input parameters
    if (NULL == InputData || 0 == InputDataSize ||
            NULL == PublicModulus || 0 == PublicModulusSize ||
            NULL == PublicExponent || 0 == PublicExponentSize ||
            NULL == EncryptedData || NULL == EncryptedDataSize)
        goto _Exit;

    if (*EncryptedDataSize < RSA2048_MODULUS_SIZE)
        goto _Exit;

    // Initialize RSA Public Key object
    RsaPublicKey = RsaNew();
    if (NULL == RsaPublicKey)
        goto _Exit;

    if (!RsaSetKey(RsaPublicKey, RsaKeyN, PublicModulus, PublicModulusSize))
        goto _Exit;

    if (!RsaSetKey(RsaPublicKey, RsaKeyE, (const UINT8*)PublicExponent, PublicExponentSize))
        goto _Exit;

    {
        UINT8 PaddedBuffer[RSA2048_MODULUS_SIZE] = { 0 };
        UINT32 PaddedBufferSize = sizeof(PaddedBuffer);
        UINT32 NumEncrypted = 0;

        // Add padding to the decrypted data
        if (!RSA_padding_add_PKCS1_OAEP(
                    (UINT8*)PaddedBuffer,
                    PaddedBufferSize,
                    (const UINT8*)InputData,
                    InputDataSize,
                    g_OAEPPad,
                    sizeof(g_OAEPPad)))
            goto _Exit;

        // Encrypt data with public key.
        NumEncrypted = RSA_public_encrypt(
                           PaddedBufferSize,
                           PaddedBuffer,
                           EncryptedData,
                           RsaPublicKey,
                           RSA_NO_PADDING);

        if (NumEncrypted == 0)
            goto _Exit;

        // SUCCESS
        *EncryptedDataSize = NumEncrypted;
        result = TRUE;
    }

_Exit:
    // Free RSA object and its components (BIGNUM)
    if (NULL != RsaPublicKey)
        RSA_free(RsaPublicKey);

    return result;
}

/**
  Calculate HMAC-SHA-1 on the given message.

  This function calculates a HMAC-SHA-1 on the input message.

  If InputMessage is NULL, then return FALSE.
  If InputMessageSize is zero, then return FALSE.
  If Key is NULL, then return FALSE.
  If KeySize is not equal to the of SHA1 digest, then return FALSE.
  If HmacResult is NULL, then return FALSE.
  If HmacResultSize is not equal to the size of SHA1 digest, then return FALSE.

  @param[in]        InputMessage         Pointer to input message buffer.
  @param[in]        InputMessageSize     Size of the InputMessage buffer in bytes.
  @param[in]        Key                  Pointer to message authentication key.
  @param[in]        KeySize              Size of the message authentication key buffer in bytes.
  @param[in,out]    HmacResult           Pointer to the HMAC result buffer in bytes.
  @param[in]        HmacResultSize       Size of to HMAC result buffer.

  @retval  TRUE   HMAC calculation succeeded.
  @retval  FALSE  HMAC calculation failed, Buffer size is invalid or pointer is NULL.
*/
BOOLEAN
EFIAPI
HmacSha1Ifx(
    const UINT8 *  InputMessage,
    const UINT16   InputMessageSize,
    const UINT8 *  Key,
    const UINT16   KeySize,
    const UINT8 *  HmacResult,
    const UINT16   HmacResultSize
)
{
    BOOLEAN result = FALSE;
    HMAC_CTX* sContext = NULL;
    unsigned int unHmacLength = SHA1_DIGEST_SIZE;

    // Check parameters
    if (NULL == InputMessage || 0 == InputMessageSize ||
            NULL == Key || SHA1_DIGEST_SIZE != KeySize ||
            NULL == HmacResult || SHA1_DIGEST_SIZE != HmacResultSize)
        goto _Exit;

    SetMem((VOID*)HmacResult, HmacResultSize, 0);

    // Calculate HMAC
    sContext = HMAC_CTX_new();
    if (NULL == sContext)
        goto _Exit;
    if (!HMAC_Init_ex(sContext, Key, KeySize, EVP_sha1(), NULL))
        goto _Exit;
    if (!HMAC_Update(sContext, InputMessage, InputMessageSize))
        goto _Exit;
    if (!HMAC_Final(sContext, (unsigned char*)HmacResult, &unHmacLength))
        goto _Exit;

    result = TRUE;

_Exit:
    // Free context if set
    if (NULL != sContext)
        HMAC_CTX_free(sContext);

    return result;
}
