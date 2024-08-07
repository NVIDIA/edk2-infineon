/**
 *  @brief      Declares the TSS_*_Marshal and TSS_*_Unmarshal methods
 *  @file       TPM2_Marshal.h
 *  @details    This file was auto-generated based on TPM2.0 specification revision 116.
 *
 *              Copyright Licenses:
 *
 *              * Trusted Computing Group (TCG) grants to the user of the source code
 *              in this specification (the "Source Code") a worldwide, irrevocable,
 *              nonexclusive, royalty free, copyright license to reproduce, create
 *              derivative works, distribute, display and perform the Source Code and
 *              derivative works thereof, and to grant others the rights granted
 *              herein.
 *
 *              * The TCG grants to the user of the other parts of the specification
 *              (other than the Source Code) the rights to reproduce, distribute,
 *              display, and perform the specification solely for the purpose of
 *              developing products based on such documents.
 *
 *              Source Code Distribution Conditions:
 *
 *              * Redistributions of Source Code must retain the above copyright
 *              licenses, this list of conditions and the following disclaimers.
 *
 *              * Redistributions in binary form must reproduce the above copyright
 *              licenses, this list of conditions and the following disclaimers in the
 *              documentation and/or other materials provided with the distribution.
 *
 *              Disclaimers:
 *
 *              * THE COPYRIGHT LICENSES SET FORTH ABOVE DO NOT REPRESENT ANY FORM OF
 *              LICENSE OR WAIVER, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, WITH
 *              RESPECT TO PATENT RIGHTS HELD BY TCG MEMBERS (OR OTHER THIRD PARTIES)
 *              THAT MAY BE NECESSARY TO IMPLEMENT THIS SPECIFICATION OR
 *              OTHERWISE. Contact TCG Administration
 *              (admin@trustedcomputinggroup.org) for information on specification
 *              licensing rights available through TCG membership agreements.
 *
 *              * THIS SPECIFICATION IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED
 *              WARRANTIES WHATSOEVER, INCLUDING ANY WARRANTY OF MERCHANTABILITY OR
 *              FITNESS FOR A PARTICULAR PURPOSE, ACCURACY, COMPLETENESS, OR
 *              NONINFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS, OR ANY WARRANTY
 *              OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.
 *
 *              * Without limitation, TCG and its members and licensors disclaim all
 *              liability, including liability for infringement of any proprietary
 *              rights, relating to use of information in this specification and to
 *              the implementation of this specification, and TCG disclaims all
 *              liability for cost of procurement of substitute goods or services,
 *              lost profits, loss of use, loss of data or any incidental,
 *              consequential, direct, indirect, or special damages, whether under
 *              contract, tort, warranty or otherwise, arising in any way out of use
 *              or reliance upon this specification or any information herein.
 *
 *              Any marks and brands contained herein are the property of their
 *              respective owners.
 */
#pragma once

#include "TPM2_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief      Marshals a UINT8 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT8_Marshal(
    _In_    const TSS_UINT8*    PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a UINT8 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT8_Unmarshal(
    _Out_   TSS_UINT8*      PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize);

