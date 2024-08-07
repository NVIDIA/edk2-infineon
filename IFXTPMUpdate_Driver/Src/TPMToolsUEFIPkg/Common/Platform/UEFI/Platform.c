/**
 *  @brief      Implements the Platform interface for UEFI
 *  @details    This module provides platform related functions (memory allocation, string manipulation, time, etc.).
 *  @file       UEFI\Platform.c
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

#include "Platform.h"

/// Handle to the UEFI image
extern EFI_HANDLE gImageHandle;

/**
 *  @brief      Memory allocation initialized with zeros
 *  @details    This function returns a pointer to a zero initialized memory
 *
 *  @param      PunSize     Memory allocation size in bytes.
 *  @retval     != NULL     Pointer to the zero initialized memory.
 *  @retval     NULL        If the allocation fails.
 */
_Check_return_
void*
Platform_MemoryAllocateZero(
    _In_ unsigned int PunSize)
{
    void* pvBuffer = NULL;

    if (0 != PunSize)
        // Allocate new memory and initialize it with zeros
        pvBuffer = AllocateZeroPool(PunSize);

    return pvBuffer;
}

/**
 *  @brief      Memory deallocation
 *  @details    This function releases the allocated memory and sets its pointer to NULL
 *
 *  @param      PppvMemory  Pointer to the pointer to the memory which should be released.
 */
void
Platform_MemoryFree(
    _Inout_opt_ void** PppvMemory)
{
    // Check if pointer is not null and free than
    if (NULL != PppvMemory && NULL != *PppvMemory)
    {
        FreePool(*PppvMemory);
        *PppvMemory = NULL;
    }
}

/**
 *  @brief      Memory compare
 *  @details    This function compares 2 memory buffers
 *
 *  @param      PpvBuffer1      Pointer to the first buffer.
 *  @param      PpvBuffer2      Pointer to the second buffer.
 *  @param      PunSize         Size to compare.
 *  @retval     0               If buffers are equal.
 *  @retval     < 0             If first buffer is less than second one.
 *  @retval     > 0             If first buffer is greater than the second one.
 */
_Check_return_
int
Platform_MemoryCompare(
    _In_reads_bytes_(PunSize)   const void*     PpvBuffer1,
    _In_reads_bytes_(PunSize)   const void*     PpvBuffer2,
    _In_                        unsigned int    PunSize)
{
    // Compare the two buffers
    return (int)CompareMem(PpvBuffer1, PpvBuffer2, PunSize);
}

/**
 *  @brief      Memory Set
 *  @details    This function sets the memory to a value
 *
 *  @param      PpvDestination      Pointer to the buffer.
 *  @param      PnValue             Value to set.
 *  @param      PunSize             Size of the buffer in bytes.
 */
void
Platform_MemorySet(
    _Out_writes_bytes_all_(PunSize) void*           PpvDestination,
    _In_                            int             PnValue,
    _In_                            unsigned int    PunSize)
{
    // Parameter check
    ASSERT(PpvDestination != NULL);
    ASSERT(PunSize != 0);
    // Set memory
    SetMem(PpvDestination, PunSize, (UINT8)PnValue);
}

/**
 *  @brief      Memory copy
 *  @details    This function copies one memory buffer to another
 *
 *  @param      PpvDestination          Pointer to the destination buffer.
 *  @param      PunDestinationCapacity  Capacity of the destination buffer.
 *  @param      PpvSource               Pointer to the source buffer.
 *  @param      PunSize                 Size of the buffer in bytes.
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_FAIL               An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function. One parameter is NULL.
 *  @retval     RC_E_BUFFER_TOO_SMALL   Destination parameter too small.
 */
_Check_return_
unsigned int
Platform_MemoryCopy(
    _Inout_bytecap_(PunDestinationCapacity) void*           PpvDestination,
    _In_                                    unsigned int    PunDestinationCapacity,
    _In_reads_bytes_opt_(PunSize)           const void*     PpvSource,
    _In_                                    unsigned int    PunSize)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        void* pBuffer = NULL;

        // Parameter check.
        if ((NULL == PpvDestination) || (0 == PunDestinationCapacity) ||
                (NULL == PpvSource))
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Check if source buffer is greater than destination buffer
        if (PunSize > PunDestinationCapacity)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Copy memory contents
        pBuffer = CopyMem(PpvDestination, PpvSource, PunSize);
        if (NULL != pBuffer)
            unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    return unReturnValue;
}

