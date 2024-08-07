/**
 *  @brief      Implements the EFI_ADAPTER_INFORMATION_PROTOCOL
 *  @details    Implements the functions of EFI_ADAPTER_INFORMATION_PROTOCOL in the Infineon TPM Firmware Update Driver.
 *              @cond !SHOW_INTERNAL @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification" contains more details on EFI_ADAPTER_INFORMATION_PROTOCOL.
 *              Functions are prefixed with @a IFXTPMUpdate_AdapterInformation_ in this document but not in @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification".
 *              @endcond
 *  @file       AdapterInformation.c
 *
 *  Copyright 2014 - 2022 Infineon Technologies AG ( www.infineon.com )
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "AdapterInformation.h"
#include "TPM2_GetCapability.h"
#include "TPM2_HierarchyChangeAuth.h"
#include "TPM_GetCapability.h"
#include "TPM_Types.h"
#include "FirmwareManagement.h"
#include "FirmwareUpdate.h"
#include "IFXTPMUpdate.h"
#include "IFXTPMUpdateApp.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

/**
 *  @brief      Returns TPM firmware update counters.
 *  @details    This function returns information about TPM firmware update counters.
 *
 *  @param      PppInformationBlock         Pointer to pointer to store @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1 structure.
 *  @param      PpullInformationBlockSize   Pointer to store the size of the PppInformationBlock in bytes.
 *
 *  @retval     EFI_SUCCESS                 The requested information was returned successfully.
 *  @retval     EFI_INVALID_PARAMETER       In case of an invalid input parameter.
 *  @retval     EFI_DEVICE_ERROR            An unexpected error occurred.
 *  @retval     EFI_OUT_OF_RESOURCES        In case memory allocation failed.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetInformationCounters(
    OUT VOID** PppInformationBlock,
    OUT UINTN* PpullInformationBlockSize)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;

    do {
        // Parameter Check
        if (NULL == PppInformationBlock || NULL == PpullInformationBlockSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            break;
        }

        // Try to initialize TPM access
        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        // Get TPM information
        TPM_STATE sTpmState;
        Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));
        unsigned int unReturnValue = FirmwareUpdate_CalculateState(FALSE, &sTpmState);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during determination of TPM state in GetInformationCounters(). (0x%.16lX)", efiStatus);
            break;
        }

        // Get number of remaining firmware updates
        unsigned int unFieldUpgradeCounter = 0;
        unReturnValue = FirmwareUpdate_GetTpmFieldUpgradeCounter(sTpmState.attribs, &unFieldUpgradeCounter);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            break;
        }

        // Get number of remaining firmware updates onto the same firmware version (only TPM2.0 FU loader).
        // If counter is not supported value will be set to 0xFFFFFFFF.
        unsigned int unFieldUpgradeCounterSelf = 0xFFFFFFFF;
        if (sTpmState.attribs.tpmHasFULoader20)
        {
            unReturnValue = FirmwareUpdate_GetTpm20FieldUpgradeCounterSelf(&unFieldUpgradeCounterSelf);
            if (RC_SUCCESS != unReturnValue)
            {
                efiStatus = EFI_DEVICE_ERROR;
                break;
            }
        }

        // Allocate memory
        *PpullInformationBlockSize = sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1);
        *PppInformationBlock = AllocateZeroPool(*PpullInformationBlockSize);
        if (NULL == *PppInformationBlock)
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during memory allocation for PppInformationBlock in GetInformationCounters(). (0x%.16lX)", efiStatus);
            break;
        }

        // Set values within structure
        EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1* pInfoCounters = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1*)*PppInformationBlock;
        pInfoCounters->UpdateCounter = unFieldUpgradeCounter;
        pInfoCounters->UpdateCounterSelf = unFieldUpgradeCounterSelf;
        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting GetInformationCounters(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Returns TPM firmware update details.
 *  @details    This function returns details about the firmware update state.
 *
 *  @param      PppInformationBlock         Pointer to pointer to store @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1 structure.
 *  @param      PpullInformationBlockSize   Pointer to store the size of the PppInformationBlock in bytes.
 *
 *  @retval     EFI_SUCCESS                 The requested information was returned successfully.
 *  @retval     EFI_INVALID_PARAMETER       In case of an invalid input parameter.
 *  @retval     EFI_DEVICE_ERROR            An unexpected error occurred.
 *  @retval     EFI_UNSUPPORTED             The information is not available at the used TPM model.
 *  @retval     EFI_OUT_OF_RESOURCES        In case memory allocation failed.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetInformationFuDetails(
    OUT VOID** PppInformationBlock,
    OUT UINTN* PpullInformationBlockSize)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;

    do {
        // Parameter Check
        if (NULL == PppInformationBlock || NULL == PpullInformationBlockSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            break;
        }

        // Try to initialize TPM access
        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        // Get TPM information
        TPM_STATE sTpmState;
        Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));
        unsigned int unReturnValue = FirmwareUpdate_CalculateState(FALSE, &sTpmState);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during determination of TPM state in GetInformationFuDetails(). (0x%.16lX)", efiStatus);
            break;
        }

        // Firmware update details are only available with TPM2.0 FU loader
        if (FALSE == sTpmState.attribs.tpmHasFULoader20)
        {
            efiStatus = EFI_UNSUPPORTED;
            break;
        }

        // Get current firmware version
        TPM_FIRMWARE_VERSION sFirmwareVersion;
        Platform_MemorySet(&sFirmwareVersion, 0, sizeof(sFirmwareVersion));

        unReturnValue = FirmwareUpdate_GetTpmFirmwareVersionByVendorCap(TPM_PT_VENDOR_FIX_FU_CURRENT_TPM_FW_VERSION, &sFirmwareVersion);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1(L"FirmwareUpdate_GetTpmFirmwareVersionByVendorCap returned an unexpected value for current firmware version.");
            break;
        }

        // Get current firmware version
        TPM_FIRMWARE_VERSION sNewFirmwareVersion;
        Platform_MemorySet(&sNewFirmwareVersion, 0, sizeof(sNewFirmwareVersion));

        if (!sTpmState.attribs.tpmFirmwareIsValid)
        {
            unReturnValue = FirmwareUpdate_GetTpmFirmwareVersionByVendorCap(TPM_PT_VENDOR_FIX_FU_NEW_TPM_FW_VERSION, &sNewFirmwareVersion);
            if (RC_SUCCESS != unReturnValue)
            {
                efiStatus = EFI_DEVICE_ERROR;
                LOGGING_WRITE_LEVEL1(L"FirmwareUpdate_GetTpmFirmwareVersionByVendorCap returned an unexpected value for new firmware version.");
                break;
            }
        }

        // Get key group ID (internal1)
        UINT32 unKeyGroupId = 0;
        unReturnValue = FirmwareUpdate_GetTpmKeyGroupId(&unKeyGroupId);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1(L"FirmwareUpdate_GetTpmKeyGroupId returned an unexpected value.");
            break;
        }

        // Get FU_START_HASH (internal2)
        TSS_TPMS_VENDOR_CAPABILITY_DATA vendorCapabilityData;
        TSS_TPMI_YES_NO bMoreData = 0;
        Platform_MemorySet(&vendorCapabilityData, 0, sizeof(vendorCapabilityData));
        unReturnValue = TSS_TPM2_GetCapability(TSS_TPM_CAP_VENDOR_PROPERTY, TPM_PT_VENDOR_FIX_FU_START_HASH_DIGEST, 1, &bMoreData, (TSS_TPMS_CAPABILITY_DATA*)&vendorCapabilityData);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1_FMT(L"TSS_TPM2_GetCapability returned an unexpected value. (0x%.8X)", unReturnValue);
            break;
        }

        // Allocate memory (with all bytes set to zero)
        *PpullInformationBlockSize = sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1);
        *PppInformationBlock = AllocateZeroPool(*PpullInformationBlockSize);
        if (NULL == *PppInformationBlock)
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during memory allocation for PppInformationBlock in GetInformationFuDetails(). (0x%.16lX)", efiStatus);
            break;
        }
        EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1* pFuDetails = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1*)*PppInformationBlock;

        UINT32 unMaxFirmwareSize = sizeof(pFuDetails->FirmwareVersion) / sizeof(CHAR16);
        unReturnValue = Platform_StringFormat(&pFuDetails->FirmwareVersion[0], &unMaxFirmwareSize, L"%d.%d.%d.%d", sFirmwareVersion.usMajor, sFirmwareVersion.usMinor, sFirmwareVersion.usBuild, sFirmwareVersion.usRevision);
        if (RC_SUCCESS != unReturnValue)
        {
            ERROR_STORE(unReturnValue, L"Platform_StringFormat returned an unexpected value.");
            break;
        }

        if (!sTpmState.attribs.tpmFirmwareIsValid)
        {
            unMaxFirmwareSize = sizeof(pFuDetails->NewFirmwareVersion) / sizeof(CHAR16);
            unReturnValue = Platform_StringFormat(&pFuDetails->NewFirmwareVersion[0], &unMaxFirmwareSize, L"%d.%d.%d.%d", sNewFirmwareVersion.usMajor, sNewFirmwareVersion.usMinor, sNewFirmwareVersion.usBuild, sNewFirmwareVersion.usRevision);
            if (RC_SUCCESS != unReturnValue)
            {
                ERROR_STORE(unReturnValue, L"Platform_StringFormat returned an unexpected value.");
                break;
            }
        }

        pFuDetails->Internal1 = unKeyGroupId;
        pFuDetails->Internal2Size = vendorCapabilityData.data.vendorData.buffer[0].size;

        if (pFuDetails->Internal2Size <= sizeof(pFuDetails->Internal2)) {
            unReturnValue = Platform_MemoryCopy(&pFuDetails->Internal2[0], sizeof(pFuDetails->Internal2), &vendorCapabilityData.data.vendorData.buffer[0].buffer[0], pFuDetails->Internal2Size);
            if (RC_SUCCESS != unReturnValue)
            {
                ERROR_STORE(unReturnValue, L"Platform_MemoryCopy returned an unexpected value.");
                break;
            }
        }
        else
            LOGGING_WRITE_LEVEL1_FMT(L"Capability returned an unsupported value TPM_PT_VENDOR_FIX_FU_START_HASH_DIGEST (0x%.8lX)", RC_E_BUFFER_TOO_SMALL);

        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting GetInformationFuDetails(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Returns TPM operation mode information.
 *  @details    This function returns TPM operation mode information.
 *
 *  @param      PppInformationBlock         Pointer to pointer to store @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1_GUID structure.
 *  @param      PpullInformationBlockSize   Pointer to store the size of the PppInformationBlock in bytes.
 *
 *  @retval     EFI_SUCCESS                 The requested information was returned successfully.
 *  @retval     EFI_INVALID_PARAMETER       In case of an invalid input parameter.
 *  @retval     EFI_UNSUPPORTED             The information is not available at the used TPM model.
 *  @retval     EFI_DEVICE_ERROR            An unexpected error occurred.
 *  @retval     EFI_OUT_OF_RESOURCES        In case memory allocation failed.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetInformationOperationMode(
    OUT VOID** PppInformationBlock,
    OUT UINTN* PpullInformationBlockSize)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;

    do {
        // Parameter Check
        if (NULL == PppInformationBlock || NULL == PpullInformationBlockSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            break;
        }

        // Try to initialize TPM access
        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        // Get TPM information
        TPM_STATE sTpmState;
        Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));
        unsigned int unReturnValue = FirmwareUpdate_CalculateState(FALSE, &sTpmState);
        if (RC_SUCCESS != unReturnValue)
        {
            efiStatus = EFI_DEVICE_ERROR;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during determination of TPM state in GetInformationOperationMode(). (0x%.16lX)", efiStatus);
            break;
        }

        // Operation mode is only available on SLB 9672
        if (FALSE == sTpmState.attribs.tpmHasFULoader20)
        {
            efiStatus = EFI_UNSUPPORTED;
            break;
        }

        // Allocate memory (with all bytes set to zero)
        *PpullInformationBlockSize = sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1);
        *PppInformationBlock = AllocateZeroPool(*PpullInformationBlockSize);
        if (NULL == *PppInformationBlock)
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during memory allocation for PppInformationBlock in GetInformationOperationMode(). (0x%.16lX)", efiStatus);
            break;
        }

        // Set operation mode value and its boolean representations
        EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1* pInfoOperationMode = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1*)*PppInformationBlock;
        pInfoOperationMode->TpmOperationMode = (UINT8)sTpmState.attribs.tpm20OperationMode;
        pInfoOperationMode->TpmInOperationalMode = sTpmState.attribs.tpmInOperationalMode ? TRUE : FALSE;
        pInfoOperationMode->TpmInFirmwareUpdateMode = sTpmState.attribs.tpmInFwUpdateMode ? TRUE : FALSE;
        pInfoOperationMode->TpmInFirmwareRecoveryMode = sTpmState.attribs.tpmInFwRecoveryMode ? TRUE : FALSE;
        pInfoOperationMode->TpmFirmwareIsValid = sTpmState.attribs.tpmFirmwareIsValid ? TRUE : FALSE;
        pInfoOperationMode->TpmRestartRequired = sTpmState.attribs.tpm20restartRequired ? TRUE : FALSE;
        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting GetInformationOperationMode(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Returns the current state information for the adapter.
 *  @details    This function returns information of type PpInformationType for an adapter. The adapter supports the following information types:
 *              <table>
 *              <tr><th>Information Type</th><th>Description</th></tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1_GUID</td>
 *              <td>Use the information type to get TPM firmware update counters. The caller will receive @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1 structure.
 *              </tr>
 *              <tr><th>Information Type</th><th>Description</th></tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1_GUID</td>
 *              <td>Use the information type to get the TPM2.0 firmware update operation mode (only SLB 9672). The caller will receive @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1 structure.
 *              </tr>
  *              <tr><th>Information Type</th><th>Description</th></tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1_GUID</td>
 *              <td>Use the information type to get the TPM2.0 firmware update details (only SLB 9672). The caller will receive @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1 structure.
 *              </tr>
 *              </table>
 *              Otherwise EFI_UNSUPPORTED is returned.
 *  @param      PpThis                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *  @param      PpInformationType           A pointer to an EFI_GUID that defines the contents of PppInformationBlock.
 *  @param      PppInformationBlock         The service returns a pointer to the buffer with the PppInformationBlock structure which contains details about the data
 *                                          specific to PpInformationType.
 *  @param      PpullInformationBlockSize   The driver returns the size of the PppInformationBlock in bytes.
 *
 *  @retval     EFI_SUCCESS                 The requested information was returned successfully.
 *  @retval     EFI_INVALID_PARAMETER       In case of an invalid input parameter.
 *  @retval     EFI_UNSUPPORTED             The PpInformationType is not known.
 *  @retval     EFI_DEVICE_ERROR            An unexpected error occurred.
 *  @retval     EFI_OUT_OF_RESOURCES        In case memory allocation failed.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetInformation(
    IN  EFI_ADAPTER_INFORMATION_PROTOCOL*   PpThis,
    IN  EFI_GUID*                           PpInformationType,
    OUT VOID**                              PppInformationBlock,
    OUT UINTN*                              PpullInformationBlockSize)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;
    LOGGING_WRITE_LEVEL2(L"Entering EFI_ADAPTER_INFORMATION.GetInformation()");

    do
    {
        const EFI_GUID guidCounters = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1_GUID;
        const EFI_GUID guidOperationMode = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1_GUID;
        const EFI_GUID guidFuDetails = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1_GUID;

        // Parameter Check
        if (NULL == PpThis || NULL == PpInformationType || NULL == PppInformationBlock || NULL == PpullInformationBlockSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in GetInformation: at least one mandatory parameter is NULL. (0x%.16lX)", efiStatus);
            break;
        }

        // Check for firmware update counters GUID
        if (CompareGuid(PpInformationType, &guidCounters))
        {
            efiStatus = IFXTPMUpdate_AdapterInformation_GetInformationCounters(PppInformationBlock, PpullInformationBlockSize);
            if (EFI_ERROR(efiStatus))
                break;
        }
        // Check for operation mode GUID
        else if (CompareGuid(PpInformationType, &guidOperationMode))
        {
            efiStatus = IFXTPMUpdate_AdapterInformation_GetInformationOperationMode(PppInformationBlock, PpullInformationBlockSize);
            if (EFI_ERROR(efiStatus))
                break;
        }
        // Check for firmware update details GUID
        else if (CompareGuid(PpInformationType, &guidFuDetails))
        {
            efiStatus = IFXTPMUpdate_AdapterInformation_GetInformationFuDetails(PppInformationBlock, PpullInformationBlockSize);
            if (EFI_ERROR(efiStatus))
                break;
        }
        else
        {
            // GetInformation called with unsupported GUID
            efiStatus = EFI_UNSUPPORTED;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in GetInformation: invalid value for PpInformationType. (0x%.16lX)", efiStatus);
            break;
        }
    }
    WHILE_FALSE_END;

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_ADAPTER_INFORMATION.GetInformation(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Sets information that will be stored by the Infineon TPM Firmware Update Driver for further use by Firmware Management Protocols methods.
 *  @details    This function sends information of type PpInformationType for an adapter. The adapter supports the following information types:
 *              <table>
 *              <tr><th>Information Type</th><th>Description</th></tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID</td>
 *              <td>Use the information type to turn on logging functionality. The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1 structure to configure
 *              a logging callback function.</td>
 *              </tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID</td>
 *              <td>Use the information type to set the TPM1.2 Owner Authorization for TPM Firmware Update if the TPM has an owner. Do not use the information type if the TPM is a TPM2.0 or if the TPM is a TPM1.2 and does not have an owner.
 *              The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1 structure to set the owner authorization hash in the Infineon TPM Firmware Update Driver.</td>
 *              </tr>
 *              <tr>
 *              <td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID</td>
 *              <td>Use the information type to set the authorized policy session handle for TPM2.0. Do not use the information type if the TPM is a TPM1.2.
 *              The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1 structure and either set the policy session handle for an authorized policy session in TPM2.0
 *              or set policy session handle to 0 to make IFXTPMUpdate.efi create a default policy session.</td>
 *              </tr>
 *              </table>
 *              Otherwise EFI_UNSUPPORTED is returned.
 *
 *  @param      PpThis                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *  @param      PpInformationType           A pointer to an EFI_GUID that defines the contents of PpInformationBlock.
 *  @param      PpInformationBlock          A pointer to the PpInformationBlock structure which contains details about the data specific to PpInformationType.
 *  @param      PullInformationBlockSize    The size of the PpInformationBlock in bytes.
 *
 *  @retval     EFI_SUCCESS                                     The information was received and interpreted successfully.
 *  @retval     EFI_DEVICE_ERROR                                An unexpected error occurred.
 *  @retval     EFI_INVALID_PARAMETER                           In case of an invalid input parameter.
 *  @retval     EFI_UNSUPPORTED                                 The PpInformationType is not known.
 *  @retval     EFI_IFXTPM_TPM12_DA_ACTIVE                      The TPM Owner is locked out due to dictionary attack\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *  @retval     EFI_IFXTPM_TPM12_INVALID_OWNERAUTH              TPM Owner authentication is incorrect\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *  @retval     EFI_IFXTPM_TPM12_NO_OWNER                       The TPM does not have an owner but TPM Owner authorization was provided\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *  @retval     EFI_IFXTPM_TPM20_FAILURE_MODE                   The TPM2.0 is in failure mode.
 *  @retval     EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER   The platformAuth is not the Empty Buffer. In this case you cannot use the default policy.
 *  @retval     EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED       The policy session handle is not loaded in the TPM\n (The error code only applies to TPM2.0 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID).
 *  @retval     EFI_IFXTPM_UNSUPPORTED_CHIP                     The Infineon TPM chip detected is not supported by the driver.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR                   The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_SetInformation(
    IN  EFI_ADAPTER_INFORMATION_PROTOCOL*   PpThis,
    IN  EFI_GUID*                           PpInformationType,
    IN  VOID*                               PpInformationBlock,
    IN  UINTN                               PullInformationBlockSize)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;
    LOGGING_WRITE_LEVEL2(L"Entering EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation()");

    do
    {
        const EFI_GUID guidLogging = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID;
        const EFI_GUID guidTpm12 = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID;
        const EFI_GUID guidTpm20 = EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID;

        // Parameter Check
        if (NULL == PpThis || NULL == PpInformationBlock || NULL == PpInformationType)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetInformation: at least one mandatory parameter is NULL. (0x%.16lX)", efiStatus);
            break;
        }

        // Check for logging structure GUID
        if (CompareGuid(PpInformationType, &guidLogging))
        {
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1* pDescriptor = NULL;
            if (PullInformationBlockSize != sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1))
            {
                efiStatus = EFI_INVALID_PARAMETER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetInformation: invalid value for PullInformationBlockSize. (0x%.16lX)", efiStatus);
                break;
            }
            pDescriptor = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1*)PpInformationBlock;
            g_pPrivateData->pfnLogCallback = pDescriptor->LogCallback;
            g_pPrivateData->fLogTimeStamps = pDescriptor->AddTimeStamps;
        }
        // Check for TPM1.2 structure GUID
        else if (CompareGuid(PpInformationType, &guidTpm12))
        {
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1* pDescriptor = NULL;
            unsigned int unReturnValue = RC_E_FAIL;
            TPM_STATE sTpmState;
            Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));

            if (PullInformationBlockSize != sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1))
            {
                efiStatus = EFI_INVALID_PARAMETER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetInformation: invalid value for PullInformationBlockSize. (0x%.16lX)", efiStatus);
                break;
            }

            // Try to initialize TPM access
            efiStatus = InitializeTpmAccess();
            if (EFI_ERROR(efiStatus))
                break;

            // Get TPM operation mode
            unReturnValue = FirmwareUpdate_CalculateState(TRUE, &sTpmState);
            if (RC_SUCCESS != unReturnValue)
            {
                efiStatus = EFI_DEVICE_ERROR;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during determination of TPM operation mode. (0x%.16lX)", efiStatus);
                break;
            }

            // Check if the recognized TPM is from an unsupported vendor and return the appropriate value.
            if (!sTpmState.attribs.infineon)
            {
                efiStatus = EFI_IFXTPM_UNSUPPORTED_VENDOR;
                LOGGING_WRITE_LEVEL1_FMT(L"Error:The TPM is not manufactured by Infineon. It is not supported by the driver. (0x%.16lX)", efiStatus);
                break;
            }

            // Check if the recognized TPM is an unsupported chip and return the appropriate value.
            if (sTpmState.attribs.unsupportedChip)
            {
                efiStatus = EFI_IFXTPM_UNSUPPORTED_CHIP;
                LOGGING_WRITE_LEVEL1_FMT(L"Error: The Infineon TPM chip detected is not supported by the driver. (0x%.16lX)", efiStatus);
                break;
            }

            // If this is a TPM2.0 then setting a TPM1.2 Owner authentication hash does not make sense.
            if (sTpmState.attribs.tpm20)
            {
                efiStatus = EFI_INVALID_PARAMETER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error: TPM1.2 Owner authentication hash cannot be used with TPM2.0. (0x%.16lX)", efiStatus);
                break;
            }

            // If this is a TPM1.2 without an owner then setting a TPM1.2 Owner authentication hash does not make sense.
            if (sTpmState.attribs.tpm12 && !sTpmState.attribs.tpm12owner)
            {
                efiStatus = EFI_IFXTPM_TPM12_NO_OWNER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error: TPM1.2 does not have an owner but TPM Owner authorization was provided. (0x%.16lX)", efiStatus);
                break;
            }

            // Get dictionary attack state for TPM_ET_OWNER and return RC_E_TPM12_DA_ACTIVE if TPM Owner is locked out.
            {
                UINT16 usSubCap = TSS_TPM_ET_OWNER;
                TSS_TPM_DA_INFO sDaInfo;
                unsigned int unDaInfoSize = sizeof(TSS_TPM_DA_INFO);
                Platform_MemorySet(&sDaInfo, 0, sizeof(sDaInfo));
                usSubCap = Platform_SwapBytes16(usSubCap);
                unReturnValue = TSS_TPM_GetCapability(TSS_TPM_CAP_DA_LOGIC, sizeof(usSubCap), (BYTE*)&usSubCap, &unDaInfoSize, (BYTE*)&sDaInfo);
                if (RC_SUCCESS != unReturnValue)
                {
                    efiStatus = EFI_DEVICE_ERROR;
                    LOGGING_WRITE_LEVEL1_FMT(L"Error: Dictionary Attack check failed. (0x%.8X)", unReturnValue);
                    break;
                }

                if (TSS_TPM_DA_STATE_ACTIVE == sDaInfo.state)
                {
                    efiStatus = EFI_IFXTPM_TPM12_DA_ACTIVE;
                    LOGGING_WRITE_LEVEL1_FMT(L"Error: TPM1.2 is in a dictionary attack mode. (0x%.16lX)", efiStatus);
                    break;
                }
            }

            // Check TPM Owner authorization (except for disabled/deactivated TPM).
            pDescriptor = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1*)PpInformationBlock;
            unReturnValue = FirmwareUpdate_CheckOwnerAuthorization(pDescriptor->OwnerPasswordSha1);
            if (RC_SUCCESS != unReturnValue)
            {
                if ((TSS_TPM_DEACTIVATED == (unReturnValue ^ RC_TPM_MASK)) || (TSS_TPM_DISABLED == (unReturnValue ^ RC_TPM_MASK)))
                {
                    // In disabled and/or deactivated state TPM Owner authorization may not be checkable prior to first TPM_FieldUpgrade request
                }
                else
                {
                    if (TSS_TPM_AUTHFAIL == (unReturnValue ^ RC_TPM_MASK))
                    {
                        efiStatus = EFI_IFXTPM_TPM12_INVALID_OWNERAUTH;
                        LOGGING_WRITE_LEVEL1_FMT(L"Error: TPM1.2 Owner authentication hash is incorrect. (0x%.16lX)", efiStatus);
                    }
                    else
                    {
                        efiStatus = EFI_DEVICE_ERROR;
                        LOGGING_WRITE_LEVEL1_FMT(L"Error: TPM1.2 Owner authorization check failed. (0x%.8X)", unReturnValue);
                    }
                    break;
                }
            }

            // Copy TPM Owner authentication hash
            CopyMem(g_pPrivateData->rgbOwnerPasswordSha1, pDescriptor->OwnerPasswordSha1, sizeof(pDescriptor->OwnerPasswordSha1));
            g_pPrivateData->fOwnedUpdate = TRUE;
        }
        // Check for TPM2.0 structure GUID
        else if (CompareGuid(PpInformationType, &guidTpm20))
        {
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1* pDescriptor = NULL;
            unsigned int unReturnValue = RC_E_FAIL;
            TPM_STATE sTpmState;
            Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));

            if (PullInformationBlockSize != sizeof(EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1))
            {
                efiStatus = EFI_INVALID_PARAMETER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetInformation: invalid value for PullInformationBlockSize. (0x%.16lX)", efiStatus);
                break;
            }

            pDescriptor = (EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1*)PpInformationBlock;

            // Initialize TPM in case it has not been done before to get the type of the plugged TPM
            efiStatus = InitializeTpmAccess();
            if (EFI_ERROR(efiStatus))
                break;

            // Get TPM operation mode
            unReturnValue = FirmwareUpdate_CalculateState(TRUE, &sTpmState);
            if (RC_SUCCESS != unReturnValue)
            {
                efiStatus = EFI_DEVICE_ERROR;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during determination of TPM operation mode. (0x%.16lX)", efiStatus);
                break;
            }

            // Check if the recognized TPM is from an unsupported vendor and return the appropriate value.
            if (!sTpmState.attribs.infineon)
            {
                efiStatus = EFI_IFXTPM_UNSUPPORTED_VENDOR;
                LOGGING_WRITE_LEVEL1_FMT(L"Error: The TPM is not manufactured by Infineon. It is not supported by the driver. (0x%.16lX)", efiStatus);
                break;
            }

            // Check if the recognized TPM is an unsupported chip and return the appropriate value.
            if (sTpmState.attribs.unsupportedChip)
            {
                efiStatus = EFI_IFXTPM_UNSUPPORTED_CHIP;
                LOGGING_WRITE_LEVEL1_FMT(L"Error: The Infineon TPM chip detected is not supported by the driver. (0x%.16lX)", efiStatus);
                break;
            }

            // If this is a TPM2.0 in firmware mode then policy session handle must be set. Otherwise it must not be set.
            if (sTpmState.attribs.tpm20)
            {
                if (sTpmState.attribs.tpm20InFailureMode)
                {
                    efiStatus = EFI_IFXTPM_TPM20_FAILURE_MODE;
                    LOGGING_WRITE_LEVEL1_FMT(L"Error: The TPM2.0 is in failure mode. (0x%.16lX)", efiStatus);
                    break;
                }

                if (pDescriptor->SessionHandle == 0)
                {
                    // IFXTPMUpdate.efi to use Empty Buffer as platformAuth and auto-generate the policy session
                    if (!sTpmState.attribs.tpm20emptyPlatformAuth)
                    {
                        efiStatus = EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER;
                        LOGGING_WRITE_LEVEL1_FMT(L"Error: policy session is not loaded. (0x%.16lX)", efiStatus);
                        break;
                    }
                }
                else
                {
                    // Read out the list of loaded session handles from the TPM.
                    TSS_TPMS_CAPABILITY_DATA handleCapabilityData;
                    BYTE bMoreData = 0;
                    unsigned int unIndex = 0;
                    BOOL fFound = FALSE;
                    Platform_MemorySet(&handleCapabilityData, 0, sizeof(handleCapabilityData));

                    unReturnValue = TSS_TPM2_GetCapability(TSS_TPM_CAP_HANDLES, TSS_TPM_HT_LOADED_SESSION << 24, 256, &bMoreData, &handleCapabilityData);
                    if (RC_SUCCESS == unReturnValue)
                    {
                        unsigned int unCountofHandles = handleCapabilityData.data.handles.count;
                        for (unIndex = 0; unIndex < unCountofHandles; unIndex++)
                        {
                            // Check whether the given SessionHandle is in the list
                            if (pDescriptor->SessionHandle == handleCapabilityData.data.handles.handle[unIndex])
                                fFound = TRUE;
                        }

                        if (!fFound)
                        {
                            efiStatus = EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED;
                            LOGGING_WRITE_LEVEL1_FMT(L"Error: policy session is not loaded. (0x%.16lX)", efiStatus);
                            break;
                        }
                    }
                    else
                    {
                        LOGGING_WRITE_LEVEL1_FMT(L"TSS_TPM2_GetCapability returned an unexpected value (0x%.16lX). (TPM_CAP_HANDLES,TPM_HT_LOADED_SESSION)", unReturnValue);
                        break;
                    }
                }
            }
            else
            {
                if (pDescriptor->SessionHandle != 0)
                {
                    efiStatus = EFI_INVALID_PARAMETER;
                    LOGGING_WRITE_LEVEL1_FMT(L"Error: policy session handle is not required and must be set to 0. (0x%.16lX)", efiStatus);
                    break;
                }
            }

            g_pPrivateData->unSessionHandle = pDescriptor->SessionHandle;
        }
        else
        {
            // SetInformation only supports EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID and EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID
            efiStatus = EFI_UNSUPPORTED;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetInformation: invalid value for PpInformationType. (0x%.16lX)", efiStatus);
            break;
        }

        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Get a list of supported information types for the Infineon TPM Firmware Update Driver.
 *  @details    This function returns a list of InformationType GUIDs that are supported on Infineon TPM Firmware Update Driver with this
 *              instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned in PppInfoTypesBuffer, and the number
 *              of GUID pointers in PppInfoTypesBuffer is returned in PpullInfoTypesBufferCount. The method returns
 *              the following information types:
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1_GUID
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1_GUID
 *              * @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1_GUID
 *
 *  @param      PpThis                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *  @param      PppInfoTypesBuffer          A pointer to the array of InformationType GUIDs that are supported by PpThis.
 *  @param      PpullInfoTypesBufferCount   A pointer to the number of GUIDs present in InfoTypesBuffer. *PpullInfoTypesBufferCount will be set to 3.
 *
 *  @retval     EFI_SUCCESS                 The list of information type GUIDs that are supported on this adapter was returned in PppInfoTypesBuffer.
 *                                          The number of information type GUIDs was returned in PpullInfoTypesBufferCount.
 *  @retval     EFI_INVALID_PARAMETER       In case of an invalid or null parameter.
 *  @retval     EFI_OUT_OF_RESOURCES        There is not enough pool memory to store the results.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetSupportedTypes(
    IN  EFI_ADAPTER_INFORMATION_PROTOCOL*   PpThis,
    OUT EFI_GUID**                          PppInfoTypesBuffer,
    OUT UINTN*                              PpullInfoTypesBufferCount)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;

    LOGGING_WRITE_LEVEL2(L"Entering EFI_ADAPTER_INFORMATION_PROTOCOL.GetSupportedTypes()");
    do
    {
        const EFI_GUID guids[] = {
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID,
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID,
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID,
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_COUNTERS_1_GUID,
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_OPERATION_MODE_1_GUID,
            EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_DETAILS_1_GUID
        };

        // Check parameters
        if (NULL == PpThis || NULL == PppInfoTypesBuffer || NULL == PpullInfoTypesBufferCount)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in GetSupportedTypes: at least one mandatory parameter is NULL. (0x%.16lX)", efiStatus);
            break;
        }
        // Initialize out parameters
        *PpullInfoTypesBufferCount = 0;
        *PppInfoTypesBuffer = NULL;

        // Allocate memory for the types buffer
        *PppInfoTypesBuffer = (EFI_GUID*)AllocateZeroPool(sizeof(guids));
        if (NULL == *PppInfoTypesBuffer)
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during memory allocation for PppInfoTypesBuffer in GetSupportedTypes. (0x%.16lX)", efiStatus);
            break;
        }
        // Set actual types count
        *PpullInfoTypesBufferCount = sizeof(guids) / sizeof(EFI_GUID);
        // Copy type information to the buffer
        CopyMem(*PppInfoTypesBuffer, guids, sizeof(guids));
    }
    WHILE_FALSE_END;

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_ADAPTER_INFORMATION_PROTOCOL.GetSupportedTypes(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/// @cond SHOW_INTERNAL
/**
 */
EFI_ADAPTER_INFORMATION_PROTOCOL g_IFXTPMUpdateAdapterInformation = {
    IFXTPMUpdate_AdapterInformation_GetInformation,
    IFXTPMUpdate_AdapterInformation_SetInformation,
    IFXTPMUpdate_AdapterInformation_GetSupportedTypes
};
/// @endcond
