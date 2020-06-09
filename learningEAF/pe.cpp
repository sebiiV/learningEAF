#include <stdio.h>
#include "pe.h"

PIMAGE_DOS_HEADER pDOSHeader = {};
PIMAGE_NT_HEADERS pImageNTHeaders = {};
PIMAGE_EXPORT_DIRECTORY pExportImageDir = {};


/*
useful links
https://ired.team/offensive-security/code-injection-process-injection/finding-kernel32-base-and-function-addresses-in-shellcode
https://blog.kowalczyk.info/articles/pefileformat.html
*/

auto getFuncAddr(void* pheader)->void* {
	pDOSHeader = (PIMAGE_DOS_HEADER)pheader;
	/*
	e_lfanew, is a 4-byte offset into the file where the PE file header is located.
	It is necessary to use this offset to locate the PE header in the file
	*/
	pImageNTHeaders = (PIMAGE_NT_HEADERS)((size_t)pheader + pDOSHeader->e_lfanew);

	/*
	NumberOfRvaAndSizes. 
	This field identifies the length of the DataDirectory array that follows. 
	field is used to identify the size of the array, not the number of valid entries in the array.

	DataDirectory. The data directory indicates where to find other important components of executable
	information in the file. It is really nothing more than an array of IMAGE_DATA_DIRECTORY 
	structures that are located at the end of the optional header structure. 
	each element is a specific bit of info, can be see in winnt.h. Or see remarks in below
	https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_data_directory

	element 0 is export dir
	*/

	pExportImageDir = (PIMAGE_EXPORT_DIRECTORY)((size_t)pheader +
		pImageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	/*
	*AddressOfFunctions
	This field is an RVA and points to an array of function addresses. The function
	addresses are the entry points (RVAs) for each exported function in this module.

	AddressOfNames
	This field is an RVA and points to an array of string pointers. The strings 
	are the names of the exported functions in this module.
	*/
	auto name = (size_t*)((BYTE*)pheader + pExportImageDir->AddressOfNames); //we modern cpp now
	auto address = (size_t*)((BYTE*)pheader + pExportImageDir->AddressOfFunctions);
	auto ordinal = (WORD*)((BYTE*)pheader + pExportImageDir->AddressOfNameOrdinals);
	
	for (int i = 0; i < pExportImageDir->NumberOfFunctions; i++) {
		auto currentName = (char*)pheader + name[i];
		auto currentAddr = (size_t*)((BYTE*)pheader + address[ordinal[i]]);
		printf_s("%s :: %p \n",currentName,(char*)currentAddr);
		if (!strcmp(currentName, "WinExec")) {
			return currentAddr;
		}
	};
};