/**
 *  @brief      Copy Unicode strings
 *  @details    This function copies the source Unicode string to the destination.
 *
 *  @param      PwszDestination         Pointer to the destination Unicode buffer.
 *  @param      PpunDestinationCapacity In: Capacity of the destination buffer in elements (include additional space for terminating 0)\n
 *                                      Out: Number of elements copied to the destination buffer (without terminating 0)
 *  @param      PwszSource              Pointer to the source Unicode buffer.
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_FAIL               An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function. One parameter is NULL.
 *  @retval     RC_E_BUFFER_TOO_SMALL   Destination parameter too small.
 */
_Check_return_
unsigned int
Platform_StringCopy(
    _Out_z_cap_(*PpunDestinationCapacity)   wchar_t*        PwszDestination,
    _Inout_                                 unsigned int*   PpunDestinationCapacity,
    _In_z_                                  const wchar_t*  PwszSource)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        RETURN_STATUS status;

        // Check parameters
        if (NULL == PwszSource || NULL == PwszDestination || NULL == PpunDestinationCapacity)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Copy string
        status = StrCpyS(PwszDestination, *PpunDestinationCapacity, PwszSource);
        if (RETURN_SUCCESS != status)
        {
            // Check that destination is large enough
            if (RETURN_BUFFER_TOO_SMALL == status)
            {
                unReturnValue = RC_E_BUFFER_TOO_SMALL;
            }
            else if (RETURN_INVALID_PARAMETER == status)
            {
                unReturnValue = RC_E_BAD_PARAMETER;
            }
            else
            {
                unReturnValue = RC_E_FAIL;
            }
            break;
        }

        // Update destination length
        *PpunDestinationCapacity = (unsigned int)StrLen(PwszSource);
        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameters
        if (NULL != PwszDestination)
            PwszDestination[0] = L'\0';
        if (NULL != PpunDestinationCapacity)
            *PpunDestinationCapacity = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Compare strings
 *  @details    This function compares two strings
 *
 *  @param      PwszString1         First string to compare.
 *  @param      PwszString2         Second string to compare.
 *  @param      PunCount            The maximum number of elements to compare.
 *  @param      PfCaseInsensitive   If set to TRUE, do a case insensitive compare; FALSE for case sensitive compare.
 *  @retval     0                   If strings match.
 *  @retval     <0                  string 1 less than string 2.
 *  @retval     >0                  string 1 greater than string 2.
 */
_Check_return_
int
Platform_StringCompare(
    _In_z_  const wchar_t*  PwszString1,
    _In_z_  const wchar_t*  PwszString2,
    _In_    unsigned int    PunCount,
    _In_    BOOL            PfCaseInsensitive)
{
    int nReturn = -2;

    // Compare pointers and strings
    if (NULL == PwszString1 && NULL == PwszString2)
        nReturn = 0;
    else if (NULL == PwszString1)
        nReturn = -1;
    else if (NULL == PwszString2)
        nReturn = 1;
    else
    {
        // Stores for upper case strings to compare them in temporary memory if necessary
        wchar_t* wszString1ToUpper = NULL;
        wchar_t* wszString2ToUpper = NULL;

        do
        {
            // If necessary, bring strings to upper case
            if (TRUE == PfCaseInsensitive)
            {
                unsigned int unIndex = 0;
                unsigned int unBufferSize = (PunCount + 1) * sizeof(L'\0'); // Add space for null-termination (required e.g. in case substring is being compared)

                // Allocate memory for first upper string
                wszString1ToUpper = (wchar_t*)Platform_MemoryAllocateZero(unBufferSize);
                if (NULL == wszString1ToUpper)
                    break;

                // Bring all characters in first string to upper case
                for (unIndex = 0; unIndex < PunCount; unIndex++)
                    *(wszString1ToUpper + unIndex) = (wchar_t)Platform_WCharToUpper((unsigned short) * (PwszString1 + unIndex));

                // Allocate memory for second upper string
                wszString2ToUpper = (wchar_t*)Platform_MemoryAllocateZero(unBufferSize);
                if (NULL == wszString2ToUpper)
                    break;

                // Bring all characters in second string to upper case
                for (unIndex = 0; unIndex < PunCount; unIndex++)
                    *(wszString2ToUpper + unIndex) = (wchar_t)Platform_WCharToUpper((unsigned short) * (PwszString2 + unIndex));

                // Compare upper case strings
                nReturn = (int)StrnCmp(wszString1ToUpper, wszString2ToUpper, PunCount);
            }
            else
                // Compare original strings
                nReturn = (int)StrnCmp(PwszString1, PwszString2, PunCount);
        }
        WHILE_FALSE_END;

        // Cleanup
        Platform_MemoryFree((void**)&wszString1ToUpper);
        Platform_MemoryFree((void**)&wszString2ToUpper);
    }

    return nReturn;
}

