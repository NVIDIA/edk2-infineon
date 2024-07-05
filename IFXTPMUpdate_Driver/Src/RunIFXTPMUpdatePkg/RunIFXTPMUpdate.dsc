##
#	@brief		Infineon TPM Tools for UEFI implementation.
#	@file		RunIFXTPMUpdate.dsc
#
#	Copyright 2013 - 2022 Infineon Technologies AG ( www.infineon.com )
#
#	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

[Defines]
	PLATFORM_NAME				= RunIFXTPMUpdatePkg
	PLATFORM_GUID				= 84EFA6EE-D383-4206-9916-6B1B621BFFA3
	PLATFORM_VERSION			= 1.00
	DSC_SPECIFICATION			= 0x0001001A
	OUTPUT_DIRECTORY			= Build/RunIFXTPMUpdatePkg
	SUPPORTED_ARCHITECTURES		= X64
	BUILD_TARGETS				= DEBUG|NOOPT|RELEASE
	SKUID_IDENTIFIER			= DEFAULT

[LibraryClasses]
	BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
	BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
	PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
	DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
	MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
	UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
	UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
	UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
	PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
	DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
	UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
	FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
	HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
	ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
	SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
	UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
	ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
	RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

[Components]
	RunIFXTPMUpdatePkg/RunIFXTPMUpdate.inf