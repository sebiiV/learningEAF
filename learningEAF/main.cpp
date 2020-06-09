#include "mem.h"
#include "pe.h"


//#define calc

int main(int ) {

	void* addr = nullptr;
	LDR_DATA_TABLE_ENTRY* modEntry;
	modEntry = getModEntry(L"kernel32.dll");
	printf("DLL baseaddr: 0x%p\n", modEntry->DllBase);




// hacky way of running winexec or virtualalloc
#ifdef calc
	addr = getFuncAddr(modEntry->DllBase, (char*)"WinExec");
	printf("Function addr: 0x%p\n", addr);

	/*
	UINT WinExec(
		LPCSTR lpCmdLine,
		UINT   uCmdShow
	*/
	typedef UINT(__stdcall* func)(LPCSTR, UINT);
	func function = (func)addr;
	function((LPCSTR)"calc.exe", (UINT)5);
#else
	addr = getFuncAddr(modEntry->DllBase, (char*)"VirtualAlloc");
	printf("Function addr: 0x%p\n", addr);

	/*
	LPVOID VirtualAlloc(
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD  flAllocationType,
		DWORD  flProtect
	*/
	typedef LPVOID(__stdcall* func)(LPVOID, SIZE_T,DWORD,DWORD);
	func function = (func)addr;
	LPVOID temp;
	temp=function((LPVOID)0,(DWORD)1024,MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	printf_s("1024 bytes allocated at 0x%p \n", temp);
#endif 


	printf_s("\nfunction called");
	return 0;
}