/**
 *  @brief      Format Unicode strings
 *  @details    This function formats a Unicode string
 *
 *  @param      PwszDestination         Pointer to the destination Unicode buffer.
 *  @param      PpunDestinationCapacity In:     Capacity of the destination buffer in elements\n
 *                                      Out:    Count of written elements
 *  @param      PwszSource              Pointer to the source Unicode buffer.
 *  @param      ...                     Additional parameters to format the string.
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_FAIL               An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function. One parameter is NULL.
 *  @retval     ...                     Error codes from Platform_StringFormatV.
 */
_Check_return_
unsigned int
IFXAPI
Platform_StringFormat(
    _Out_z_cap_(*PpunDestinationCapacity)   wchar_t*        PwszDestination,
    _Inout_                                 unsigned int*   PpunDestinationCapacity,
    _In_z_                                  const wchar_t*  PwszSource,
    ...)
{
    va_list argptr;
    unsigned int unReturnValue = RC_E_FAIL;

    // Check used parameter (the others will be checked later)
    if (NULL == PwszSource)
        unReturnValue = RC_E_BAD_PARAMETER;
    else
    {
        // Prepare string to write in buffer szBuf
        va_start(argptr, PwszSource);
        unReturnValue = Platform_StringFormatV(PwszDestination, PpunDestinationCapacity, PwszSource, argptr);
        va_end(argptr);
    }

    return unReturnValue;
}

/**
 *  @brief      Format Unicode strings using a va_list
 *  @details    This function formats a Unicode string using a va_list
 *
 *  @param      PwszDestination         Pointer to the destination Unicode buffer.
 *  @param      PpunDestinationCapacity In:     Capacity of the destination buffer in elements\n
 *                                      Out:    Count of written elements
 *  @param      PwszSource              Pointer to the source Unicode buffer.
 *  @param      PargList                Additional parameters to format the string.
 *
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_FAIL               An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function. One parameter is NULL.
 *  @retval     RC_E_BUFFER_TOO_SMALL   Destination parameter too small.
 *  @retval     RC_E_INTERNAL           An internal error occurs (only on windows and linux).
 */
