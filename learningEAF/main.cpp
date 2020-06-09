#include "mem.h"
#include "pe.h"


int main() {

	void* addr = nullptr;
	LDR_DATA_TABLE_ENTRY* modEntry;
	modEntry = getModEntry(L"kernel32.dll");
	printf("DLL baseaddr: 0x%p\n", modEntry->DllBase);
	addr = getFuncAddr(modEntry->DllBase);
	printf("Function addr: 0x%p\n", addr);

	typedef UINT (__stdcall *func)(LPCSTR,UINT);
	func function = (func)addr;
	//LPCSTR temp = "calc.exe";
	UINT i = function((LPCSTR)"calc.exe",(UINT)5);

	printf_s("\n calc popped!");
	return 0;
}
