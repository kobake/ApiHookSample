#include <stdio.h>
#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib,"imagehlp.lib")

void* RewriteFunctionImp(const char* szRewriteModuleName, const char* szRewriteFunctionName, void* pRewriteFunctionPointer)
{
	for(int i = 0; i < 2; i++){
		// ベースアドレス
		DWORD dwBase = 0;
		if(i == 0){
			if(szRewriteModuleName){
				dwBase = (DWORD)(intptr_t)::GetModuleHandleA(szRewriteModuleName);
			}
		}
		else if(i == 1){
			dwBase = (DWORD)(intptr_t)GetModuleHandle(NULL);
		}
		if(!dwBase)continue;

		// イメージ列挙
		ULONG ulSize;
		PIMAGE_IMPORT_DESCRIPTOR pImgDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData((HMODULE)(intptr_t)dwBase, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
		for(; pImgDesc->Name; pImgDesc++){
			const char* szModuleName = (char*)(intptr_t)(dwBase+pImgDesc->Name);
			// THUNK情報
			PIMAGE_THUNK_DATA pFirstThunk = (PIMAGE_THUNK_DATA)(intptr_t)(dwBase+pImgDesc->FirstThunk);
			PIMAGE_THUNK_DATA pOrgFirstThunk = (PIMAGE_THUNK_DATA)(intptr_t)(dwBase+pImgDesc->OriginalFirstThunk);
			// 関数列挙
			for(;pFirstThunk->u1.Function; pFirstThunk++, pOrgFirstThunk++){
				if(IMAGE_SNAP_BY_ORDINAL(pOrgFirstThunk->u1.Ordinal))continue;
				PIMAGE_IMPORT_BY_NAME pImportName = (PIMAGE_IMPORT_BY_NAME)(intptr_t)(dwBase+(DWORD)pOrgFirstThunk->u1.AddressOfData);
				if(!szRewriteFunctionName){
					// 表示のみ
					printf("Module:%s Hint:%d, Name:%s\n", szModuleName, pImportName->Hint, pImportName->Name);
				}
				else{
					// 書き換え判定
					if(stricmp((const char*)pImportName->Name, szRewriteFunctionName) != 0)continue;

					// 保護状態変更
					DWORD dwOldProtect;
					if( !VirtualProtect(&pFirstThunk->u1.Function, sizeof(pFirstThunk->u1.Function), PAGE_READWRITE, &dwOldProtect) )
						return NULL; // エラー

					// 書き換え
					void* pOrgFunc = (void*)(intptr_t)pFirstThunk->u1.Function; // 元のアドレスを保存しておく
					WriteProcessMemory(GetCurrentProcess(), &pFirstThunk->u1.Function, &pRewriteFunctionPointer, sizeof(pFirstThunk->u1.Function), NULL);
					pFirstThunk->u1.Function = (DWORD)(intptr_t)pRewriteFunctionPointer;

					// 保護状態戻し
					VirtualProtect(&pFirstThunk->u1.Function, sizeof(pFirstThunk->u1.Function), dwOldProtect, &dwOldProtect);
					return pOrgFunc; // 元のアドレスを返す
				}
			}
		}
	}
	return NULL;
}

void* RewriteFunction(const char* szRewriteModuleName, const char* szRewriteFunctionName, void* pRewriteFunctionPointer)
{
	return RewriteFunctionImp(szRewriteModuleName, szRewriteFunctionName, pRewriteFunctionPointer);
}

void PrintFunctions()
{
	printf("----\n");
	RewriteFunctionImp(NULL, NULL, NULL);
	printf("----\n");
}