_Check_return_
unsigned int
Platform_StringFormatV(
    _Out_z_cap_(*PpunDestinationCapacity)   wchar_t*        PwszDestination,
    _Inout_                                 unsigned int*   PpunDestinationCapacity,
    _In_z_                                  const wchar_t*  PwszSource,
    _In_                                    va_list         PargList)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        int nWritten = 0;
        unsigned int unSize = 0;

        // Check parameters
        if (NULL == PwszDestination || NULL == PpunDestinationCapacity || PLATFORM_STRING_IS_NULL_OR_EMPTY(PwszSource))
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Check destination buffer capacity; we need at least a size of 1 for null-termination
        if (0 == *PpunDestinationCapacity)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Get byte capacity of destination buffer
        unSize = *PpunDestinationCapacity * sizeof(wchar_t);

        // Format string
        nWritten = (int)UnicodeVSPrint(PwszDestination, unSize, PwszSource, PargList);

        // Currently, the code below has no effect on UEFI, since UnicodeVSPrint() never returns -1.
        // => no chance at the moment to check the size limitation.
        if (nWritten < 0)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        *PpunDestinationCapacity = nWritten;
        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameters
        if (NULL != PwszDestination)
            PwszDestination[0] = L'\0';
        if (NULL != PpunDestinationCapacity)
            *PpunDestinationCapacity = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Get Unicode string length
 *  @details    This function returns the length of a Unicode string in elements without the terminating 0
 *
 *  @param      PwszBuffer              Pointer to the Unicode string buffer.
 *  @param      PunMaximumCapacity      Maximum capacity of the string buffer in elements (including terminating 0).
 *  @param      PpunStrLen              Length of the Unicode string in elements (without terminating 0)
 *  @returns    RC_SUCCESS  The operation completed successfully. In case of an error, *PpunStrLen is set to 0 if possible and one of the following error codes is being returned:
 *  @retval     RC_E_FAIL               An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function. PwszBuffer or PpunStrLen is NULL, or PunMaximumCapacity is 0.
 *  @retval     RC_E_BUFFER_TOO_SMALL   String is not null-terminated within given maximum capacity.
 */
_Check_return_
unsigned int
Platform_StringGetLength(
    _In_z_  const wchar_t*  PwszBuffer,
    _In_    unsigned int    PunMaximumCapacity,
    _Out_   unsigned int*   PpunStrLen)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        // Check parameters
        if (NULL == PwszBuffer || 0 == PunMaximumCapacity || NULL == PpunStrLen)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Get string length
        *PpunStrLen = (unsigned int)StrLen(PwszBuffer);
        if (*PpunStrLen >= PunMaximumCapacity) // In this case the input string is not null-terminated and / or maximum capacity is too small
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameter
        if (NULL != PpunStrLen)
            *PpunStrLen = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Concatenates Unicode strings
 *  @details    This function concatenates destination and source Unicode strings.
 *
 *  @param      PwszDestination             In: Pointer to the destination Unicode buffer
 *                                          Out: Pointer to the concatenated Unicode buffer
 *  @param      PpunDestinationCapacity     In: Capacity of the destination buffer in elements (include additional space for terminating 0)\n
 *                                          Out: Number of elements in the destination buffer (without terminating 0)
 *  @param      PwszSource                  Pointer to the source Unicode buffer.
 *  @retval     RC_SUCCESS                  The operation completed successfully.
 *  @retval     RC_E_FAIL                   An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER          An invalid parameter was passed to the function. One parameter is NULL or strings overlap.
 *  @retval     RC_E_BUFFER_TOO_SMALL       Destination buffer is too small.
 */
