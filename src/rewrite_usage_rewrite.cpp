#include <stdio.h>
#include "rewrite.h"

int __cdecl mygetchar(void)
{
	return 'A';
}
int main()
{
	void* p = RewriteFunction("MSVCR100D.dll", "getchar", mygetchar);
	printf("%c\n", getchar());
	RewriteFunction("MSVCR100D.dll", "getchar", p); // 戻す
	return 0;
}
