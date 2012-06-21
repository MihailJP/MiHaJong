/*=============================
 * HSP麻雀クライアントMiHaJong
 *      [専用DLLの設定]
 *=============================
 */

#uselib "mjlib.dll"

/* ロギングツール */
/*
#func global _initlog_ "?initLogger@logger@@YAHXZ"
#func global _trace_msg_ "?trace_msg@logger@@YAXPBD@Z" str
#func global _debug_msg_ "?debug_msg@logger@@YAXPBD@Z" str
#func global _info_msg_ "?info_msg@logger@@YAXPBD@Z" str
#func global _warn_msg_ "?warn_msg@logger@@YAXPBD@Z" str
#func global _error_msg_ "?error_msg@logger@@YAXPBD@Z" str
#func global _fatal_msg_ "?fatal_msg@logger@@YAXPBD@Z" str
*/

/* ルールコンフィグ */
/*
#func global _storeRule "?storeRule@@YAXPAPAD@Z" var
#func global _exportRule "?exportRule@@YAXPAPAD@Z" var
*/
#cfunc global getRule "?getRule@@YAHH@Z" int
