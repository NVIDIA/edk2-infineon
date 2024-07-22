/**
 *  @brief      Implements the EFI_FIRMWARE_MANAGEMENT_PROTOCOL
 *  @details    Implements the functions of EFI_FIRMWARE_MANAGEMENT_PROTOCOL in the Infineon TPM Firmware Update Driver.
 *              @cond !SHOW_INTERNAL @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification" contains more details on EFI_FIRMWARE_MANAGEMENT_PROTOCOL.
 *              Functions are prefixed with @a IFXTPMUpdate_FirmwareManagement_ in this document but not in @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification".
 *              @endcond
 *  @file       FirmwareManagement.c
 *
 *  Copyright 2014 - 2022 Infineon Technologies AG ( www.infineon.com )
 *  SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "StdInclude.h"
#include "Crypt.h"
#include "DeviceManagement.h"
#include "FirmwareManagement.h"
#include "FirmwareUpdate.h"
#include "IFXTPMUpdate.h"
#include "IFXTPMUpdateApp.h"
#include "TPM_Types.h"

#include <Library/DisplayUpdateProgressLib.h>

/**
  Function indicate the current completion progress of the firmware
  update. Platform may override with own specific progress function.

  @param[in]  Completion  A value between 1 and 100 indicating the current
                          completion progress of the firmware update

  @retval EFI_SUCESS             The capsule update progress was updated.
  @retval EFI_INVALID_PARAMETER  Completion is greater than 100%.

**/
EFI_STATUS
EFIAPI
UpdateImageProgress (
  IN UINTN  Completion
  )
{
  EFI_STATUS                           Status;
  UINTN                                Seconds;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION  *Color;

  DEBUG ((DEBUG_INFO, "Update Progress - %llu%%\n", Completion));

  if (Completion > 100) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Use a default timeout of 5 minutes
  //
  Seconds = 5 * 60;
  Color   = NULL;

  //
  // Cancel the watchdog timer
  //
  gBS->SetWatchdogTimer (0, 0x0000, 0, NULL);

  if (Completion != 100) {
    //
    // Arm the watchdog timer from PCD setting
    //
    if (Seconds != 0) {
      DEBUG ((DEBUG_VERBOSE, "Arm watchdog timer %llu seconds\n", Seconds));
      gBS->SetWatchdogTimer (Seconds, 0x0000, 0, NULL);
    }
  }

  Status = DisplayUpdateProgress (Completion, Color);

  return Status;
}

/// Predefined image version
CHAR16 gwszVersionName[MAX_NAME];

