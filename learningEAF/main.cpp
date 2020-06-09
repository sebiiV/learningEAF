#include "mem.h"
#include "pe.h"


int main() {
	LDR_DATA_TABLE_ENTRY* modEntry;
	modEntry = getModEntry(L"kernel32.dll");
	printf("DLL baseaddr: %p\n", modEntry->DllBase);
	auto addr = getFuncAddr(modEntry->DllBase);
	printf("Function addr: %p\n", addr);

	typedef int func(void);
	func* f = (func*)addr;
	int i = f();
	return 0;
}