_Check_return_
unsigned int
Platform_StringConcatenate(
    _Inout_updates_z_(*PpunDestinationCapacity) wchar_t*        PwszDestination,
    _Inout_                                     unsigned int*   PpunDestinationCapacity,
    _In_z_                                      const wchar_t*  PwszSource)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        unsigned int unSourceLength = 0;
        unsigned int unDestinationLength = 0;
        RETURN_STATUS status;

        // Check parameters
        if (NULL == PwszDestination || NULL == PpunDestinationCapacity || NULL == PwszSource)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Check destination buffer capacity; we need at least a size of 1 for null-termination
        if (0 == *PpunDestinationCapacity)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Get original string lengths
        unReturnValue = Platform_StringGetLength(PwszSource, *PpunDestinationCapacity, &unSourceLength);
        if (RC_SUCCESS != unReturnValue)
            break;
        unReturnValue = Platform_StringGetLength(PwszDestination, *PpunDestinationCapacity, &unDestinationLength);
        if (RC_SUCCESS != unReturnValue)
            break;

        // Check destination buffer capacity; we need at least a size of 1 more element for null-termination
        if (*PpunDestinationCapacity <= unSourceLength + unDestinationLength)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Check if strings overlap (not supported by concatenation method)
        if ((PwszSource == PwszDestination) ||
                (PwszSource < PwszDestination && PwszSource + unSourceLength >= PwszDestination) ||
                (PwszSource > PwszDestination && PwszDestination + unDestinationLength >= PwszSource))
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Concatenate strings
        status = StrCatS(PwszDestination, *PpunDestinationCapacity, PwszSource);
        if (RETURN_SUCCESS != status)
        {
            unReturnValue = RC_E_FAIL;
            break;
        }

        // Update destination length
        unReturnValue = Platform_StringGetLength(PwszDestination, *PpunDestinationCapacity, PpunDestinationCapacity);
    }
    WHILE_FALSE_END;

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameters
        if (NULL != PwszDestination)
            PwszDestination[0] = L'\0';
        if (NULL != PpunDestinationCapacity)
            *PpunDestinationCapacity = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Concatenates Unicode strings representing two paths
 *  @details    This function concatenates destination and source Unicode strings representing two paths. The function adds if necessary
 *              a '\' or '/' depending on the operating system.
 *
 *  @param      PwszDestination             In: Pointer to the destination Unicode buffer
 *                                          Out: Pointer to the concatenated Unicode buffer
 *  @param      PpunDestinationCapacity     In: Capacity of the destination buffer in elements (include additional space for terminating 0)\n
 *                                          Out: Number of elements in the destination buffer (without terminating 0)
 *  @param      PwszSource                  Pointer to the source Unicode buffer.
 *  @retval     RC_SUCCESS                  The operation completed successfully.
 *  @retval     RC_E_FAIL                   An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER          An invalid parameter was passed to the function. One parameter is NULL or strings overlap.
 *  @retval     RC_E_BUFFER_TOO_SMALL       Destination buffer is too small.
 */
_Check_return_
unsigned int
Platform_StringConcatenatePaths(
    _Inout_updates_z_(*PpunDestinationCapacity) wchar_t*        PwszDestination,
    _Inout_                                     unsigned int*   PpunDestinationCapacity,
    _In_z_                                      const wchar_t*  PwszSource)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        unsigned int unSourceLength = 0;
        unsigned int unDestinationLength = 0;
        RETURN_STATUS status;

        // Check parameters
        if (NULL == PwszDestination || NULL == PpunDestinationCapacity || NULL == PwszSource)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Check destination buffer capacity; we need at least a size of 1 for null-termination
        if (0 == *PpunDestinationCapacity)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Get original string lengths
        unReturnValue = Platform_StringGetLength(PwszSource, *PpunDestinationCapacity, &unSourceLength);
        if (RC_SUCCESS != unReturnValue)
            break;
        unReturnValue = Platform_StringGetLength(PwszDestination, *PpunDestinationCapacity, &unDestinationLength);
        if (RC_SUCCESS != unReturnValue)
            break;

        // Check if last character of destination is '\' or '/'
        if (unDestinationLength > 0 &&
                PwszDestination[unDestinationLength - 1] != L'\\' &&
                PwszDestination[unDestinationLength - 1] != L'/')
        {
            if (*PpunDestinationCapacity <= unDestinationLength + 1)
            {
                unReturnValue = RC_E_BUFFER_TOO_SMALL;
                break;
            }

            // Check if strings overlap (not supported by concatenation method)
            if ((PwszSource == PwszDestination) ||
                    (PwszSource < PwszDestination && PwszSource + unSourceLength >= PwszDestination) ||
                    (PwszSource > PwszDestination && PwszDestination + unDestinationLength >= PwszSource))
            {
                unReturnValue = RC_E_BAD_PARAMETER;
                break;
            }

            // Concatenate strings
            status = StrCatS(PwszDestination, *PpunDestinationCapacity, L"\\");
            if (RETURN_SUCCESS != status)
            {
                unReturnValue = RC_E_FAIL;
                break;
            }
        }

        // Concatenate strings
        unReturnValue = Platform_StringConcatenate(PwszDestination, PpunDestinationCapacity, PwszSource);
    }
    WHILE_FALSE_END;

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameters
        if (NULL != PwszDestination)
            PwszDestination[0] = L'\0';
        if (NULL != PpunDestinationCapacity)
            *PpunDestinationCapacity = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Convert an ANSI string to a Unicode string
 *  @details    This function converts an ANSI string to a Unicode string
 *
 *  @param      PwszDestination             Pointer to the destination Unicode string buffer.
 *  @param      PunDestinationCapacity      Size of the destination Unicode string buffer in wide characters.
 *  @param      PszSource                   Pointer to the ANSI string buffer.
 *  @retval     RC_SUCCESS                  The operation completed successfully.
 *  @retval     RC_E_FAIL                   An unexpected error occurred.
 *  @retval     RC_E_BAD_PARAMETER          An invalid parameter was passed to the function. One parameter is NULL.
 *  @retval     RC_E_BUFFER_TOO_SMALL       Destination buffer is too small.
 */
