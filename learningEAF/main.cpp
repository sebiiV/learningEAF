#include "mem.h"
#include "pe.h"


int main() {
	LDR_DATA_TABLE_ENTRY* modEntry;
	modEntry = GetModEntry(L"kernel32.dll");
	printf("dll baseaddr: %p\n", modEntry->DllBase);
	readPeHeader(modEntry->DllBase);
	return 0;
}
