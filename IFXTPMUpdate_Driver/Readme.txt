--------------------------------------------------------------------------------
                       Infineon TPM Firmware Update Driver
                               Source Code Package
                                 v02.01.3610.00
                                 Release Notes

All information in this document is
Copyright (c) 2014 - 2022 Infineon Technologies AG.
All rights reserved.

Microsoft and Visual Studio are registered trademarks of Microsoft Corporation
in the United States and/or other countries.
--------------------------------------------------------------------------------

Contents:

1. Welcome
1.1 Prerequisites
1.2 Contents of the Package

2. Build steps

3. If You Have Questions

4. Release Info
4.1 About This Release
4.2 New Features, Fixes, and Improvements
4.3 Known Bugs and Limitations
4.4 Further information

================================================================================

1. Welcome

Welcome to Infineon TPM Firmware Update Driver Source Code Package.

This package contains source code, EDK II (UEFI Development Kit) solution and
project files required for compiling IFXTPMUpdate.efi, a UEFI (Unified
Extensible Firmware Interface) service driver that can be used by applications
to update the firmware of an Infineon TPM (Trusted Platform Module).

For further information about TPM and TCG (Trusted Computing Group) please visit
https://www.trustedcomputinggroup.org
For further information about UEFI please visit
http://www.uefi.org
For further information about EDK II please visit
http://www.tianocore.org


1.1 Prerequisites

- UEFI Development Kit: edk2-stable202202
- Target architecture: x64
- Build tool chain: Microsoft Visual Studio 2019


1.1.1 Supported devices

- Infineon TPM SLB 9672 TPM2.0
- Infineon TPM SLB 9670 TPM2.0
- Infineon TPM SLB 9670 TPM1.2


1.1.2 Important hints

To avoid any interruption of the update process, it is necessary to suspend all
usage of the TPM during the update process.

Do not turn off or shut down the system during the update process.

It is recommended to always restart the system directly after the TPM Firmware
Update, since certain system hardware and software components might not be aware
of a TPM Firmware Update without a restart (especially in case the TPM family
has been changed with the update).

The total number of firmware updates allowed by the TPM is limited (please
consult your local Infineon representative for further details). Once the limit
has been reached, no further TPM Firmware Update will be possible.
This is also true if the counter for updates onto the same firmware version
becomes zero, even if the overall update counter would allow further updates.
The counter for updates onto the same firmware is decremented if a firmware
update was interrupted or firmware recovery was performed (only SLB 9672).


1.2 Contents of the Package

File name                             Description
=========                             ===========
Doc\IFXTPMUpdate.chm                  Doxygen Source Code Documentation for
                                      IFXTPMUpdate

Doc\Programmers_Reference_Manual.chm  Programmers Reference Manual for
                                      IFXTPMUpdate

Src\RunIFXTPMUpdatePkg\*.*            Source code for test application

Src\TPMToolsUEFIPkg\Common\*.*        Common and platform abstraction layer
                                      source code components of IFXTPMUpdate

Src\TPMToolsUEFIPkg\IFXTPMUpdate\*.*  UEFI TPM Firmware Update Driver related
                                      source code components of IFXTPMUpdate

Src\TPMToolsUEFIPkg\IFXTPMUpdate.dec  EDK II Package Declaration file for
                                      IFXTPMUpdate

Src\TPMToolsUEFIPkg\IFXTPMUpdate.dsc  EDK II Platform Description file for
                                      IFXTPMUpdate

Src\TPMToolsUEFIPkg\IFXTPMUpdate.inf  EDK II Module Information file for
                                      IFXTPMUpdate

Readme.txt                            This file


2. Build steps

The IFXTPMUpdate driver can be built inside EDK II development
environment. This section describes how to integrate IFXTPMUpdate.inf with
edk2-stable202202 and build from Microsoft Visual Studio 2019.
The build steps use the following root directories:
1.  [EDK2]: base directory of the EDK II workspace.
2.  [IFXTPMUPDATE]: base directory for extracted ZIP package of IFXTPMUpdate.

Build steps:
1.  Install OpenSSL according to [EDK2]\CryptoPkg\Library\OpensslLib\
    OpenSSL-HOWTO.txt
