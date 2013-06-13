#include <stdio.h>
#include <time.h>
#include "rewrite.h"
typedef __time64_t (__cdecl *time64type)(__time64_t* t);
time64type orgtime;
__time64_t __cdecl mytime64(__time64_t* t)
{
	__time64_t value = orgtime(t);
	value += 60 * 60 * 24; // 1日進める
	if(t)*t = value;
	return value;
}
int main()
{
	printf("time: %d\n", time(NULL));
	orgtime = (time64type)RewriteFunction("MSVCR100D.dll", "_time64", mytime64);
	printf("time: %d\n", time(NULL));
	RewriteFunction("MSVCR100D.dll", "_time64", orgtime); // 戻す
	return 0;
}
