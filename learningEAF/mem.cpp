#include "mem.h"
#include <windows.h>
#include "ntapi.h"
uintptr_t GetModuleBaseAddress(DWORD procID, const wchar_t* modName)
{

	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32  modEntry;
		//Before calling the Process32First function, set this member to sizeof(PROCESSENTRY32). If you do not initialize dwSize, Process32First fails.
		modEntry.dwSize = sizeof(modEntry);
		// Loop till we find our module
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

/*
Internal PEB functions
*/

PEB* GetPEBInternal()
{
#ifdef _WIN64
	PEB* peb = (PEB*)__readgsqword(0x60);
#else
	PEB* peb = (PEB*)__readfsdword(0x30);
#endif
	return peb;
}

LDR_DATA_TABLE_ENTRY* GetLDREntryInternal(const wchar_t* modName)
{
	LDR_DATA_TABLE_ENTRY* modEntry = nullptr;
	PEB* peb = GetPEBInternal();
	LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;
	LIST_ENTRY curr = head;

	for (auto curr = head; curr.Flink != &peb->Ldr->InMemoryOrderModuleList; curr = *curr.Flink)
	{
		LDR_DATA_TABLE_ENTRY* mod = (LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		if (mod->BaseDllName.Buffer)
		{
			wprintf_s(mod->FullDllName.Buffer);
			std::cout << std::endl;
			if (_wcsicmp(modName, mod->BaseDllName.Buffer) == 0)
			{
				modEntry = mod;
				//break, for experimenting lets just print out everything
			}
		}
	}
	std::cout << std::endl;
	return modEntry;
}

char* GetModuleBaseAddressInternalPEB(const wchar_t* modName)
{
	LDR_DATA_TABLE_ENTRY* modEntry = GetLDREntryInternal(modName);
	return (char*)modEntry->DllBase;
}

LDR_DATA_TABLE_ENTRY* getModEntry(const wchar_t* modName) {
	LDR_DATA_TABLE_ENTRY* modEntry = GetLDREntryInternal(modName);
	return modEntry;
}