/**
 *  @brief      Marshals a UINT8 array
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT8_Array_Marshal(
    _In_    const TSS_UINT8*    PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_INT32           PnCount);

/**
 *  @brief      Unmarshals a UINT8 array
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT8_Array_Unmarshal(
    _Out_   TSS_UINT8*      PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize,
    _In_    TSS_INT32       PnCount);

/**
 *  @brief      Marshals a BYTE type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BYTE_Marshal(
    _In_    const TSS_BYTE*     PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a BYTE type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BYTE_Unmarshal(
    _Out_   TSS_BYTE*       PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize);

/**
 *  @brief      Marshals a BYTE array
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BYTE_Array_Marshal(
    _In_    const TSS_BYTE*     PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_INT32           PnCount);

/**
 *  @brief      Unmarshals a BYTE array
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BYTE_Array_Unmarshal(
    _Out_   TSS_BYTE*       PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize,
    _In_    TSS_INT32       PnCount);

/**
 *  @brief      Marshals a BOOL type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BOOL_Marshal(
    _In_    const TSS_BOOL*     PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a BOOL type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_BOOL_Unmarshal(
    _Out_   TSS_BOOL*       PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize);

/**
 *  @brief      Marshals a UINT16 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT16_Marshal(
    _In_    const TSS_UINT16*       PpSource,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Unmarshals a UINT16 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT16_Unmarshal(
    _Out_   TSS_UINT16*     PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize);

/**
 *  @brief      Marshals a UINT32 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT32_Marshal(
    _In_    const TSS_UINT32*       PpSource,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Unmarshals a UINT32 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT32_Unmarshal(
    _Out_   TSS_UINT32*     PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize);

/**
 *  @brief      Unmarshals a UINT32 array
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_UINT32_Array_Unmarshal(
    _Out_   TSS_UINT32*     PpTarget,
    _Inout_ TSS_BYTE**      PprgbBuffer,
    _Inout_ TSS_INT32*      PpnSize,
    _In_    TSS_INT32       PnCount);

/**
 *  @brief      Marshals a INT32 type
 *  @details    Refer to: Table 3 - Definition of Base Types
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_INT32_Marshal(
    _In_    const TSS_INT32*    PpSource,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TPM_ALGORITHM_ID type
 *  @details    Refer to: Table 5 - Definition of Types for Documentation Clarity
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ALGORITHM_ID_Unmarshal(
    _Out_   TSS_TPM_ALGORITHM_ID*       PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TPM_KEY_BITS type
 *  @details    Refer to: Table 5 - Definition of Types for Documentation Clarity
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_KEY_BITS_Marshal(
    _In_    const TSS_TPM_KEY_BITS*     PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

#define TSS_TPMI_AES_KEY_BITS_Marshal(PpSource, PprgbBuffer, PpnSize) TSS_TPM_KEY_BITS_Marshal((TSS_TPM_KEY_BITS *)(PpSource), PprgbBuffer, PpnSize)
#define TSS_TPMI_SM4_KEY_BITS_Marshal(PpSource, PprgbBuffer, PpnSize) TSS_TPM_KEY_BITS_Marshal((TSS_TPM_KEY_BITS *)(PpSource), PprgbBuffer, PpnSize)
#define TSS_TPMI_CAMELLIA_KEY_BITS_Marshal(PpSource, PprgbBuffer, PpnSize) TSS_TPM_KEY_BITS_Marshal((TSS_TPM_KEY_BITS *)(PpSource), PprgbBuffer, PpnSize)

/**
 *  @brief      Marshals a TSS_TPM_ALG_ID type
 *  @details    Refer to: Table 9 - Definition of (UINT16) TPM_ALG_ID Constants [IN/OUT, S]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ALG_ID_Marshal(
    _In_    const TSS_TPM_ALG_ID*           PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_ALG_ID type
 *  @details    Refer to: Table 9 - Definition of (UINT16) TPM_ALG_ID Constants [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ALG_ID_Unmarshal(
    _Out_   TSS_TPM_ALG_ID*         PpTarget,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_ECC_CURVE type
 *  @details    Refer to: Table 10 - Definition of (UINT16) {ECC} TPM_ECC_CURVE Constants [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ECC_CURVE_Unmarshal(
    _Out_   TSS_TPM_ECC_CURVE*          PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_ECC_CURVE array
 *  @details    Refer to: Table 10 - Definition of (UINT16) {ECC} TPM_ECC_CURVE Constants [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ECC_CURVE_Array_Unmarshal(
    _Out_   TSS_TPM_ECC_CURVE*          PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize,
    _In_    TSS_INT32                   PnCount);

/**
 *  @brief      Marshals a TSS_TPM_CC type
 *  @details    Refer to: Table 13 - Definition of (UINT32) TPM_CC Constants (Numeric Order) [IN/OUT, S]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_CC_Marshal(
    _In_    const TSS_TPM_CC*           PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_CC type
 *  @details    Refer to: Table 13 - Definition of (UINT32) TPM_CC Constants (Numeric Order) [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_CC_Unmarshal(
    _Out_   TSS_TPM_CC*         PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_CC array
 *  @details    Refer to: Table 13 - Definition of (UINT32) TPM_CC Constants (Numeric Order) [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_CC_Array_Unmarshal(
    _Out_   TSS_TPM_CC*         PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_INT32           PnCount);

/**
 *  @brief      Unmarshals a TSS_TPM_RC type
 *  @details    Refer to: Table 17 - Definition of (UINT32) TPM_RC Constants (Actions) [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_RC_Unmarshal(
    _Out_   TSS_TPM_RC*         PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TSS_TPM_ST type
 *  @details    Refer to: Table 20 - Definition of (UINT16) TPM_ST Constants [IN/OUT, S]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ST_Marshal(
    _In_    const TSS_TPM_ST*           PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_ST type
 *  @details    Refer to: Table 20 - Definition of (UINT16) TPM_ST Constants [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_ST_Unmarshal(
    _Out_   TSS_TPM_ST*         PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TSS_TPM_SU type
 *  @details    Refer to: Table 21 - Definition of (UINT16) TPM_SU Constants [IN]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_SU_Marshal(
    _In_    const TSS_TPM_SU*           PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TSS_TPM_SE type
 *  @details    Refer to: Table 22 - Definition of (UINT8) TPM_SE Constants [IN]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_SE_Marshal(
    _In_    const TSS_TPM_SE*           PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TSS_TPM_CAP type
 *  @details    Refer to: Table 23 - Definition of (UINT32) TPM_CAP Constants
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_CAP_Marshal(
    _In_    const TSS_TPM_CAP*          PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_CAP type
 *  @details    Refer to: Table 23 - Definition of (UINT32) TPM_CAP Constants
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_CAP_Unmarshal(
    _Out_   TSS_TPM_CAP*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM_PT type
 *  @details    Refer to: Table 24 - Definition of (UINT32) TPM_PT Constants [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_PT_Unmarshal(
    _Out_   TSS_TPM_PT*         PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TPM_HANDLE type
 *  @details    Refer to: Table 27 - Definition of Types for Handles
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_HANDLE_Marshal(
    _In_    const TSS_TPM_HANDLE*       PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TPM_HANDLE type
 *  @details    Refer to: Table 27 - Definition of Types for Handles
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_HANDLE_Unmarshal(
    _Out_   TSS_TPM_HANDLE*     PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TPM_HANDLE array
 *  @details    Refer to: Table 27 - Definition of Types for Handles
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM_HANDLE_Array_Unmarshal(
    _Out_   TSS_TPM_HANDLE*     PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_INT32           PnCount);

/**
 *  @brief      Unmarshals a TSS_TPMA_ALGORITHM type
 *  @details    Refer to: Table 31 - Definition of (UINT32) TPMA_ALGORITHM Bits
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMA_ALGORITHM_Unmarshal(
    _Out_   TSS_TPMA_ALGORITHM*         PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TSS_TPMA_SESSION type
 *  @details    Refer to: Table 33 - Definition of (UINT8) TPMA_SESSION Bits [IN/OUT]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMA_SESSION_Marshal(
    _In_    const TSS_TPMA_SESSION*         PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMA_SESSION type
 *  @details    Refer to: Table 33 - Definition of (UINT8) TPMA_SESSION Bits [IN/OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMA_SESSION_Unmarshal(
    _Out_   TSS_TPMA_SESSION*           PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMA_CC type
 *  @details    Refer to: Table 38 - Definition of (TPM_CC) TPMA_CC Bits [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMA_CC_Unmarshal(
    _Out_   TSS_TPMA_CC*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMA_CC array
 *  @details    Refer to: Table 38 - Definition of (TPM_CC) TPMA_CC Bits [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMA_CC_Array_Unmarshal(
    _Out_   TSS_TPMA_CC*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_INT32           PnCount);

/**
 *  @brief      Unmarshals a TSS_TPMI_YES_NO type
 *  @details    Refer to: Table 39 - Definition of (BYTE) TPMI_YES_NO Type
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_YES_NO_Unmarshal(
    _Out_   TSS_TPMI_YES_NO*        PpTarget,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_DH_OBJECT type
 *  @details    Refer to: Table 40 - Definition of (TPM_HANDLE) TPMI_DH_OBJECT Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_DH_OBJECT_Marshal(
    _In_    const TSS_TPMI_DH_OBJECT*           PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_DH_ENTITY type
 *  @details    Refer to: Table 42 - Definition of (TPM_HANDLE) TPMI_DH_ENTITY Type [IN]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_DH_ENTITY_Marshal(
    _In_    const TSS_TPMI_DH_ENTITY*           PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_SH_AUTH_SESSION type
 *  @details    Refer to: Table 44 - Definition of (TPM_HANDLE) TPMI_SH_AUTH_SESSION Type [IN/OUT]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_SH_AUTH_SESSION_Marshal(
    _In_    const TSS_TPMI_SH_AUTH_SESSION*         PpSource,
    _Inout_ TSS_BYTE**                              PprgbBuffer,
    _Inout_ TSS_INT32*                              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMI_SH_AUTH_SESSION type
 *  @details    Refer to: Table 44 - Definition of (TPM_HANDLE) TPMI_SH_AUTH_SESSION Type [IN/OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_SH_AUTH_SESSION_Unmarshal(
    _Out_   TSS_TPMI_SH_AUTH_SESSION*           PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_SH_POLICY type
 *  @details    Refer to: Table 46 - Definition of (TPM_HANDLE) TPMI_SH_POLICY Type [IN/OUT]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_SH_POLICY_Marshal(
    _In_    const TSS_TPMI_SH_POLICY*           PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_DH_CONTEXT type
 *  @details    Refer to: Table 47 - Definition of (TPM_HANDLE) TPMI_DH_CONTEXT Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_DH_CONTEXT_Marshal(
    _In_    const TSS_TPMI_DH_CONTEXT*          PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMI_RH_HIERARCHY type
 *  @details    Refer to: Table 48 - Definition of (TPM_HANDLE) TPMI_RH_HIERARCHY Type
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_RH_HIERARCHY_Unmarshal(
    _Out_   TSS_TPMI_RH_HIERARCHY*          PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_RH_HIERARCHY_AUTH type
 *  @details    Refer to: Table 50 - Definition of (TPM_HANDLE) TPMI_RH_HIERARCHY_AUTH Type [IN]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_RH_HIERARCHY_AUTH_Marshal(
    _In_    const TSS_TPMI_RH_HIERARCHY_AUTH*           PpSource,
    _Inout_ TSS_BYTE**                                  PprgbBuffer,
    _Inout_ TSS_INT32*                                  PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_RH_PLATFORM type
 *  @details    Refer to: Table 51 - Definition of (TPM_HANDLE) TPMI_RH_PLATFORM Type [IN]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_RH_PLATFORM_Marshal(
    _In_    const TSS_TPMI_RH_PLATFORM*         PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_ALG_HASH type
 *  @details    Refer to: Table 59 - Definition of (TPM_ALG_ID) TPMI_ALG_HASH Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ALG_HASH_Marshal(
    _In_    const TSS_TPMI_ALG_HASH*        PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMI_ALG_HASH type
 *  @details    Refer to: Table 59 - Definition of (TPM_ALG_ID) TPMI_ALG_HASH Type
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ALG_HASH_Unmarshal(
    _Out_   TSS_TPMI_ALG_HASH*          PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_ALG_SYM type
 *  @details    Refer to: Table 61 - Definition of (TPM_ALG_ID) TPMI_ALG_SYM Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ALG_SYM_Marshal(
    _In_    const TSS_TPMI_ALG_SYM*         PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_ALG_SYM_MODE type
 *  @details    Refer to: Table 63 - Definition of (TPM_ALG_ID) TPMI_ALG_SYM_MODE Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ALG_SYM_MODE_Marshal(
    _In_    const TSS_TPMI_ALG_SYM_MODE*        PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMI_ST_COMMAND_TAG type
 *  @details    Refer to: Table 67 - Definition of (TPM_ST) TPMI_ST_COMMAND_TAG Type
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ST_COMMAND_TAG_Marshal(
    _In_    const TSS_TPMI_ST_COMMAND_TAG*          PpSource,
    _Inout_ TSS_BYTE**                              PprgbBuffer,
    _Inout_ TSS_INT32*                              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMI_ST_COMMAND_TAG type
 *  @details    Refer to: Table 67 - Definition of (TPM_ST) TPMI_ST_COMMAND_TAG Type
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMI_ST_COMMAND_TAG_Unmarshal(
    _Out_   TSS_TPMI_ST_COMMAND_TAG*        PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Marshals a TSS_TPMU_HA union
 *  @details    Refer to: Table 70 - Definition of TPMU_HA Union [IN/OUT, S]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PunSelector Selector for the source.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMU_HA_Marshal(
    _In_    const TSS_TPMU_HA*          PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize,
    _In_    TSS_UINT32                  PunSelector);
/**
 *  @brief      Unmarshals a TSS_TPMU_HA union
 *  @details    Refer to: Table 70 - Definition of TPMU_HA Union [IN/OUT, S]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PunSelector Selector for the target.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMU_HA_Unmarshal(
    _Inout_ TSS_TPMU_HA*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize,
    _In_    TSS_UINT32          PunSelector);

/**
 *  @brief      Marshals a TSS_TPMT_HA structure
 *  @details    Refer to: Table 71 - Definition of TPMT_HA Structure [IN/OUT]
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMT_HA_Marshal(
    _In_    const TSS_TPMT_HA*          PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMT_HA structure
 *  @details    Refer to: Table 71 - Definition of TPMT_HA Structure [IN/OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMT_HA_Unmarshal(
    _Out_   TSS_TPMT_HA*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TSS_TPM2B_DIGEST structure
 *  @details    Refer to: Table 72 - Definition of TPM2B_DIGEST Structure
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_DIGEST_Marshal(
    _In_    const TSS_TPM2B_DIGEST*         PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM2B_DIGEST structure
 *  @details    Refer to: Table 72 - Definition of TPM2B_DIGEST Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_DIGEST_Unmarshal(
    _Out_   TSS_TPM2B_DIGEST*           PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Marshals a TPM2B_NONCE type
 *  @details    Refer to: Table 74 - Definition of Types for TPM2B_NONCE
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_NONCE_Marshal(
    _In_    const TSS_TPM2B_NONCE*      PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TPM2B_NONCE type
 *  @details    Refer to: Table 74 - Definition of Types for TPM2B_NONCE
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_NONCE_Unmarshal(
    _Out_   TSS_TPM2B_NONCE*    PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TPM2B_AUTH type
 *  @details    Refer to: Table 75 - Definition of Types for TPM2B_AUTH
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_AUTH_Marshal(
    _In_    const TSS_TPM2B_AUTH*       PpSource,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TPM2B_AUTH type
 *  @details    Refer to: Table 75 - Definition of Types for TPM2B_AUTH
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_AUTH_Unmarshal(
    _Out_   TSS_TPM2B_AUTH*     PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Marshals a TSS_TPM2B_MAX_BUFFER structure
 *  @details    Refer to: Table 78 - Definition of TPM2B_MAX_BUFFER Structure
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_MAX_BUFFER_Marshal(
    _In_    const TSS_TPM2B_MAX_BUFFER*         PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM2B_MAX_BUFFER structure
 *  @details    Refer to: Table 78 - Definition of TPM2B_MAX_BUFFER Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_MAX_BUFFER_Unmarshal(
    _Out_   TSS_TPM2B_MAX_BUFFER*           PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPM2B_MAX_BUFFER array
 *  @details    Refer to: Table 78 - Definition of TPM2B_MAX_BUFFER Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_MAX_BUFFER_Array_Unmarshal(
    _Out_   TSS_TPM2B_MAX_BUFFER*           PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize,
    _In_    TSS_INT32                       PnCount);

/**
 *  @brief      Unmarshals a TSS_TPM2B_TIMEOUT structure
 *  @details    Refer to: Table 80 - Definition of TPM2B_TIMEOUT Structure [IN/OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_TIMEOUT_Unmarshal(
    _Out_   TSS_TPM2B_TIMEOUT*          PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_PCR_SELECTION structure
 *  @details    Refer to: Table 85 - Definition of TPMS_PCR_SELECTION Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_PCR_SELECTION_Unmarshal(
    _Out_   TSS_TPMS_PCR_SELECTION*         PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_PCR_SELECTION array
 *  @details    Refer to: Table 85 - Definition of TPMS_PCR_SELECTION Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_PCR_SELECTION_Array_Unmarshal(
    _Out_   TSS_TPMS_PCR_SELECTION*         PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize,
    _In_    TSS_INT32                       PnCount);

/**
 *  @brief      Unmarshals a TSS_TPMT_TK_AUTH structure
 *  @details    Refer to: Table 90 - Definition of TPMT_TK_AUTH Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMT_TK_AUTH_Unmarshal(
    _Out_   TSS_TPMT_TK_AUTH*           PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_ALG_PROPERTY structure
 *  @details    Refer to: Table 92 - Definition of TPMS_ALG_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_ALG_PROPERTY_Unmarshal(
    _Out_   TSS_TPMS_ALG_PROPERTY*          PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_ALG_PROPERTY array
 *  @details    Refer to: Table 92 - Definition of TPMS_ALG_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_ALG_PROPERTY_Array_Unmarshal(
    _Out_   TSS_TPMS_ALG_PROPERTY*          PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize,
    _In_    TSS_INT32                       PnCount);

/**
 *  @brief      Unmarshals a TSS_TPMS_TAGGED_PROPERTY structure
 *  @details    Refer to: Table 93 - Definition of TPMS_TAGGED_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_TAGGED_PROPERTY_Unmarshal(
    _Out_   TSS_TPMS_TAGGED_PROPERTY*           PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_TAGGED_PROPERTY array
 *  @details    Refer to: Table 93 - Definition of TPMS_TAGGED_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_TAGGED_PROPERTY_Array_Unmarshal(
    _Out_   TSS_TPMS_TAGGED_PROPERTY*           PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize,
    _In_    TSS_INT32                           PnCount);

/**
 *  @brief      Unmarshals a TSS_TPMS_TAGGED_PCR_SELECT structure
 *  @details    Refer to: Table 94 - Definition of TPMS_TAGGED_PCR_SELECT Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_TAGGED_PCR_SELECT_Unmarshal(
    _Out_   TSS_TPMS_TAGGED_PCR_SELECT*         PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMS_TAGGED_PCR_SELECT array
 *  @details    Refer to: Table 94 - Definition of TPMS_TAGGED_PCR_SELECT Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PnCount     Number of elements.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_TAGGED_PCR_SELECT_Array_Unmarshal(
    _Out_   TSS_TPMS_TAGGED_PCR_SELECT*         PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize,
    _In_    TSS_INT32                           PnCount);

/**
 *  @brief      Unmarshals a TSS_TPML_CC structure
 *  @details    Refer to: Table 95 - Definition of TPML_CC Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_CC_Unmarshal(
    _Out_   TSS_TPML_CC*        PpTarget,
    _Inout_ TSS_BYTE**          PprgbBuffer,
    _Inout_ TSS_INT32*          PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_CCA structure
 *  @details    Refer to: Table 96 - Definition of TPML_CCA Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_CCA_Unmarshal(
    _Out_   TSS_TPML_CCA*           PpTarget,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_HANDLE structure
 *  @details    Refer to: Table 98 - Definition of TPML_HANDLE Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_HANDLE_Unmarshal(
    _Out_   TSS_TPML_HANDLE*        PpTarget,
    _Inout_ TSS_BYTE**              PprgbBuffer,
    _Inout_ TSS_INT32*              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_PCR_SELECTION structure
 *  @details    Refer to: Table 102 - Definition of TPML_PCR_SELECTION Structure
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_PCR_SELECTION_Unmarshal(
    _Out_   TSS_TPML_PCR_SELECTION*         PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_ALG_PROPERTY structure
 *  @details    Refer to: Table 103 - Definition of TPML_ALG_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_ALG_PROPERTY_Unmarshal(
    _Out_   TSS_TPML_ALG_PROPERTY*          PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_TAGGED_TPM_PROPERTY structure
 *  @details    Refer to: Table 104 - Definition of TPML_TAGGED_TPM_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_TAGGED_TPM_PROPERTY_Unmarshal(
    _Out_   TSS_TPML_TAGGED_TPM_PROPERTY*           PpTarget,
    _Inout_ TSS_BYTE**                              PprgbBuffer,
    _Inout_ TSS_INT32*                              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_TAGGED_PCR_PROPERTY structure
 *  @details    Refer to: Table 105 - Definition of TPML_TAGGED_PCR_PROPERTY Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_TAGGED_PCR_PROPERTY_Unmarshal(
    _Out_   TSS_TPML_TAGGED_PCR_PROPERTY*           PpTarget,
    _Inout_ TSS_BYTE**                              PprgbBuffer,
    _Inout_ TSS_INT32*                              PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPML_ECC_CURVE structure
 *  @details    Refer to: Table 106 - Definition of {ECC} TPML_ECC_CURVE Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPML_ECC_CURVE_Unmarshal(
    _Out_   TSS_TPML_ECC_CURVE*         PpTarget,
    _Inout_ TSS_BYTE**                  PprgbBuffer,
    _Inout_ TSS_INT32*                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_TPMU_CAPABILITIES union
 *  @details    Refer to: Table 107 - Definition of TPMU_CAPABILITIES Union [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PunSelector Selector for the target.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMU_CAPABILITIES_Unmarshal(
    _Inout_ TSS_TPMU_CAPABILITIES*          PpTarget,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize,
    _In_    TSS_UINT32                      PunSelector);

/**
 *  @brief      Unmarshals a TSS_TPMS_CAPABILITY_DATA structure
 *  @details    Refer to: Table 108 - Definition of TPMS_CAPABILITY_DATA Structure [OUT]
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMS_CAPABILITY_DATA_Unmarshal(
    _Out_   TSS_TPMS_CAPABILITY_DATA*           PpTarget,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize);

/**
 *  @brief      Marshals a TSS_TPMU_SYM_KEY_BITS union
 *  @details    Refer to: Table 125 - Definition of TPMU_SYM_KEY_BITS Union
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PunSelector Selector for the source.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMU_SYM_KEY_BITS_Marshal(
    _In_    const TSS_TPMU_SYM_KEY_BITS*        PpSource,
    _Inout_ TSS_BYTE**                          PprgbBuffer,
    _Inout_ TSS_INT32*                          PpnSize,
    _In_    TSS_UINT32                          PunSelector);

/**
 *  @brief      Marshals a TSS_TPMU_SYM_MODE union
 *  @details    Refer to: Table 126 - Definition of TPMU_SYM_MODE Union
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *  @param      PunSelector Selector for the source.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMU_SYM_MODE_Marshal(
    _In_    const TSS_TPMU_SYM_MODE*        PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize,
    _In_    TSS_UINT32                      PunSelector);

/**
 *  @brief      Marshals a TSS_TPMT_SYM_DEF structure
 *  @details    Refer to: Table 128 - Definition of TPMT_SYM_DEF Structure
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPMT_SYM_DEF_Marshal(
    _In_    const TSS_TPMT_SYM_DEF*         PpSource,
    _Inout_ TSS_BYTE**                      PprgbBuffer,
    _Inout_ TSS_INT32*                      PpnSize);

/**
 *  @brief      Marshals a TSS_TPM2B_ENCRYPTED_SECRET structure
 *  @details    Refer to: Table 175 - Definition of TPM2B_ENCRYPTED_SECRET Structure
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_TPM2B_ENCRYPTED_SECRET_Marshal(
    _In_    const TSS_TPM2B_ENCRYPTED_SECRET*           PpSource,
    _Inout_ TSS_BYTE**                                  PprgbBuffer,
    _Inout_ TSS_INT32*                                  PpnSize);

/**
 *  @brief      Marshals a TSS_AuthorizationCommandData structure
 *  @details    Refer to: Table 14 - Session-Based Authorization of Command
 *
 *  @param      PpSource    Location containing the value that is to be marshaled in to the designated buffer.
 *  @param      PprgbBuffer Location in the output buffer where the first octet of the TYPE is to be placed.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_AuthorizationCommandData_Marshal(
    _In_    const TSS_AuthorizationCommandData*         PpSource,
    _Inout_ TSS_BYTE**                                  PprgbBuffer,
    _Inout_ TSS_INT32*                                  PpnSize);

/**
 *  @brief      Unmarshals a TSS_AcknowledgmentResponseData structure
 *  @details    Refer to: Table 15 - Session-Based Acknowledgment in Response
 *
 *  @param      PpTarget    Location into which the data from **PprgbBuffer is placed.
 *  @param      PprgbBuffer Location in the output buffer containing the most significant octet (MSO) of *PpTarget.
 *  @param      PpnSize     Number of octets remaining in **PprgbBuffer.
 *
 *  @retval     RC_SUCCESS  The operation completed successfully.
 *  @retval     ...         Error codes from called functions.
 */
_Check_return_
unsigned int
TSS_AcknowledgmentResponseData_Unmarshal(
    _Out_   TSS_AcknowledgmentResponseData*         PpTarget,
    _Inout_ TSS_BYTE**                              PprgbBuffer,
    _Inout_ TSS_INT32*                              PpnSize);

#ifdef __cplusplus
}
#endif
