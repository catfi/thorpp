/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#ifndef UNICODEUTIL_H_
#define UNICODEUTIL_H_

#include "cg/Prerequisite.h"

namespace zillians {

void utf8_to_ucs4(const std::string& input, std::wstring& output);
void ucs4_to_utf8(const std::wstring& input, std::string& output);

void wcs_to_cstr(const wchar_t* src, char* dest);
void cstr_to_wcs(const char* src, wchar_t* dest);
std::wstring s_to_ws(std::string s);
std::string ws_to_s(std::wstring ws);

}

#endif /* UNICODEUTIL_H_ */
