#pragma once
#include <Windows.h>

typedef struct _UNICODE_STRING2 {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING2, * PUNICODE_STRING2;

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    PVOID RootDirectory;
    PUNICODE_STRING2 ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef _Null_terminated_ const wchar_t* NTSTRSAFE_PCWSTR;

#define InitOA(ptr, root, attrib, name, desc, qos) { (ptr)->Length = sizeof(OBJECT_ATTRIBUTES); (ptr)->RootDirectory = root; (ptr)->Attributes = attrib; (ptr)->ObjectName = name; (ptr)->SecurityDescriptor = desc; (ptr)->SecurityQualityOfService = qos; }
typedef NTSYSAPI NTSTATUS(NTAPI* __ntOpenProcess)(
    PHANDLE            ProcessHandle,
    ACCESS_MASK        DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PCLIENT_ID         ClientId
    );
