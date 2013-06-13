#include <stdio.h>
#include <Windows.h>
#include "rewrite.h"

typedef HANDLE (WINAPI *CreateFileTypeW)(
	LPCWSTR name, DWORD dw1, DWORD dw2, LPSECURITY_ATTRIBUTES lps,
	DWORD dw3, DWORD dw4, HANDLE h);

CreateFileTypeW org;

HANDLE WINAPI MyCreateFileW(
	LPCWSTR name, DWORD dw1, DWORD dw2, LPSECURITY_ATTRIBUTES lps,
	DWORD dw3, DWORD dw4, HANDLE h)
{
	printf("CreateFileW: %ls\n", name);
	return org(name, dw1, dw2, lps, dw3, dw4, h);
}

int main()
{
	org = (CreateFileTypeW)RewriteFunction("KERNEL32.dll", "CreateFileW", MyCreateFileW);
	fopen("app.fopen", "rb");
	CreateFileW(L"app.createfileW", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CreateFileA("app.createfileA", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	RewriteFunction("KERNEL32.dll", "CreateFileW", org); // 戻す
	return 0;
}
