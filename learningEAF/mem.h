#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include "ntapi.h"
PEB* GetPEBInternal();
LDR_DATA_TABLE_ENTRY* GetLDREntryInternal(const wchar_t* modName);
char* GetModuleBaseAddressInternalPEB(const wchar_t* modName);
LDR_DATA_TABLE_ENTRY* GetModEntry(const wchar_t* modName);

