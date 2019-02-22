/*++

Copyright (c) 2009 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials are licensed and made available under
  the terms and conditions of the BSD License that accompanies this distribution.
  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.



Module Name:

  MiscSystemManufacturerFunction.c

Abstract:

  This driver parses the mMiscSubclassDataTable structure and reports
  any generated data.

--*/


#include "CommonHeader.h"
#include "MiscSubclassDriver.h"
#include <Protocol/DxeSmmReadyToLock.h>
#include <Library/NetLib.h>
#include "Library/DebugLib.h"
#include <Uefi/UefiBaseType.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>


extern EFI_PLATFORM_INFO_HOB *mPlatformInfo;
EFI_GUID                  mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;


/**
  Return the description for network boot device.

  @param Handle                Controller handle.

  @return  The description string.
**/
CHAR16 *
GetNetworkDescription (
  IN EFI_HANDLE                  Handle
  )
{
  EFI_STATUS                     Status;
  EFI_DEVICE_PATH_PROTOCOL       *DevicePath;
  MAC_ADDR_DEVICE_PATH           *Mac;
  CHAR16                         *Description;
  UINTN                          DescriptionSize;

  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiLoadFileProtocolGuid,
                  NULL,
                  gImageHandle,
                  Handle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath,
                  gImageHandle,
                  Handle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status) || (DevicePath == NULL)) {
    return NULL;
  }

  //
  // The PXE device path is like:
  //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]
  //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv4(...)
  //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv6(...)
  //
  // The HTTP device path is like:
  //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv4(...)/Uri(...)
  //   ....../Mac(...)[/Vlan(...)][/Wi-Fi(...)]/IPv6(...)/Uri(...)
  //
  while (!IsDevicePathEnd (DevicePath) &&
         ((DevicePathType (DevicePath) != MESSAGING_DEVICE_PATH) ||
          (DevicePathSubType (DevicePath) != MSG_MAC_ADDR_DP))
         ) {
    DevicePath = NextDevicePathNode (DevicePath);
  }

  if (IsDevicePathEnd (DevicePath)) {
    return NULL;
  }

  Mac = (MAC_ADDR_DEVICE_PATH *) DevicePath;
  DevicePath = NextDevicePathNode (DevicePath);

  //
  // Skip the optional Wi-Fi node
  //
  if ((DevicePathType (DevicePath) == MESSAGING_DEVICE_PATH) &&
      (DevicePathSubType (DevicePath) == MSG_WIFI_DP)
      ) {
    DevicePath = NextDevicePathNode (DevicePath);
  }

  //
  // Build description like below:
  //   "PXEv6 (MAC:112233445566 VLAN1)"
  //   "HTTPv4 (MAC:112233445566)"
  //
  DescriptionSize = sizeof (L"112233445566");
  Description     = AllocatePool (DescriptionSize);
  ASSERT (Description != NULL);
  UnicodeSPrint (
    Description, DescriptionSize,
    L"%02x%02x%02x%02x%02x%02x",
    Mac->MacAddress.Addr[0], Mac->MacAddress.Addr[1], Mac->MacAddress.Addr[2],
    Mac->MacAddress.Addr[3], Mac->MacAddress.Addr[4], Mac->MacAddress.Addr[5]
    );

  return Description;
}



CHAR16 *
GetMacAddressString(
  )
{
  EFI_HANDLE    *Handles;
  UINTN         HandleCount;
  UINT8         Index;
  CHAR16        *MacAddressString = NULL;
  
  //
  // Parse load file protocol
  //
  gBS->LocateHandleBuffer (
       ByProtocol,
       &gEfiLoadFileProtocolGuid,
       NULL,
       &HandleCount,
       &Handles
       );
  for (Index = 0; Index < HandleCount; Index++) {

    MacAddressString = GetNetworkDescription (Handles[Index]);

    if (MacAddressString != NULL) {
      break;
    }
  }

  if (HandleCount != 0) {
    FreePool (Handles);
  }

  return MacAddressString;
}


