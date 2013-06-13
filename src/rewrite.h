/*! 関数コール書き換え。
	@return 元の関数アドレス
*/
void* RewriteFunction(
	const char*	szRewriteModuleName,	//!< [in] 書き替えたいモジュール名
	const char*	szRewriteFunctionName,	//!< [in] 書き替えたい関数名
	void*		pRewriteFunctionAddress	//!< [in] 書き替え後の関数アドレス
);
void PrintFunctions();