/**
 *  @brief      Returns information about the current firmware image of the TPM.
 *  @details    The function returns a EFI_FIRMWARE_IMAGE_DESCRIPTOR in PpImageInfo which contains information about the current firmware image on the TPM.
 *
 *  @param      PpThis                      A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
 *  @param      PpullImageInfoSize          A pointer to the size, in bytes, of the PpImageInfo buffer.
 *  @param      PpImageInfo                 A pointer to the buffer in which firmware places the current image information. IFXTPMUpdate returns 1 EFI_FIRMWARE_IMAGE_DESCRIPTOR.
 *                                          The relevant parameters are:
 *                                          - ImageTypeId = EFI_IFXTPM_FIRMWARE_TYPE_GUID
 *                                          - VersionName = TPM version information as Unicode string. The pattern is (Major).(Minor).(SubversionMinor) and an example value would be 4.40.119.0. The string is allocated statically by IFXTPMUpdate, memory will be invalidated at next call to @ref IFXTPMUpdate_FirmwareManagement_GetImageInfo or at unload of IFXTPMUpdate. Memory does not need to be freed by the caller.
 *                                          - AttributesSupported = @ref IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE | @ref IMAGE_ATTRIBUTE_IFXTPM_HAS_OWNER | @ref IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE | @ref IMAGE_ATTRIBUTE_IFXTPM_NON_OPERATIONAL_MODE | @ref IMAGE_ATTRIBUTE_IFXTPM_2_0 | @ref IMAGE_ATTRIBUTE_IFXTPM_1_2 | @ref IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED | @ref IMAGE_ATTRIBUTE_IFXTPM_DEFERREDPP | IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE
 *                                          - AttributesSetting:
 *                                              The AttributesSetting can be split into three different sections:\n
 *                                              Section 1 - Attributes to describe the currently active TPM firmware (@ref IMAGE_ATTRIBUTE_IFXTPM_1_2, @ref IMAGE_ATTRIBUTE_IFXTPM_2_0 and @ref IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE are mutually exclusive)
 *                                              <table>
 *                                              <tr><th>Value</th><th>Description</th></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_1_2</td><td>The TPM is a TPM1.2.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_2_0</td><td>The TPM is a TPM2.0.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE</td><td>The TPM is in invalid firmware mode.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_NON_OPERATIONAL_MODE</td><td>The TPM is in non-operational mode (only SLB 9672).</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_HAS_OWNER</td><td>The TPM1.2 has an owner.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_DEFERREDPP</td><td>The TPM1.2 has asserted Deferred Physical Presence.</td></tr>
 *                                              </table>\n
 *                                              Section 2 - Attributes to describe whether the currently active TPM firmware can be updated, If no attributes in this section are set, the TPM firmware cannot be updated:
 *                                              <table>
 *                                              <tr><th>Value</th><th>Description</th></tr>
 *                                              <tr><td>IMAGE_ATTRIBUTE_IMAGE_UPDATABLE</td><td>The TPM can be updated two or more times.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE | IMAGE_ATTRIBUTE_IMAGE_UPDATABLE</td><td>The TPM can be updated one more time.</td></tr>
 *                                              <tr><td>@ref IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED</td><td>Due to the current TPM state the attributes @ref IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE and IMAGE_ATTRIBUTE_IMAGE_UPDATABLE cannot be determined. A TPM restart is required to read these attributes.</td></tr>
 *                                              </table>\n
 *                                              Section 3 - UEFI specific attributes that are always present:
 *                                              <table>
 *                                              <tr><th>Value</th><th>Description</th></tr>
 *                                              <tr><td>IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE</td><td>The currently active TPM firmware image is in use. After updating the TPM firmware a reset is required so that the new firmware becomes active.</td></tr>
 *                                              </table>\n
 *                                          Further parameters (required to be compliant with EFI_FIRMWARE_MANAGEMENT_PROTOCOL):
 *                                          - ImageIndex = 1
 *                                          - ImageId = 0
 *                                          - ImageIdName = NULL
 *                                          - Version = 0
 *                                          - Size = 0
 *                                          - Compatibilities = 0
 *                                          - LowestSupportedImageVersion = 0
 *  @param      PpunDescriptorVersion       A pointer to the location in which firmware returns the version number associated with the EFI_FIRMWARE_IMAGE_DESCRIPTOR. IFXTPMUpdate returns EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION (2).
 *  @param      PpbDescriptorCount          A pointer to the location in which firmware returns the number of descriptors or firmware images within this device. IFXTPMUpdate returns 1.
 *  @param      PpullDescriptorSize         A pointer to the location in which firmware returns the size, in bytes, of an individual EFI_FIRMWARE_IMAGE_DESCRIPTOR. IFXTPMUpdate returns sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR).
 *  @param      PpunPackageVersion          A version number that represents all the firmware images in the device. The format is vendor specific. IFXTPMUpdate returns 0xFFFFFFFF (not supported).
 *  @param      PppPackageVersionName       A pointer to a null-terminated string representing the package version name. IFXTPMUpdate returns NULL.
 *
 *  @retval     EFI_SUCCESS                         The operation completed successfully.
 *  @retval     EFI_BUFFER_TOO_SMALL                The ImageInfo buffer was too small. The current buffer size needed to hold the image(s) information is returned in PpImageInfoSize.
 *  @retval     EFI_DEVICE_ERROR                    Valid information could not be returned. This error may occur if the communication with the TPM failed.
 *  @retval     EFI_INVALID_PARAMETER               PpullImageInfoSize is NULL.
 *  @retval     EFI_IFXTPM_TPM12_DEACTIVATED        The TPM is deactivated. It needs to be activated to retrieve TPM firmware information (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DISABLED           The TPM is disabled. It needs to be enabled to retrieve TPM firmware information (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_FAILED_SELFTEST    The TPM1.2 failed the self-test.
 *  @retval     EFI_IFXTPM_TPM20_FAILURE_MODE       The TPM2.0 is in failure mode.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_CHIP         The Infineon TPM chip detected is not supported by the driver.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR       The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_GetImageInfo(
    IN      EFI_FIRMWARE_MANAGEMENT_PROTOCOL*   PpThis,
    IN OUT  UINTN*                              PpullImageInfoSize,
    IN OUT  EFI_FIRMWARE_IMAGE_DESCRIPTOR*      PpImageInfo,
    OUT     UINT32*                             PpunDescriptorVersion,
    OUT     UINT8*                              PpbDescriptorCount,
    OUT     UINTN*                              PpullDescriptorSize,
    OUT     UINT32*                             PpunPackageVersion,
    OUT     CHAR16**                            PppPackageVersionName)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;
    LOGGING_WRITE_LEVEL2(L"Entering EFI_FIRMWARE_MANAGEMENT_PROTOCOL.GetImageInfo()");

    do
    {
        unsigned int unCapacity = RG_LEN(gwszVersionName);
        unsigned int unRemainingUpdates = REMAINING_UPDATES_UNAVAILABLE; // -1
        unsigned int unRemainingUpdatesSelf = REMAINING_UPDATES_UNAVAILABLE; // -1
        unsigned long long ullAttributesSetting = 0;

        // Check parameters
        if (NULL == PpThis || NULL == PpullImageInfoSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input first parameter check in GetImageInfo: at least one mandatory parameter is NULL. (0x%.16lX)", efiStatus);
            break;
        }

        // The function can be asked for the needed output structure size. In this case PpImageInfo may be NULL and PpullImageInfoSize points to a size
        // that is smaller then the used return structure. The function returns EFI_BUFFER_TOO_SMALL and puts the required size in PpullImageInfoSize.
        if (*PpullImageInfoSize < sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR))
        {
            // This is not a real error, since the caller just wants to obtain the output structure size as defined
            *PpullImageInfoSize = sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR);
            efiStatus = EFI_BUFFER_TOO_SMALL;
            break;
        }

        // If *PpullImageInfoSize is large enough then there must be a pointer in PpImageInfo.
        if (NULL == PpImageInfo)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during parameter check in GetImageInfo: PpImageInfo is NULL. (0x%.16lX)", efiStatus);
            break;
        }

        // Check the rest of the parameters
        if (NULL == PpunDescriptorVersion || NULL == PpbDescriptorCount || NULL == PpullDescriptorSize ||
                NULL == PpunPackageVersion || NULL == PppPackageVersionName)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input third parameter check in GetImageInfo: at least one mandatory parameter is NULL. (0x%.16lX)", efiStatus);
            break;
        }

        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        // Get Image Info
        {
            TPM_STATE sTpmState;
            unsigned int unReturnValue = RC_E_FAIL;
            Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));

            unReturnValue = FirmwareUpdate_GetImageInfo(gwszVersionName, &unCapacity, &sTpmState, &unRemainingUpdates);
            if (RC_SUCCESS != unReturnValue)
            {
                switch (unReturnValue)
                {
                    case RC_E_NO_IFX_TPM:
                        efiStatus = EFI_IFXTPM_UNSUPPORTED_VENDOR;
                        break;
                    case RC_E_UNSUPPORTED_CHIP:
                        efiStatus = EFI_IFXTPM_UNSUPPORTED_CHIP;
                        break;
                    case RC_E_BAD_PARAMETER:
                        efiStatus = EFI_INVALID_PARAMETER;
                        break;
                    case RC_TPM_MASK|TSS_TPM_DEACTIVATED:
                        efiStatus = EFI_IFXTPM_TPM12_DEACTIVATED;
                        break;
                    case RC_TPM_MASK|TSS_TPM_DISABLED:
                        efiStatus = EFI_IFXTPM_TPM12_DISABLED;
                        break;
                    default:
                        efiStatus = EFI_DEVICE_ERROR;
                        break;
                }
                if (RC_E_UNSUPPORTED_CHIP != unReturnValue)
                    break;
            }
            else if (sTpmState.attribs.tpm20InFailureMode)
            {
                efiStatus = EFI_IFXTPM_TPM20_FAILURE_MODE;
                break;
            }
            else if (sTpmState.attribs.tpm12FailedSelfTest)
            {
                efiStatus = EFI_IFXTPM_TPM12_FAILED_SELFTEST;
                break;
            }
            else
            {
                if(sTpmState.attribs.tpmHasFULoader20)
                {
                    unReturnValue = FirmwareUpdate_GetTpm20FieldUpgradeCounterSelf(&unRemainingUpdatesSelf);
                    if (RC_SUCCESS != unReturnValue)
                    {
                        efiStatus = EFI_DEVICE_ERROR;
                        break;
                    }
                }

                ullAttributesSetting |= sTpmState.attribs.tpm12 ? IMAGE_ATTRIBUTE_IFXTPM_1_2 : 0;
                ullAttributesSetting |= sTpmState.attribs.tpm20 ? IMAGE_ATTRIBUTE_IFXTPM_2_0 : 0;
                ullAttributesSetting |= sTpmState.attribs.tpm20restartRequired ? IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED : 0;
                ullAttributesSetting |= sTpmState.attribs.tpm12owner ? IMAGE_ATTRIBUTE_IFXTPM_HAS_OWNER : 0;
                ullAttributesSetting |= sTpmState.attribs.tpm12DeferredPhysicalPresence ? IMAGE_ATTRIBUTE_IFXTPM_DEFERREDPP : 0;
                if (sTpmState.attribs.tpmHasFULoader20)
                    ullAttributesSetting |= !sTpmState.attribs.tpmInOperationalMode ? IMAGE_ATTRIBUTE_IFXTPM_NON_OPERATIONAL_MODE : 0;
                else
                    ullAttributesSetting |= !sTpmState.attribs.tpmFirmwareIsValid ? IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE : 0;
            }
        }

        // Set attributes according to number of remaining updates
        ullAttributesSetting |= IMAGE_ATTRIBUTE_IN_USE | IMAGE_ATTRIBUTE_RESET_REQUIRED;

        // Get effective counter value (no more updates are possible if one of the counter is zero)
        UINT32 unEffectiveCounter = MIN(unRemainingUpdates, unRemainingUpdatesSelf);
        switch (unEffectiveCounter)
        {
            case 0:
                // No more updates possible
                break;
            case 1:
                // 1 update possible
                ullAttributesSetting |= IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE;
                break;
            case REMAINING_UPDATES_UNAVAILABLE: // -1
                // counter not available, restart required
                ullAttributesSetting |= IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED;
                break;
            default:
                // More than 1 update possible
                ullAttributesSetting |= IMAGE_ATTRIBUTE_IMAGE_UPDATABLE;
                break;
        }

        // Fill out parameters
        *PpullImageInfoSize = sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR);
        {
            const EFI_GUID guid = EFI_IFXTPM_FIRMWARE_TYPE_GUID;

            // Must be set to 1
            PpImageInfo->ImageIndex = 1;
            // Must be set to EFI_IFXTPM_FIRMWARE_TYPE_GUID
            CopyGuid(&PpImageInfo->ImageTypeId, &guid);
            // Must be set to 0
            PpImageInfo->ImageId = 0;
            // Must be set to NULL
            PpImageInfo->ImageIdName = (CHAR16*) NULL;
            // Must be set to 1
            PpImageInfo->Version = 0;
            // A pointer to a null-terminated string representing the firmware image version name.
            PpImageInfo->VersionName = gwszVersionName;
            // Size of the image in bytes. If size=0, then only ImageIndex and ImageTypeId are valid.
            PpImageInfo->Size = 0;
            // Must include all supported attributes
            PpImageInfo->AttributesSupported = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE | IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE |
                                               IMAGE_ATTRIBUTE_IFXTPM_HAS_OWNER | IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE | IMAGE_ATTRIBUTE_IFXTPM_2_0 | IMAGE_ATTRIBUTE_IFXTPM_1_2 | IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED |
                                               IMAGE_ATTRIBUTE_IFXTPM_DEFERREDPP;
            PpImageInfo->AttributesSetting = ullAttributesSetting;
            // Must be set to IMAGE_COMPATIBILITY_CHECK_SUPPORTED
            PpImageInfo->Compatibilities = IMAGE_COMPATIBILITY_CHECK_SUPPORTED;
            // Must be set to 0. Describes the lowest ImageDescriptor version that the device will accept. Only present in version 2 or higher.
            PpImageInfo->LowestSupportedImageVersion = 0;
            // Must be set to EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION
            *PpunDescriptorVersion = EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION;
            // Must be set to 1
            *PpbDescriptorCount = 1;
            // Must be set to sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR)
            *PpullDescriptorSize = *PpullImageInfoSize;
            // PackageVersion/Name is not supported. Must be set to 0xFFFFFFFF
            *PpunPackageVersion = 0xFFFFFFFF;
            // Must be set to NULL
            *PppPackageVersionName = (CHAR16*) NULL;
        }
        if (EFI_IFXTPM_UNSUPPORTED_CHIP != efiStatus)
            efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_FIRMWARE_MANAGEMENT_PROTOCOL.GetImageInfo(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      The function is not supported.
 *  @details
 *
 *  @param      PpThis                      -.
 *  @param      PbImageIndex                -.
 *  @param      PpImage                     -.
 *  @param      PpullImageSize              -.
 *  @retval     EFI_UNSUPPORTED             The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_GetImage(
    IN      EFI_FIRMWARE_MANAGEMENT_PROTOCOL*   PpThis,
    IN      UINT8                               PbImageIndex,
    IN OUT  VOID*                               PpImage,
    IN OUT  UINTN*                              PpullImageSize)
{
    return EFI_UNSUPPORTED;
}

/**
 *  @brief      Checks if the firmware image is valid for the TPM (internal).
 *  @details    Internal function to perform integrity, consistency and content checks to determine if the given firmware image can be applied to the installed TPM.
 *
 *  @param      PpImage                 A pointer to the binary contents of a firmware image file.
 *                                      @cond !SHOW_INTERNAL @ref FirmwareImages describes the structure of this file. @endcond
 *  @param      PullImageSize           Size of the firmware image file in bytes.
 *  @param      PpunImageUpdatable      Optional pointer for returning information if the firmware image is valid for update.
 *                                      <table>
 *                                      <tr><th>Value</th><th>Description</th></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_VALID</td><td>The firmware image is valid.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_SAME_VERSION</td><td>The firmware image is valid and will result in updating to the same firmware version.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_FIRMWARE_RECOVERY</td><td>The firmware image is valid and allows firmware recovery.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_DEVICETYPE_CHANGE | @ref IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS</td><td>The firmware image is valid, will change the TPM device type (TPM1.2 to TPM2.0 or vice versa) and will reset the TPM to factory defaults. All data and state from the old TPM firmware will be lost.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS</td><td>The firmware image is valid and will reset the TPM to factory defaults. All data and state from the old TPM firmware will be lost.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_INVALID</td><td>The TPM does not allow further updates because the update counter is zero.</td></tr>
 *                                      <tr><td>IMAGE_UPDATABLE_INVALID_TYPE</td><td>The firmware image is invalid.</td></tr>
 *                                      <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_INVALID_IMAGE_CORRUPTED</td><td>The firmware image is corrupt.</td></tr>
 *                                      <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_NEWER_DRIVER_REQUIRED</td><td>The given image is not compatible with this version of the driver. A newer version of the driver is required to read the image.</td></tr>
 *                                      <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_NEWER_IMAGE_REVISION_REQUIRED</td><td>A newer revision of the firmware image file is required.</td></tr>
 *                                      </table>
 *
 *  @retval     EFI_SUCCESS                     The image was successfully checked or image information were returned by parameter PpunImageUpdatable.
 *  @retval     EFI_DEVICE_ERROR                The communication with the TPM failed.
 *  @retval     EFI_INVALID_PARAMETER           PpunImageUpdatable was NULL or PullImageSize was 0.
 *  @retval     EFI_IFXTPM_RESTART_REQUIRED     The system must be restarted before the firmware image can be verified.
 *  @retval     EFI_IFXTPM_TPM12_DEACTIVATED    The TPM is deactivated. It needs to be activated to check the firmware image (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DISABLED       The TPM is disabled. It needs to be enabled to check the firmware image (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM20_FAILURE_MODE   The TPM2.0 is in failure mode.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR   The TPM is not manufactured by Infineon. It is not supported by the driver.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_CHIP     The Infineon TPM chip detected is not supported by the driver.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_CheckImageInternal(
    IN  CONST VOID*     PpImage,
    IN        UINTN     PullImageSize,
    OPTIONAL  UINT32*   PpunImageUpdatable)
{
    EFI_STATUS efiStatus = EFI_UNSUPPORTED;
    LOGGING_WRITE_LEVEL2(L"Entering IFXTPMUpdate_FirmwareManagement_CheckImage_Internal()");

    do
    {
        // Check input parameters
        if (NULL == PpImage || 0 == PullImageSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1(L"Error during parameter check");
            break;
        }

        {
            BOOL fValid = FALSE;
            UINT32 unImageUpdatable = 0;
            unsigned int unReturnValue = RC_E_FAIL;
            unsigned int unErrorDetails = RC_E_FAIL;
            BITFIELD_NEW_TPM_FIRMWARE_INFO bfNewTpmFirmwareInfo;
            ZeroMem(&bfNewTpmFirmwareInfo, sizeof(bfNewTpmFirmwareInfo));

            unReturnValue = FirmwareUpdate_CheckImage((BYTE*)PpImage, PullImageSize, &fValid, &bfNewTpmFirmwareInfo, &unErrorDetails);
            if (RC_SUCCESS != unReturnValue)
            {
                switch (unReturnValue)
                {
                    case RC_E_RESTART_REQUIRED:
                        efiStatus = EFI_IFXTPM_RESTART_REQUIRED;
                        break;
                    case RC_E_BAD_PARAMETER:
                        efiStatus = EFI_INVALID_PARAMETER;
                        break;
                    case RC_E_NO_IFX_TPM:
                        efiStatus = EFI_IFXTPM_UNSUPPORTED_VENDOR;
                        break;
                    case RC_E_UNSUPPORTED_CHIP:
                        efiStatus = EFI_IFXTPM_UNSUPPORTED_CHIP;
                        break;
                    case RC_TPM_MASK | TSS_TPM_DEACTIVATED:
                        efiStatus = EFI_IFXTPM_TPM12_DEACTIVATED;
                        break;
                    case RC_TPM_MASK | TSS_TPM_DISABLED:
                        efiStatus = EFI_IFXTPM_TPM12_DISABLED;
                        break;
                    case RC_E_TPM20_FAILURE_MODE:
                        efiStatus = EFI_IFXTPM_TPM20_FAILURE_MODE;
                        break;
                    default:
                        efiStatus = EFI_DEVICE_ERROR;
                        break;
                }
                break;
            }

            // Map error details to EFI_STATUS code
            switch (unErrorDetails)
            {
                case RC_SUCCESS:
                    unImageUpdatable |= bfNewTpmFirmwareInfo.deviceTypeChange ? IMAGE_UPDATABLE_IFXTPM_DEVICETYPE_CHANGE : 0;
                    unImageUpdatable |= bfNewTpmFirmwareInfo.factoryDefaults ? IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS : 0;
                    unImageUpdatable |= bfNewTpmFirmwareInfo.fwUpdateSameVersion ? IMAGE_UPDATABLE_IFXTPM_SAME_VERSION : 0;
                    unImageUpdatable |= bfNewTpmFirmwareInfo.fwRecovery ? IMAGE_UPDATABLE_IFXTPM_FIRMWARE_RECOVERY : 0;
                    unImageUpdatable |= IMAGE_UPDATABLE_VALID;
                    efiStatus = EFI_SUCCESS;
                    break;

                case RC_E_FW_UPDATE_BLOCKED:
                    unImageUpdatable = IMAGE_UPDATABLE_INVALID;
                    efiStatus = EFI_IFXTPM_NO_MORE_UPDATES;
                    LOGGING_WRITE_LEVEL2_FMT(L"Info: TPM does not allow any more firmware updates. (0x%.16lX)", efiStatus);
                    break;

                case RC_E_WRONG_DECRYPT_KEYS:
                case RC_E_WRONG_FW_IMAGE:
                    unImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
                    efiStatus = EFI_IFXTPM_WRONG_FIRMWARE_IMAGE;
                    LOGGING_WRITE_LEVEL2_FMT(L"Info: The provided firmware image is wrong. (0x%.16lX)", efiStatus);
                    break;

                case RC_E_NEWER_TOOL_REQUIRED:
                    unImageUpdatable = IMAGE_UPDATABLE_IFXTPM_NEWER_DRIVER_REQUIRED;
                    efiStatus = EFI_IFXTPM_NEWER_DRIVER_REQUIRED;
                    LOGGING_WRITE_LEVEL2_FMT(L"Info: A newer version of the driver is required to process the firmware image. (0x%.16lX)", efiStatus);
                    break;

                case RC_E_NEWER_FW_IMAGE_REQUIRED:
                    unImageUpdatable = IMAGE_UPDATABLE_IFXTPM_NEWER_IMAGE_REVISION_REQUIRED;
                    efiStatus = EFI_IFXTPM_NEWER_FW_IMAGE_REQUIRED;
                    LOGGING_WRITE_LEVEL2_FMT(L"Info: A newer revision of the firmware image is required. (0x%.16lX)", efiStatus);
                    break;

                case RC_E_CORRUPT_FW_IMAGE:
                default:
                    unImageUpdatable = IMAGE_UPDATABLE_IFXTPM_INVALID_IMAGE_CORRUPTED;
                    efiStatus = EFI_IFXTPM_CORRUPT_FIRMWARE_IMAGE;
                    LOGGING_WRITE_LEVEL2_FMT(L"Info: The provided firmware image is corrupt. (0x%.16lX)", efiStatus);
                    break;
            }

            // Set image updatable flags if pointer is set and return EFI_SUCCESS
            if (PpunImageUpdatable)
            {
                *PpunImageUpdatable = unImageUpdatable;
                efiStatus = EFI_SUCCESS;
            }
        }
    }
    WHILE_FALSE_END;

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting IFXTPMUpdate_FirmwareManagement_CheckImage_Internal: (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Progress callback function for EFI_FIRMWARE_MANGEMENT_PROTOCOL.SetImage function.
 *  @details    This function is used if no ProgressCallback function pointer was set.
 *
 *  @param      PunCompletion   Completion value.
 *  @retval     EFI_SUCCESS     Always succeeds.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_ProgressCallback(
    _In_ UINTN PunCompletion)
{
    return EFI_SUCCESS;
}

/**
 *  @brief      Updates the firmware of the TPM.
 *  @details    This function performs the actual TPM Firmware Update. To ensure successful execution respectively proper error handling, it also performs the firmware image checks of @ref IFXTPMUpdate_FirmwareManagement_CheckImage again prior to updating the firmware.
 *
 *  @param      PpThis                      A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
 *  @param      PbImageIndex                Must be set to 1.
 *  @param      PpImage                     A pointer to the binary contents of a TPM firmware image file or NULL for abandoning an update.
 *  @param      PullImageSize               Size of the TPM firmware image file in bytes or zero for abandoning an update.
 *  @param      PpVendorCode                IFXTPMUpdate does not support vendor-specific firmware image update policy. The pointer must be set to NULL by the caller.
 *  @param      PfnProgress                 A callback function used by the driver to report the progress of the firmware update.
 *                                          The progress value is in range from 0 to 100 and shows the completion percentage of the TPM Firmware Update.
 *  @param      PppAbortReason              A pointer to a null-terminated string providing more details for the aborted operation. IFXTPMUpdate does not support abort reasons and sets the pointer to NULL always.
 *
 *  @retval     EFI_SUCCESS                                     The device was successfully updated with the new image.
 *  @retval     EFI_DEVICE_ERROR                                The communication with the TPM failed.
 *  @retval     EFI_INVALID_PARAMETER                           PpThis or PpImage was NULL or PbImageIndex was != 1 or PullImageSize was 0 or PpVendorCode was != NULL.
 *  @retval     EFI_IFXTPM_CORRUPT_FIRMWARE_IMAGE               The image is corrupt.
 *  @retval     EFI_IFXTPM_FIRMWARE_UPDATE_FAILED               The update operation was started but failed.
 *  @retval     EFI_IFXTPM_NEWER_DRIVER_REQUIRED                A newer version of the driver is required to process the firmware image.
 *  @retval     EFI_IFXTPM_NO_MORE_UPDATES                      The TPM firmware can no longer be updated.
 *  @retval     EFI_IFXTPM_RESTART_REQUIRED                     The system must be restarted before the firmware can be updated.
 *  @retval     EFI_IFXTPM_TPM12_DA_ACTIVE                      The TPM Owner is locked out due to dictionary attack (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DEACTIVATED                    The TPM is deactivated. It needs to be activated for firmware update (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DEFERREDPP_REQUIRED            Deferred Physical Presence has not been set (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DISABLED                       The TPM is disabled. It needs to be enabled for firmware update (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_INVALID_OWNERAUTH              TPM Owner authorization failed (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_MISSING_OWNERAUTH              The TPM has an owner but TPM Owner authorization was not provided (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_NO_OWNER                       The TPM does not have an owner but TPM Owner authorization was provided (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM20_FAILURE_MODE                   The TPM2.0 is in failure mode.
 *  @retval     EFI_IFXTPM_TPM20_INVALID_POLICYSESSION          The policy session handle has not been set or it is invalid (TPM2.0 only).
 *  @retval     EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER   The platformAuth is not the Empty Buffer (TPM2.0 only).
 *  @retval     EFI_IFXTPM_TPM20_PLATFORMHIERARCHY_DISABLED     The platform hierarchy is disabled (TPM2.0 only).
 *  @retval     EFI_IFXTPM_TPM20_POLICY_HANDLE_OUT_OF_RANGE     The policy session handle value is out of range (TPM2.0 only).
 *  @retval     EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED       The policy session is not loaded to the TPM (TPM2.0 only).
 *  @retval     EFI_IFXTPM_UNSUPPORTED_CHIP                     The Infineon TPM chip detected is not supported by the driver.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR                   The TPM is not manufactured by Infineon. It is not supported by the driver.
 *  @retval     EFI_IFXTPM_WRONG_FIRMWARE_IMAGE                 The image cannot be used to update the TPM firmware. The image is either too old or applicable to different TPM firmware only.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_SetImage(
    IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL*               PpThis,
    IN  UINT8                                           PbImageIndex,
    IN  CONST VOID*                                     PpImage,
    IN  UINTN                                           PullImageSize,
    IN  CONST VOID*                                     PpVendorCode,
    IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS   PfnProgress,
    OUT CHAR16**                                        PppAbortReason)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;
    LOGGING_WRITE_LEVEL2(L"Entering EFI_FIRMWARE_MANAGEMENT_PROTOCOL.SetImage()");

    do
    {
        unsigned int unReturnValue = RC_E_FAIL;

        // Check input parameters
        if (NULL == PpThis || 1 != PbImageIndex || NULL != PpVendorCode)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetImage: at least one mandatory parameter is NULL or invalid. (0x%.16lX)", efiStatus);
            break;
        }

#ifdef STRICT_PARAM_CHECK_SETIMAGE
        if (NULL == PfnProgress || NULL == PppAbortReason)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetImage: at least one mandatory parameter is NULL or invalid. (0x%.16lX)", efiStatus);
            break;
        }
#endif

        // Override external callback to use internal update progress instead
        PfnProgress = &UpdateImageProgress;

        // Only set abort reason if pointer is set (no strict parameter enforcement)
        if (NULL != PppAbortReason)
            *PppAbortReason = (CHAR16*)NULL;

        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        // Abort of firmware update or recovery mode requested?
        if (NULL == PpImage && 0 == PullImageSize)
        {
            // Get TPM state
            TPM_STATE sTpmState;
            Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));

            unReturnValue = FirmwareUpdate_CalculateState(TRUE, &sTpmState);
            if (RC_SUCCESS != unReturnValue)
            {
                efiStatus = EFI_DEVICE_ERROR;
                break;
            }

            // TPM in correct state?
            if (sTpmState.attribs.tpmHasFULoader20 &&
                    ((0x01 == sTpmState.attribs.tpm20OperationMode) || (0x81 == sTpmState.attribs.tpm20OperationMode)))
            {
                // If yes, call FieldUpgrade
                unReturnValue = FirmwareUpdate_AbandonUpdate();
                if (RC_SUCCESS != unReturnValue)
                {
                    LOGGING_WRITE_LEVEL2_FMT(L"Unexpected error calling FirmwareUpdate_Abandon: (0x%.8lX)", unReturnValue);
                    efiStatus = EFI_DEVICE_ERROR;
                    break;
                }
                efiStatus = EFI_SUCCESS;
            }
            else
            {
                // Otherwise return invalid parameter
                efiStatus = EFI_INVALID_PARAMETER;
                break;
            }
        }
        else
        {
            // Check additional parameters
            if (NULL == PpImage || 0 == PullImageSize)
            {
                efiStatus = EFI_INVALID_PARAMETER;
                LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in SetImage: at least one mandatory parameter is NULL or invalid. (0x%.16lX)", efiStatus);
                break;
            }

            efiStatus = IFXTPMUpdate_FirmwareManagement_CheckImageInternal((BYTE*)PpImage, PullImageSize, NULL);
            if (EFI_ERROR(efiStatus))
                break;

            if (g_pPrivateData->unSessionHandle == 0)
            {
                // Get TPM state
                TPM_STATE sTpmState;
                Platform_MemorySet(&sTpmState, 0, sizeof(sTpmState));

                unReturnValue = FirmwareUpdate_CalculateState(TRUE, &sTpmState);
                if (RC_SUCCESS != unReturnValue)
                {
                    efiStatus = EFI_DEVICE_ERROR;
                    break;
                }

                // Prepare the default policy session on TPM2.0 (only possible within operational mode)
                if (sTpmState.attribs.tpm20 && sTpmState.attribs.tpmInOperationalMode)
                {
                    unsigned int unPolicySession = 0;
                    unReturnValue = FirmwareUpdate_PrepareTPM20Policy(&unPolicySession);
                    switch (unReturnValue)
                    {
                        case RC_SUCCESS:
                            efiStatus = EFI_SUCCESS;
                            break;
                        case RC_E_PLATFORM_AUTH_NOT_EMPTY:
                            efiStatus = EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER;
                            break;
                        case RC_E_PLATFORM_HIERARCHY_DISABLED:
                            efiStatus = EFI_IFXTPM_TPM20_PLATFORMHIERARCHY_DISABLED;
                            break;
                        default:
                            efiStatus = EFI_DEVICE_ERROR;
                            break;
                    }
                    if (EFI_ERROR(efiStatus))
                        break;

                    g_pPrivateData->unSessionHandle = unPolicySession;
                }
            }

            {
                IfxFirmwareUpdateData sFirmwareUpdateData;
                ZeroMem(&sFirmwareUpdateData, sizeof(sFirmwareUpdateData));
                sFirmwareUpdateData.rgbFirmwareImage = (BYTE*) PpImage;
                sFirmwareUpdateData.unFirmwareImageSize = (unsigned int)PullImageSize;
                sFirmwareUpdateData.fnProgressCallback = (PFN_FIRMWAREUPDATE_PROGRESSCALLBACK) PfnProgress;
                sFirmwareUpdateData.unSessionHandle = g_pPrivateData->unSessionHandle;
                if (g_pPrivateData->fOwnedUpdate)
                {
                    unReturnValue = Platform_MemoryCopy(
                                        sFirmwareUpdateData.rgbOwnerAuthHash,
                                        sizeof(sFirmwareUpdateData.rgbOwnerAuthHash),
                                        g_pPrivateData->rgbOwnerPasswordSha1,
                                        sizeof(g_pPrivateData->rgbOwnerPasswordSha1));
                    if (RC_SUCCESS != unReturnValue)
                        break;
                    sFirmwareUpdateData.fOwnerAuthProvided = TRUE;
                }

                // Call firmware update
                unReturnValue = FirmwareUpdate_UpdateImage(&sFirmwareUpdateData);

                // Evaluate the return code
                switch (unReturnValue)
                {
                    case RC_SUCCESS:
                        efiStatus = EFI_SUCCESS;
                        break;
                    case RC_E_BAD_PARAMETER:
                        efiStatus = EFI_INVALID_PARAMETER;
                        break;
                    case RC_E_TPM20_INVALID_POLICY_SESSION:
                        efiStatus = EFI_IFXTPM_TPM20_INVALID_POLICYSESSION;
                        break;
                    case RC_E_TPM20_POLICY_SESSION_NOT_LOADED:
                        efiStatus = EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED;
                        break;
                    case RC_E_TPM20_POLICY_HANDLE_OUT_OF_RANGE:
                        efiStatus = EFI_IFXTPM_TPM20_POLICY_HANDLE_OUT_OF_RANGE;
                        break;
                    case RC_E_FIRMWARE_UPDATE_FAILED:
                        efiStatus = EFI_IFXTPM_FIRMWARE_UPDATE_FAILED;
                        break;
                    case RC_E_CORRUPT_FW_IMAGE:
                        efiStatus = EFI_IFXTPM_CORRUPT_FIRMWARE_IMAGE;
                        break;
                    case RC_E_TPM12_MISSING_OWNERAUTH:
                        efiStatus = EFI_IFXTPM_TPM12_MISSING_OWNERAUTH;
                        break;
                    case RC_E_TPM12_DA_ACTIVE:
                        efiStatus = EFI_IFXTPM_TPM12_DA_ACTIVE;
                        break;
                    case RC_E_TPM12_INVALID_OWNERAUTH:
                        efiStatus = EFI_IFXTPM_TPM12_INVALID_OWNERAUTH;
                        break;
                    case RC_E_TPM12_DEFERREDPP_REQUIRED:
                        efiStatus = EFI_IFXTPM_TPM12_DEFERREDPP_REQUIRED;
                        break;
                    case RC_E_TPM12_NO_OWNER:
                        efiStatus = EFI_IFXTPM_TPM12_NO_OWNER;
                        break;
                    default:
                        efiStatus = EFI_DEVICE_ERROR;
                        break;
                }
                if (EFI_ERROR(efiStatus))
                    break;
            }

            efiStatus = EFI_SUCCESS;
        }
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_FIRMWARE_MANAGEMENT_PROTOCOL.SetImage(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      Checks if the firmware image is valid for the TPM.
 *  @details    This function performs integrity, consistency and content checks to determine if the given firmware image can be applied to the installed TPM.
 *              It allows firmware update application to validate the firmware image without invoking the SetImage() first.
 *
 *  @param      PpThis                      A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
 *  @param      PbImageIndex                Must be set to 1.
 *  @param      PpImage                     A pointer to the binary contents of a firmware image file.
 *                                          @cond !SHOW_INTERNAL @ref FirmwareImages describes the structure of this file. @endcond
 *  @param      PullImageSize               Size of the firmware image file in bytes.
 *  @param      PpunImageUpdatable          Indicates if the firmware image is valid for update.
 *                                          <table>
 *                                          <tr><th>Value</th><th>Description</th></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_VALID</td><td>The firmware image is valid.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_SAME_VERSION</td><td>The firmware image is valid and will result in updating to the same firmware version.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_FIRMWARE_RECOVERY</td><td>The firmware image is valid and allows firmware recovery.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_DEVICETYPE_CHANGE | @ref IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS</td><td>The firmware image is valid, will change the TPM device type (TPM1.2 to TPM2.0 or vice versa) and will reset the TPM to factory defaults. All data and state from the old TPM firmware will be lost.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_VALID | @ref IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS</td><td>The firmware image is valid and will reset the TPM to factory defaults. All data and state from the old TPM firmware will be lost.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_INVALID</td><td>The TPM does not allow further updates because the update counter is zero.</td></tr>
 *                                          <tr><td>IMAGE_UPDATABLE_INVALID_TYPE</td><td>The firmware image is invalid.</td></tr>
 *                                          <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_INVALID_IMAGE_CORRUPTED</td><td>The firmware image is corrupt.</td></tr>
 *                                          <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_NEWER_DRIVER_REQUIRED</td><td>The given image is not compatible with this version of the driver. A newer version of the driver is required to read the image.</td></tr>
 *                                          <tr><td>@ref IMAGE_UPDATABLE_IFXTPM_NEWER_IMAGE_REVISION_REQUIRED</td><td>A newer revision of the firmware image file is required.</td></tr>
 *                                          </table>
 *
 *  @retval     EFI_SUCCESS                         The image was successfully checked.
 *  @retval     EFI_DEVICE_ERROR                    The communication with the TPM failed.
 *  @retval     EFI_INVALID_PARAMETER               PpThis or PpImage or PpunImageUpdatable was NULL or PbImageIndex was != 1 or PullImageSize was 0.
 *  @retval     EFI_IFXTPM_RESTART_REQUIRED         The system must be restarted before the firmware image can be verified.
 *  @retval     EFI_IFXTPM_TPM12_DEACTIVATED        The TPM is deactivated. It needs to be activated to check the firmware image (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM12_DISABLED           The TPM is disabled. It needs to be enabled to check the firmware image (TPM1.2 only).
 *  @retval     EFI_IFXTPM_TPM20_FAILURE_MODE       The TPM2.0 is in failure mode.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR       The TPM is not manufactured by Infineon. It is not supported by the driver.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_CHIP         The Infineon TPM chip detected is not supported by the driver.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_CheckImage(
    IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL*   PpThis,
    IN  UINT8                               PbImageIndex,
    IN  CONST VOID*                         PpImage,
    IN  UINTN                               PullImageSize,
    OUT UINT32*                             PpunImageUpdatable)
{
    EFI_STATUS efiStatus = EFI_SUCCESS;
    LOGGING_WRITE_LEVEL2(L"Entering EFI_FIRMWARE_MANAGEMENT_PROTOCOL.CheckImage()");

    do
    {
        // Check input parameters
        if (NULL == PpThis || NULL == PpImage || NULL == PpunImageUpdatable || 1 != PbImageIndex || 0 == PullImageSize)
        {
            efiStatus = EFI_INVALID_PARAMETER;
            LOGGING_WRITE_LEVEL1_FMT(L"Error during input parameter check in CheckImage: at least one mandatory parameter is NULL or invalid. (0x%.16lX)", efiStatus);
            break;
        }

        efiStatus = InitializeTpmAccess();
        if (EFI_ERROR(efiStatus))
            break;

        efiStatus = IFXTPMUpdate_FirmwareManagement_CheckImageInternal((BYTE*)PpImage, PullImageSize, PpunImageUpdatable);
        if (EFI_ERROR(efiStatus))
            break;
    }
    WHILE_FALSE_END;

    UninitializeTpmAccess();

    LOGGING_WRITE_LEVEL2_FMT(L"Exiting EFI_FIRMWARE_MANAGEMENT_PROTOCOL.CheckImage(): (0x%.16lX)", efiStatus);

    return efiStatus;
}

/**
 *  @brief      The function is not supported.
 *  @details
 *
 *  @param      PpThis                          -.
 *  @param      PpunPackageVersion              -.
 *  @param      PppPackageVersionName           -.
 *  @param      PpunPackageVersionNameMaxLen    -.
 *  @param      PpullAttributesSupported        -.
 *  @param      PpullAttributesSetting          -.
 *
 *  @retval     EFI_UNSUPPORTED                 The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_GetPackageInfo(
    IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL*   PpThis,
    OUT UINT32*                             PpunPackageVersion,
    OUT CHAR16**                            PppPackageVersionName,
    OUT UINT32*                             PpunPackageVersionNameMaxLen,
    OUT UINT64*                             PpullAttributesSupported,
    OUT UINT64*                             PpullAttributesSetting)
{
    return EFI_UNSUPPORTED;
}

/**
 *  @brief      The function is not supported.
 *  @details
 *
 *  @param      PpThis                      -.
 *  @param      PpImage                     -.
 *  @param      PullImageSize               -.
 *  @param      PpVendorCode                -.
 *  @param      PunPackageVersion           -.
 *  @param      PpPackageVersionName        -.
 *
 *  @retval     EFI_UNSUPPORTED             The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_FirmwareManagement_SetPackageInfo(
    IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL*   PpThis,
    IN  CONST VOID*                         PpImage,
    IN  UINTN                               PullImageSize,
    IN  CONST VOID*                         PpVendorCode,
    IN  UINT32                              PunPackageVersion,
    IN  CONST CHAR16*                       PpPackageVersionName)
{
    return EFI_UNSUPPORTED;
}

//-------------------------------------------------------------------------------------------------------
// Non Firmware Management protocol functions and defines
/// @cond SHOW_INTERNAL
//-------------------------------------------------------------------------------------------------------

/// The services provided by protocol for managing firmware.
EFI_FIRMWARE_MANAGEMENT_PROTOCOL g_IFXTPMUpdateFirmwareManagement =
{
    IFXTPMUpdate_FirmwareManagement_GetImageInfo,
    IFXTPMUpdate_FirmwareManagement_GetImage,
    IFXTPMUpdate_FirmwareManagement_SetImage,
    IFXTPMUpdate_FirmwareManagement_CheckImage,
    IFXTPMUpdate_FirmwareManagement_GetPackageInfo,
    IFXTPMUpdate_FirmwareManagement_SetPackageInfo
};
/// @endcond