EFI_STATUS
EFIAPI
AddSmbiosManuCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  CHAR8                             *OptionalStrStart;
  UINTN                             ManuStrLen;
  UINTN                             VerStrLen;
  UINTN                             PdNameStrLen;
  UINTN                             SerialNumStrLen;
  UINTN                             SkuNumberStrLen;
  UINTN                             FamilyNameStrLen;
  EFI_STATUS                        Status;
  EFI_STRING                        Manufacturer;
  EFI_STRING                        ProductName;
  EFI_STRING                        Version;
  EFI_STRING                        SerialNumber;
  EFI_STRING                        SkuNumber;
  EFI_STRING                        FamilyName;
  STRING_REF                        TokenToGet;
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  SMBIOS_TABLE_TYPE1                *SmbiosRecord;
  EFI_MISC_SYSTEM_MANUFACTURER      *ForType1InputData;
  CHAR16                            Buffer[40];
  CHAR16                            PlatformNameBuffer[40];
  CHAR16                            *MacAddressString = NULL;
  CHAR16                            SerialNumberBuffer[sizeof (L"112233445566")];
  EFI_SMBIOS_PROTOCOL               *Smbios;
  SYSTEM_CONFIGURATION              SetupVarBuffer;
  UINTN                             VariableSize;
  CHAR16                            *Uuid;
  UINT64                            TempData;
  UINTN                             Index;
  static BOOLEAN                    RemoveTable = TRUE;
  EFI_SMBIOS_TABLE_HEADER           *Record;