2.  Open Developer Command Prompt for VS 2019
3.  Navigate to [EDK2] directory
4.  Call edksetup.bat
5.  Call build -a X64 -b RELEASE -t VS2019 -p ShellPkg/ShellPkg.dsc to ensure
    that EDK II environment is set up correctly.
6.  Copy [IFXTPMUPDATE]\Src\*.* to [EDK2]\*.*
7.  Copy [EDK2]\TPMToolsUEFIPkg\Common\Crypt\UEFI\CryptFncIfx\CryptFncIfx.h
    to [EDK2]\CryptoPkg\Include\Library
8.  Copy [EDK2]\TPMToolsUEFIPkg\Common\Crypt\UEFI\CryptFncIfx\CryptFncIfx.c
    to [EDK2]\CryptoPkg\Library\BaseCryptLib\CryptFncIfx
9.  Open [EDK2]\CryptoPkg\Library\BaseCryptLib\BaseCryptLib.inf and add path
    "CryptFncIfx/CryptFncIfx.c" to [Sources] section
10. Call build -a X64 -b RELEASE -t VS2019 -p TPMToolsUEFIPkg/
    IFXTPMUpdate.dsc
11. The IFXTPMUpdate driver will be created at [EDK2]\Build\
    IFXTPMUpdate\RELEASE_VS2019\X64\IFXTPMUpdate.efi
12. Call build -a X64 -b RELEASE -t VS2019 -p RunIFXTPMUpdatePkg/
    RunIFXTPMUpdate.dsc
13. The RunIFXTPMUpdate sample application will be created at [EDK2]\
    Build\RunIFXTPMUpdate\RELEASE_VS2019\X64\RunIFXTPMUpdate.efi

Verification:
1.  Start the RunIFXTPMUpdate.efi sample application to use IFXTPMUpdate.efi.
2.  Use IFXTPMUpdate.efi with UEFI shell commands (for example)
     Load the driver.
     FS0:>LOAD IFXTPMUpdate.efi
     Verify that driver is loaded.
     FS0:>DRIVERS
     Unload the driver after obtaining its index.
     FS0:>UNLOAD -n [DriverImageIndex]


3. If You Have Questions

If you have any questions or problems, please contact your local Infineon
representative. Further information is available at
https://www.infineon.com/tpm


4. Release Info


4.1 About This Release

Infineon TPM Firmware Update Driver Source Code Package version is
02.01.3610.00.


4.2 New Features, Fixes, and Improvements

[N]ew Features, [R]emoved Features, [F]ixes, and [I]mprovements.

Changes with version 02.01.3610.00
- [N] Add support for SLB 9672
- [N] Add information types for getting firmware update related information
      like operation mode, firmware update counters and firmware details with
      GetInformation method (Adapter Information Protocol)
- [N] Add functionality to abandon non-operational mode if possible with
      SetImage method (Firmware Management Protocol) and in case of unexpected
      failures during start of firmware update process
- [I] Compile with Visual Studio 2019
- [I] Compile with edk2-stable202202 tag
- [I] Add additional vendor check into connect method using TPM_VID register
- [I] Programmers Reference Manual: Enhance manual regarding new functionality
      of IFXTPMUpdate driver and introduce TPM non-operational mode

Changes since version 01.02.2570.00
- [F] Support well-known owner auth for TPM1.2 (20 bytes with zeros)
- [I] Indentation uses spaces instead of tabs

Changes since version 01.01.2532.00
- [I] Resolve namespace collision in MicroTss if using this code and TPM
      command library of UDK in one project.
- [F] Fix sporadic error during firmware update on SLB 9670 device.

Changes since version 01.01.2437.00
- [I] Changed source code license
- [F] Methods from EFI TPM protocols (such as EFI_TCG_PROTOCOL) are no longer
      failing with EFI_BUFFER_TOO_SMALL in between invoking any method in
      IFXTPMUpdate.efi and before unloading IFXTPMUpdate.efi.


4.3 Known Bugs and Limitations

none


4.4 Further information

- On some platforms after performing TPM Firmware Update no UEFI application can
  be started. The workaround is to restart the platform and start the
  application again.

- In case the TPM Firmware Update process has been interrupted and you want to
  resume the update with RunIFXTPMUpdate.efi after reboot, use command line
  RunIFXTPMUpdate.efi tpm12-pp to resume the update (not valid for SLB 9672).