_Check_return_
unsigned int
Platform_AnsiString2UnicodeString(
    _Inout_updates_z_(PunDestinationCapacity)   wchar_t*        PwszDestination,
    _In_                                        unsigned int    PunDestinationCapacity,
    _In_z_                                      const char*     PszSource)
{
    unsigned int unReturnValue = RC_E_FAIL;
    RETURN_STATUS status;

    do
    {
        // Check parameters
        if (NULL == PwszDestination || 0 == PunDestinationCapacity || NULL == PszSource)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Convert from ANSI to Unicode
        status = AsciiStrToUnicodeStrS(PszSource, PwszDestination, PunDestinationCapacity);
        if (RETURN_SUCCESS != status)
        {
            // Check that destination is large enough
            if (RETURN_BUFFER_TOO_SMALL == status)
            {
                unReturnValue = RC_E_BUFFER_TOO_SMALL;
            }
            else
            {
                unReturnValue = RC_E_FAIL;
            }
            break;
        }
        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    return unReturnValue;
}

/**
 *  @brief      Convert a string to integer
 *  @details    This function returns the int value produced by interpreting the input characters as a decimal number
 *
 *  @param      PwszBuffer              Pointer to the Unicode string buffer
 *  @returns    Decimal numerical value of the interpreted string as integer
 */
_Check_return_
int
Platform_String2Int(
    _In_z_ const wchar_t* PwszBuffer)
{
    int nRetVal = 0;

    if (!PLATFORM_STRING_IS_NULL_OR_EMPTY(PwszBuffer))
        nRetVal = (int)StrDecimalToUintn(PwszBuffer);

    return nRetVal;
}

/**
 *  @brief      Set a String to zero wide characters
 *  @details    This function sets all wide characters of the buffer to zero
 *
 *  @param      PwszBuffer          Pointer to the Unicode string buffer.
 *  @param      PunBufferSize       Size of wchar_t buffer in elements.
 *  @retval     RC_SUCCESS          The operation completed successfully.
 *  @retval     RC_E_BAD_PARAMETER  An invalid parameter was passed to the function. One parameter is NULL.
 */
#ifdef _MSC_VER
#pragma warning(suppress: 6054) // Can be suppressed since output buffer will definitely be null-terminated
#endif
_Check_return_
unsigned int
Platform_StringSetZero(
    _Out_writes_z_(PunBufferSize)   wchar_t*        PwszBuffer,
    _In_                            unsigned int    PunBufferSize)
{
    unsigned int unReturnValue = RC_E_FAIL;

    // Check input parameters
    if (NULL == PwszBuffer || PunBufferSize == 0)
        unReturnValue = RC_E_BAD_PARAMETER;
    else
    {
        Platform_MemorySet(PwszBuffer, 0, PunBufferSize * sizeof(PwszBuffer[0]));
        unReturnValue = RC_SUCCESS;
    }

    if (RC_SUCCESS != unReturnValue)
        // Reset out parameter
        if (NULL != PwszBuffer)
            PwszBuffer[0] = L'\0';

    return unReturnValue;
}

/**
 *  @brief      Converts a wide character to upper case
 *  @details
 *
 *  @param      PwchToUpper     Wide character to be converted
 *  @returns    Converted upper case wide character
 */
_Check_return_
wchar_t
Platform_WCharToUpper(
    _In_ wchar_t PwchToUpper)
{
    // Check if value is a lower case letter
    if (PwchToUpper >= 'a' && PwchToUpper <= 'z')
        // Convert letter to upper case
        PwchToUpper += (unsigned short)('A' - 'a');

    return PwchToUpper;
}

/**
 *  @brief      Gets the current time
 *  @details    Retrieves the current local date and time. The accuracy is platform and OS dependent.
 *
 *  @param      PpTime                  The current local time; accuracy is given in this structure and is platform and OS dependent.
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function.
 *  @retval     RC_E_FAIL               An unexpected error occurred. Returned from the system call.
 */
_Check_return_
unsigned int
Platform_GetTime(
    _Inout_ IfxTime* PpTime)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        EFI_STATUS efiStatus = EFI_DEVICE_ERROR;
        EFI_TIME time;
        EFI_TIME_CAPABILITIES timeCapabilities;

        ZeroMem(&time, sizeof(time));
        ZeroMem(&timeCapabilities, sizeof(timeCapabilities));

        // Check parameters
        if (NULL == PpTime)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Get time
        efiStatus = gRT->GetTime(&time, &timeCapabilities);
        if (EFI_INVALID_PARAMETER == efiStatus)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }
        if (EFI_ERROR(efiStatus))
            break;

        // Assign values to returned structure
        PpTime->unYear = time.Year;
        PpTime->unMonth = time.Month;
        PpTime->unDay = time.Day;
        PpTime->unHour = time.Hour;
        PpTime->unMinute = time.Minute;
        PpTime->unSecond = time.Second;
        if (1 < timeCapabilities.Resolution)
        {
            PpTime->fMillisecondAvailable = TRUE;
            PpTime->nMillisecond = time.Nanosecond / 1000000;
        }
        else
        {
            PpTime->fMillisecondAvailable = FALSE;
            PpTime->nMillisecond = MIL_SEC_INVAL;
        }

        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    return unReturnValue;
}