if (Event != NULL) {
  gBS->CloseEvent (Event);   // Unload this event.
}

  DEBUG ((EFI_D_INFO, "Executing AddSmbiosManuCallback.\n"));


  ForType1InputData = (EFI_MISC_SYSTEM_MANUFACTURER *)Context;
  //
  // First check for invalid parameters.
  //
  if (Context == NULL || mPlatformInfo == NULL) {
    DEBUG ((EFI_D_INFO, "MISC_SMBIOS_TABLE_FUNCTION error.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  ASSERT_EFI_ERROR (Status);


  if (BOARD_ID_MINNOW2_TURBOT == mPlatformInfo->BoardId) {
  //
  // Detect the board is Turbot board platform
  //
  UnicodeSPrint (PlatformNameBuffer, sizeof (PlatformNameBuffer),L"%s",L"Minnowboard Turbot ");
  } else {
    UnicodeSPrint (PlatformNameBuffer, sizeof (PlatformNameBuffer),L"%s",L"Minnowboard Max ");
  }

  //
  // Silicon Steppings
  //
  switch (PchStepping()) {
    case PchA0:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"A0 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"A0");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "A0 Stepping Detected\n"));
      break;
    case PchA1:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"A1 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"A1");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "A1 Stepping Detected\n"));
      break;
    case PchB0:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"B0 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"B0");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "B0 Stepping Detected\n"));
      break;
    case PchB1:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"B1 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"B1");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "B1 Stepping Detected\n"));
      break;
    case PchB2:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"B2 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"B2");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "B2 Stepping Detected\n"));
      break;
    case PchB3:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"B3 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"B3");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "B3 Stepping Detected\n"));
      break;
    case PchC0:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"C0 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"C0");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "C0 Stepping Detected\n"));
      break;
   case PchD0:
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s%s", PlatformNameBuffer, L"D0/D1 PLATFORM");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_PRODUCT_NAME), Buffer, NULL);
      UnicodeSPrint (Buffer, sizeof (Buffer),L"%s",L"D0/D1");
      HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_VERSION), Buffer, NULL);
      DEBUG ((EFI_D_ERROR, "D0/D1 Stepping Detected\n"));
      break;
    default:
    DEBUG ((EFI_D_ERROR, "Unknow Stepping Detected\n"));
      break;
  }

  if (BOARD_ID_MINNOW2_TURBOT == mPlatformInfo->BoardId) {
    UnicodeSPrint (Buffer, sizeof (Buffer),L"Silicom");
    HiiSetString(mHiiHandle,STRING_TOKEN(STR_MISC_SYSTEM_MANUFACTURER), Buffer, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_MANUFACTURER);
  Manufacturer = SmbiosMiscGetString (TokenToGet);
  ManuStrLen = StrLen(Manufacturer);
  if (ManuStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_PRODUCT_NAME);
  ProductName = SmbiosMiscGetString (TokenToGet);
  PdNameStrLen = StrLen(ProductName);
  if (PdNameStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_VERSION);
  Version = SmbiosMiscGetString (TokenToGet);
  VerStrLen = StrLen(Version);
  if (VerStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }


  MacAddressString = GetMacAddressString();

  if ( MacAddressString != NULL) {
    UnicodeSPrint (SerialNumberBuffer, sizeof (L"112233445566"), L"%s", GetMacAddressString());
    HiiSetString (mHiiHandle, STRING_TOKEN (STR_MISC_SYSTEM_SERIAL_NUMBER), SerialNumberBuffer, NULL);
  }
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_SERIAL_NUMBER);
  SerialNumber = SmbiosMiscGetString (TokenToGet);
  SerialNumStrLen = StrLen(SerialNumber);
  if (SerialNumStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_SKU_NUMBER);
  SkuNumber = SmbiosMiscGetString (TokenToGet);
  SkuNumberStrLen = StrLen(SkuNumber);
  if (SkuNumberStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }
  TokenToGet = STRING_TOKEN (STR_MISC_SYSTEM_FAMILY_NAME1);
  FamilyName = SmbiosMiscGetString (TokenToGet);
  FamilyNameStrLen = StrLen(FamilyName);
  if (FamilyNameStrLen > SMBIOS_STRING_MAX_LENGTH) {
    return EFI_UNSUPPORTED;
  }

  //
  // Two zeros following the last string.
  //
  SmbiosRecord = AllocatePool(sizeof (SMBIOS_TABLE_TYPE1) + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + SkuNumberStrLen + 1 + FamilyNameStrLen + 1 + 1);
  ZeroMem(SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE1) + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + SkuNumberStrLen + 1 + FamilyNameStrLen + 1 + 1);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;
  SmbiosRecord->Hdr.Length = sizeof (SMBIOS_TABLE_TYPE1);

  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  //
  // Manu will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Manufacturer = 1;

  //
  // ProductName will be the 2nd optional string following the formatted structure.
  //
  SmbiosRecord->ProductName = 2;

  //
  // Version will be the 3rd optional string following the formatted structure.
  //
  SmbiosRecord->Version = 3;

  //
  // Version will be the 4th optional string following the formatted structure.
  //
  SmbiosRecord->SerialNumber = 4;

  SmbiosRecord->SKUNumber= 5;
  SmbiosRecord->Family= 6;

  //
  // Unique UUID
  //
  ForType1InputData->SystemUuid.Data1 = PcdGet32 (PcdProductSerialNumber);
  ForType1InputData->SystemUuid.Data4[0] = PcdGet8 (PcdEmmcManufacturerId);
  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  L"Setup",
                  &mSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  &SetupVarBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  Uuid = AllocatePool(sizeof(SetupVarBuffer.SystemUuid));
  ASSERT (Uuid != NULL);
  CopyMem (Uuid, SetupVarBuffer.SystemUuid, sizeof(SetupVarBuffer.SystemUuid));

  if(StrLen(Uuid) != 0) {
    DEBUG ((EFI_D_INFO, "CustomerUuid %s\n",SetupVarBuffer.SystemUuid));
    ForType1InputData->SystemUuid.Data1 = (UINT32) StrHexToUint64 (Uuid);
    ForType1InputData->SystemUuid.Data2 = (UINT16) StrHexToUint64 (Uuid + 9);
    ForType1InputData->SystemUuid.Data3 = (UINT16) StrHexToUint64 (Uuid + 14);
    ForType1InputData->SystemUuid.Data4[0] = (UINT8) (StrHexToUint64 (Uuid + 19) >> 8);
    ForType1InputData->SystemUuid.Data4[1] = (UINT8) StrHexToUint64 (Uuid + 21);
    TempData = StrHexToUint64 (Uuid + 24);
    for(Index = sizeof(ForType1InputData->SystemUuid.Data4)/sizeof(UINT8); Index > 2; Index--) {
      ForType1InputData->SystemUuid.Data4[Index-1] = (UINT8) TempData;
      TempData = TempData >> 8;
    }
  } else if (MacAddressString != NULL) {
    ForType1InputData->SystemUuid.Data1 = (UINT32)MacAddressString [0] + (((UINT32)MacAddressString [1]) << 16);
    ForType1InputData->SystemUuid.Data2 = (UINT16)MacAddressString [2];
    ForType1InputData->SystemUuid.Data3 = (UINT16)MacAddressString [3];
    ForType1InputData->SystemUuid.Data4[0] = (UINT8)MacAddressString [4];
    ForType1InputData->SystemUuid.Data4[1] = (UINT8)(MacAddressString [5]);
    ForType1InputData->SystemUuid.Data4[2] = (UINT8)MacAddressString [6];
    ForType1InputData->SystemUuid.Data4[3] = (UINT8)(MacAddressString [7]);
    ForType1InputData->SystemUuid.Data4[4] = (UINT8)(MacAddressString [8]);
    ForType1InputData->SystemUuid.Data4[5] = (UINT8)(MacAddressString [9]);
    ForType1InputData->SystemUuid.Data4[6] = (UINT8)(MacAddressString [10]);
    ForType1InputData->SystemUuid.Data4[7] = (UINT8)(MacAddressString [11]);
  }

  CopyMem ((UINT8 *) (&SmbiosRecord->Uuid),&ForType1InputData->SystemUuid,16);


  SmbiosRecord->WakeUpType = (UINT8)ForType1InputData->SystemWakeupType;

  OptionalStrStart = (CHAR8 *)(SmbiosRecord + 1);
  UnicodeStrToAsciiStr(Manufacturer, OptionalStrStart);
  UnicodeStrToAsciiStr(ProductName, OptionalStrStart + ManuStrLen + 1);
  UnicodeStrToAsciiStr(Version, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1);
  UnicodeStrToAsciiStr(SerialNumber, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1);

  UnicodeStrToAsciiStr(SkuNumber, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1);
  UnicodeStrToAsciiStr(FamilyName, OptionalStrStart + ManuStrLen + 1 + PdNameStrLen + 1 + VerStrLen + 1 + SerialNumStrLen + 1 + SkuNumberStrLen +1);

  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  if (RemoveTable == TRUE) {
    Status = EFI_SUCCESS;
    while (!EFI_ERROR(Status)) {
      Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
      if (Record->Type == SMBIOS_TYPE_SYSTEM_INFORMATION) {
        Status = Smbios-> Remove(
                            Smbios,
                            SmbiosHandle
                            );
        RemoveTable = FALSE;
        break;
      }
   }
 }

  Status = Smbios-> Add(
                      Smbios,
                      NULL,
                      &SmbiosHandle,
                      (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord
                      );
  FreePool(SmbiosRecord);

  return Status;
}


/**

  Publish the smbios type 1.

  @param Event      Event whose notification function is being invoked (gEfiDxeSmmReadyToLockProtocolGuid).
  @param Context    Pointer to the notification functions context, which is implementation dependent.

  @retval None

**/
MISC_SMBIOS_TABLE_FUNCTION(MiscSystemManufacturer)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  static BOOLEAN        CallbackIsInstalledManu = FALSE;
  EFI_EVENT             AddSmbiosManuCallbackEvent;


  Status = AddSmbiosManuCallback (NULL, RecordData);

  if (CallbackIsInstalledManu == FALSE) {
    CallbackIsInstalledManu = TRUE;   // Prevent more than 1 callback.
    DEBUG ((EFI_D_INFO, "Create Smbios Manu callback.\n"));


    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               (EFI_EVENT_NOTIFY)AddSmbiosManuCallback,
               RecordData,
               &AddSmbiosManuCallbackEvent
               );
  }

  return Status;

}

