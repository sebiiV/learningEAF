#pragma once
#include "Windows.h"
/*
+8?
0x3c into the file RVA of PE signature
0x78 bytes after PE signature RVA of Export Table
0x14 into the Export Table Number of functions exported by a module
0x1c into the Export Table RVA of Address Table - addresses of exported functions
0x20 into the Export Table RVA of Name Pointer Table - addresses of exported function names
0x24 into the Export Table RVA of Ordinal Table - function order number as listed in the table
*/

void readPeHeader(void* pheader);
