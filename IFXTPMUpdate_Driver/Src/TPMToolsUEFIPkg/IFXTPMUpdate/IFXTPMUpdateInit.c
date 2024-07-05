/**
 *  @brief      Implements the initialization for IFXTPMUpdate
 *  @details    Implementation file for the Infineon TPM Firmware Update Driver.
 *              The IFXTPMUpdate driver implements the TPM Firmware Update functionality.
 *  @file       IFXTPMUpdateInit.c
 *
 *  Copyright 2014 - 2022 Infineon Technologies AG ( www.infineon.com )
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "StdInclude.h"
#include "DeviceManagement.h"
#include "IFXTPMUpdateApp.h"
#include "Crypt.h"

IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA* g_pPrivateData = NULL;

const wchar_t CwszErrorMsgFormatPropertyStorage_Set[] = L"Error while setting property %ls. (0x%.16lX)";

/**
 *  @brief      Initialize the driver/library data.
 *  @details    Initialize the driver/library data.
 *
 *  @retval     EFI_SUCCESS             The entry point is executed successfully.
 *  @retval     EFI_OUT_OF_RESOURCES    In case of an allocate memory failed.
 *  @retval     EFI_DEVICE_ERROR        In case the random number generator could not be seeded.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdateInitialize()
{
    EFI_STATUS  efiStatus;

    do
    {
        // Allocate and zero the private context data structure
        g_pPrivateData = (IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA*)AllocateZeroPool(sizeof(IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA));
        if (NULL == g_pPrivateData)
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            break;
        }

        // Seed the random number generator
        if (RC_SUCCESS != Crypt_SeedRandom(NULL, 0))
        {
            efiStatus = EFI_DEVICE_ERROR;
            break;
        }

        // Set abandon firmware update behavior.
        if (!PropertyStorage_SetUIntegerValueByKey(PROPERTY_ABANDON_UPDATE_MODE, ABANDON_UPDATE_IF_MANIFEST_CALL_FAIL))
        {
            efiStatus = EFI_OUT_OF_RESOURCES;
            LOGGING_WRITE_LEVEL1_FMT(CwszErrorMsgFormatPropertyStorage_Set, PROPERTY_ABANDON_UPDATE_MODE, efiStatus);
            break;
        }

        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    return efiStatus;
}

/**
 *  @brief      Initializes access to the TPM.
 *  @details
 *
 *  @retval     EFI_SUCCESS                         TPM access initialized successfully or already initialized.
 *  @retval     EFI_DEVICE_ERROR                    Failed to initialize TPM access.
 *  @retval     EFI_OUT_OF_RESOURCES                Not enough memory.
 *  @retval     EFI_IFXTPM_UNSUPPORTED_VENDOR       The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
EFI_STATUS
EFIAPI
InitializeTpmAccess()
{
    EFI_STATUS efiStatus = EFI_DEVICE_ERROR;

    do
    {
        if (!g_pPrivateData->fTpmAccessInitialized)
        {
            unsigned int unReturnValue = RC_E_FAIL;

            // Set default access mode: LOCALITY_0
            if (!PropertyStorage_SetUIntegerValueByKey(PROPERTY_LOCALITY, LOCALITY_0))
            {
                efiStatus = EFI_OUT_OF_RESOURCES;
                LOGGING_WRITE_LEVEL1_FMT(CwszErrorMsgFormatPropertyStorage_Set, PROPERTY_LOCALITY, efiStatus);
                break;
            }

            // Set locality request mode
            if (!PropertyStorage_SetBooleanValueByKey(PROPERTY_KEEP_LOCALITY_ACTIVE, TRUE))
            {
                efiStatus = EFI_OUT_OF_RESOURCES;
                LOGGING_WRITE_LEVEL1_FMT(CwszErrorMsgFormatPropertyStorage_Set, PROPERTY_KEEP_LOCALITY_ACTIVE, efiStatus);
                break;
            }

            // Set default TPM device access
            UINT32 unDeviceAccess = TPM_DEVICE_ACCESS_MEMORY_BASED;

            // Set device access mode
            if (!PropertyStorage_SetUIntegerValueByKey(PROPERTY_TPM_DEVICE_ACCESS_MODE, unDeviceAccess))
            {
                efiStatus = EFI_OUT_OF_RESOURCES;
                LOGGING_WRITE_LEVEL1_FMT(CwszErrorMsgFormatPropertyStorage_Set, PROPERTY_TPM_DEVICE_ACCESS_MODE, efiStatus);
                break;
            }

            // Initialize TPM device access
            DeviceManagement_Initialize();

            // Connect to the TPM device
            unReturnValue = DeviceManagement_Connect();
            if (RC_SUCCESS != unReturnValue)
            {
                LOGGING_WRITE_LEVEL1_FMT(L"Error during connecting to the TPM. (0x%.8X)", unReturnValue);
                break;
            }

            g_pPrivateData->fTpmAccessInitialized = TRUE;
        }
        efiStatus = EFI_SUCCESS;
    }
    WHILE_FALSE_END;

    return efiStatus;
}

/**
 *  @brief      Uninitializes access to the TPM.
 *  @details
 */
VOID
EFIAPI
UninitializeTpmAccess()
{
    if (g_pPrivateData->fTpmAccessInitialized)
    {
        // Disconnect to the TPM device
        IGNORE_RETURN_VALUE(DeviceManagement_Disconnect());
        // Uninitialize TPM device access
        DeviceManagement_Uninitialize();
        g_pPrivateData->fTpmAccessInitialized = FALSE;
    }
}