/**
 *  @brief      Sleeps the given time in milliseconds
 *  @details
 *
 *  @param      PunSleepTime            Time to sleep in milliseconds.
 */
void
Platform_Sleep(
    _In_ unsigned int PunSleepTime)
{
    Platform_SleepMicroSeconds(1000 * PunSleepTime);
}

/**
 *  @brief      Sleeps the given time in microseconds.
 *  @details
 *
 *  @param      PunSleepTime    Time to sleep in microseconds.
 */
void
Platform_SleepMicroSeconds(
    _In_ unsigned int PunSleepTime)
{
    // Check global pointer
    if (NULL != gBS)
        gBS->Stall(PunSleepTime);
}

/**
 *  @brief      Swaps a UINT16
 *  @details
 *
 *  @param      PusValue
 *  @returns    Swapped UINT16 value
 */
unsigned short
Platform_SwapBytes16(
    _In_ unsigned short PusValue)
{
    return SwapBytes16(PusValue);
}

/**
 *  @brief      Swaps a UINT32
 *  @details
 *
 *  @param      PunValue
 *  @returns    Swapped UINT32 value
 */
unsigned int
Platform_SwapBytes32(
    _In_ unsigned int PunValue)
{
    return SwapBytes32(PunValue);
}

/**
 *  @brief      Unmarshal a Unicode string (16bit per character) to the target platform
 *  @details
 *
 *  @param      PrgbBuffer              Binary buffer.
 *  @param      PunBufferLen            Length of binary buffer.
 *  @param      PwszTargetString        Receives the unmarshalled string including null-termination.
 *  @param      PpunTargetStringLen     On input the capacity of the wchar_t buffer in elements.
 *                                      On output the length of the wchar_t string in elements without terminating NULL character.
 *
 *  @retval     RC_SUCCESS              The operation completed successfully.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function.
 *  @retval     RC_E_BUFFER_TOO_SMALL   The buffer in PwszTargetString is too small to hold the contents of the binary buffer.
 *  @retval     RC_E_FAIL               An internal error occurred. Either there was not enough memory to create a temporary copy of PrgbBuffer or StrnCpyS failed.
 */
