#define wcslen m_wcslen
#define PHNT_MODE PHNT_MODE_USER 
#include <phnt_windows.h>
#include <phnt.h>
#include <ntddkbd.h>
#include <stddef.h>
#include <ntrtl.h>
#pragma comment(lib, "ntdll.lib")

//AKT 1
size_t m_wcslen(const wchar_t* s) {
	const wchar_t* p = s;
	while (*p) p++;
	return p - s;
}

void StrDisplay(PCWSTR sourceString, UCHAR displayTimeSeconds) {
	UNICODE_STRING msg1;
	RtlInitUnicodeString(&msg1, sourceString);

	NtDisplayString(&msg1);

	LARGE_INTEGER delay;
	delay.QuadPart = -((LONGLONG)displayTimeSeconds * 1000000);
	NtDelayExecution(FALSE, &delay);
}

void StrBootDraw(PCWSTR sourceString, UCHAR displayTimeSeconds) {
	UNICODE_STRING msg1; 
	RtlInitUnicodeString(&msg1, sourceString);

	NtDrawText(&msg1);

	LARGE_INTEGER delay;

	delay.QuadPart = -((LONGLONG)displayTimeSeconds * 1000000);
	NtDelayExecution(FALSE, &delay);
}


void ShowError() {

	StrBootDraw(L"Error encoured...\n", 20);
	StrDisplay(L"Error encoured...\n", 20);
	NtTerminateProcess(NtCurrentProcess(), 0x69);


}
// AKT 2
NTSTATUS NTAPI NtProcessStartup(PPEB PEB) {
	

	HANDLE file;
	OBJECT_ATTRIBUTES attrs;
	UNICODE_STRING msg;
	RtlInitUnicodeString(&msg, L"\\??\\C:\\Users\\ghasdhc\\Desktop\\szewcy.txt");  //HARDCODED PATH
	InitializeObjectAttributes(&attrs, &msg, OBJ_CASE_INSENSITIVE, NULL, NULL);
	IO_STATUS_BLOCK skip;
	NTSTATUS stat = NtCreateFile(&file, FILE_GENERIC_READ, &attrs, &skip, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured1...\n", 20);
		StrDisplay(L"Error encoured1...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	HANDLE hProc = NtCurrentProcess();
	
	FILE_STANDARD_INFORMATION fileInfo;
	SIZE_T size;
	 stat =  NtQueryInformationFile(file, &skip, &fileInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
	if (!(NT_SUCCESS(stat))) {
	}
	if (NT_SUCCESS(stat)) {
		size = fileInfo.EndOfFile.QuadPart;
	}
	else {
		size = 148574; //size of szewcy.txt (polish version) as a fallback option - u might want to change it
	}          
	SIZE_T oryginalFileSize = size;
	PVOID mem = NULL;

	stat = NtAllocateVirtualMemory(hProc, &mem, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!(NT_SUCCESS(stat))) {
		NtClose(file);
		StrBootDraw(L"Error encoured2...\n", 20);
		StrDisplay(L"Error encoured2...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	stat = NtReadFile(file, NULL, NULL, NULL, &skip, mem, (ULONG)oryginalFileSize, NULL, NULL);
	if (!(NT_SUCCESS(stat))) {
		NtClose(file);
		StrBootDraw(L"Error encoured3...\n", 20);
		StrDisplay(L"Error encoured3...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	
	SIZE_T size2 = 1;
	PVOID mem2 = NULL;
	
	stat = NtAllocateVirtualMemory(NtCurrentProcess(), &mem2, 0, &size2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured4...\n", 20);
		StrDisplay(L"Error encoured4...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	PSIZE_T memNum = mem2;
	mem2 = (char*)mem2+8; 
	HANDLE file2;
	OBJECT_ATTRIBUTES attrs2;
	UNICODE_STRING LocationOfBinFile;
	 RtlInitUnicodeString(&LocationOfBinFile, L"\\??\\C:\\Users\\ghasdhc\\Desktop\\szewcy.bin"); //HARDOCODED PATH
	
	
	InitializeObjectAttributes(&attrs2, &LocationOfBinFile, OBJ_CASE_INSENSITIVE, NULL, NULL); 
	SIZE_T offset = 0;
	stat = NtCreateFile(&file2, FILE_GENERIC_READ | FILE_GENERIC_WRITE, &attrs2, &skip, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!(NT_SUCCESS(stat))) {
		offset = 0;  //but u might want to choose other offset
		goto skip;
	}
	stat = NtReadFile(file2, NULL, NULL, NULL, &skip, memNum, 8, NULL, NULL);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured6...\n", 20);
		StrDisplay(L"Error encoured6...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}

	 offset = *memNum;
	offset %= oryginalFileSize; 
	offset++;

	skip:
	SIZE_T lastChar = offset;
	for (;lastChar < oryginalFileSize && ((char*)mem)[lastChar] != '\n';lastChar++); 
	LARGE_INTEGER offset_file;
	offset_file.QuadPart = 0;
	stat = NtWriteFile(file2, NULL, NULL, NULL, &skip, &lastChar, 8, &offset_file, NULL);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured7...\n", 20);
		StrDisplay(L"Error encoured7...\n", 20);
		//NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	//AKT 3
	STRING utf8string;
	utf8string.Buffer = (char*)mem + offset;
	utf8string.Length = (USHORT)(lastChar - offset);
	utf8string.MaximumLength = utf8string.Length + 2;

	

	UNICODE_STRING line;
	line.MaximumLength = utf8string.MaximumLength * sizeof(WCHAR);
	line.Buffer = mem2;
	line.Length = utf8string.Length*2;
	stat = RtlUTF8StringToUnicodeString(&line, &utf8string,0);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured8...\n", 20);
		StrDisplay(L"Error encoured8...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}


	LARGE_INTEGER delay;
	delay.QuadPart = -(50 * 1000000);
	
	stat = NtDisplayString(&line);
	if (!(NT_SUCCESS(stat))) {
		StrBootDraw(L"Error encoured9...\n", 20);
		StrDisplay(L"Error encoured9...\n", 20);
		NtTerminateProcess(NtCurrentProcess(), 0x69);
	}
	//NtDrawText(&line); //u might like this more (now there is NtDisplayString)
	NtDelayExecution(FALSE, &delay);
	NtTerminateProcess(hProc, 0);
}


/*
Trzeba mieć duży takt.
By skończyć trzeci akt.
To nie złudzenie — to fakt.
Koniec aktu trzeciego i ostatniego
*/
