#include "mem.h"
#include "pe.h"
#include <WinBase.h>


int main() {
	LDR_DATA_TABLE_ENTRY* modEntry;

	printf("%p \n",WinExec);
	modEntry = getModEntry(L"kernel32.dll");
	printf("DLL baseaddr: 0x%p\n", modEntry->DllBase);
	auto addr = getFuncAddr(modEntry->DllBase);
	printf("Function addr: 0x%p\n", addr);

	typedef int (*FunctionType)(LPCSTR,UINT);
	FunctionType function = (FunctionType)addr;
	LPCSTR temp = "calc.exe";
	function(temp,0);

	return 0;
}
