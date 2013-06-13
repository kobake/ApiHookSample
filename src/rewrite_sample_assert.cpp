#include <assert.h>
#include <stdio.h>
#include "rewrite.h"
void __cdecl _wassert2(const wchar_t* expr, const wchar_t* filename, unsigned lineno)
{
	// ※ここで filename や lineno で条件チェックを行うほうがより丁寧ですが
	// 　とりあえず今回は全スキップしてみました
}
int main()
{
	void* p = RewriteFunction("MSVCR100D.dll", "_wassert", _wassert2);
	printf("start\n");
	assert(0); // このassertはスルーされる
	printf("end\n");
	RewriteFunction("MSVCR100D.dll", "_wassert", p); // 戻す
	return 0;
}
