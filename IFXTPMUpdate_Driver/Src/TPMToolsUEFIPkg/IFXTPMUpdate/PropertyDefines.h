/**
 *  @brief      Declares property keys for IFXTPMUpdate
 *  @details
 *  @file       IFXTPMUpdate\PropertyDefines.h
 *
 *  Copyright 2014 - 2022 Infineon Technologies AG ( www.infineon.com )
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "StdInclude.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Define for locality configuration setting property
#define PROPERTY_LOCALITY               L"Locality"
/// Define for locality request.
/// If TRUE, locality is requested for use once before first TPM command and released after last TPM response.
/// If FALSE, locality is requested before each TPM command and released after each TPM response.
#define PROPERTY_KEEP_LOCALITY_ACTIVE   L"KeepLocalityActive"
/// Define for firmware update abandon mode property
#define PROPERTY_ABANDON_UPDATE_MODE            L"AbandonUpdateMode"
/// Definitions for PROPERTY_ABANDON_UPDATE_MODE (bit field)
/// Define for no action if firmware update fails
#define ABANDON_UPDATE_NO_ACTION                (UINT32)0x00000000
/// Define for calling abandon update if FieldUpgradeManifestVendor command fails
#define ABANDON_UPDATE_IF_MANIFEST_CALL_FAIL    (UINT32)0x00000001

#ifdef __cplusplus
}
#endif