_Check_return_
unsigned int
Platform_UnmarshalString(
    _In_bytecount_(PunBufferLen)            const void*     PrgbBuffer,
    _In_                                    unsigned int    PunBufferLen,
    _Out_writes_z_(*PpunTargetStringLen)    wchar_t*        PwszTargetString,
    _Inout_                                 unsigned int*   PpunTargetStringLen)
{
    unsigned int unReturnValue = RC_E_FAIL;
    wchar_t* pwszTempBuffer = NULL;

    do
    {
        // Check parameters
        if (NULL == PrgbBuffer || 0 == PunBufferLen || NULL == PwszTargetString || 0 == PpunTargetStringLen || 0 == *PpunTargetStringLen)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Input buffer must have even length for unmarshaling to succeed.
        if (PunBufferLen % 2 != 0)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Check if target buffer is too small to fit the input string.
        if (PunBufferLen / 2 >= *PpunTargetStringLen)
        {
            unReturnValue = RC_E_BUFFER_TOO_SMALL;
            break;
        }

        // Memory for Strn* APIs must be aligned to 16bit boundaries. Copy the buffer to aligned memory first.
        pwszTempBuffer = AllocateZeroPool(PunBufferLen);
        if (NULL == pwszTempBuffer)
        {
            unReturnValue = RC_E_FAIL;
            break;
        }
        CopyMem(pwszTempBuffer, PrgbBuffer, PunBufferLen);

        {
            // Get string length of the buffer, truncate if it is not null-terminated.
            RETURN_STATUS status;
            UINTN ullBuffer = StrnLenS(pwszTempBuffer, PunBufferLen / 2);

            // Copy the string.
            status = StrnCpyS(PwszTargetString, *PpunTargetStringLen, pwszTempBuffer, ullBuffer);
            if (RETURN_SUCCESS != status)
            {
                unReturnValue = RC_E_FAIL;
                break;
            }

            // Update the string length.
            *PpunTargetStringLen = (unsigned int)ullBuffer;
        }

        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    if (pwszTempBuffer != NULL)
    {
        FreePool(pwszTempBuffer);
    }

    if (RC_SUCCESS != unReturnValue)
    {
        // Reset out parameter
        if (NULL != PpunTargetStringLen)
            *PpunTargetStringLen = 0;
    }

    return unReturnValue;
}

/**
 *  @brief      Finds a string in another string.
 *  @details
 *
 *  @param      PwszSearch              String to search for.
 *  @param      PwszString              String to search in.
 *  @param      PpwszStart              If return code is RC_SUCCESS, returns the start position of the PwszSearch in PwszString.
 *
 *  @retval     RC_SUCCESS              Search string was found in the actual string.
 *  @retval     RC_E_NOT_FOUND          Search string was not found in the actual string.
 *  @retval     RC_E_BAD_PARAMETER      An invalid parameter was passed to the function.
 */
_Check_return_
unsigned int
Platform_FindString(
    _In_z_  const wchar_t*  PwszSearch,
    _In_z_  const wchar_t*  PwszString,
    _Out_   wchar_t**       PpwszStart)
{
    unsigned int unReturnValue = RC_E_FAIL;

    do
    {
        // Check parameters
        if (NULL == PwszSearch || NULL == PwszString || NULL == PpwszStart)
        {
            unReturnValue = RC_E_BAD_PARAMETER;
            break;
        }

        // Find the string
        *PpwszStart = StrStr(PwszString, PwszSearch);
        if (NULL == *PpwszStart)
        {
            // String was not found
            unReturnValue = RC_E_NOT_FOUND;
            break;
        }

        // String was found
        unReturnValue = RC_SUCCESS;
    }
    WHILE_FALSE_END;

    return unReturnValue;
}
