/**
 * Copyright (c) 2011 Zillians Inc, Taiwan. All Rights Reserved.
 *
 * For more information about Zillians, you can take a look at
 *
 *    http://www.zillians.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cg/UnicodeUtil.h"
#include <cstring>

#define UNKNOWN_CHAR '?'

namespace zillians {

void utf8_to_ucs4(const std::string& input, std::wstring& output)
{
	typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;
	iterator_type first(input.begin());
	iterator_type last(input.end());
	std::copy(first, last, std::back_inserter(output));
}

void ucs4_to_utf8(const std::wstring& input, std::string& output)
{
	typedef boost::u32_to_u8_iterator<std::wstring::const_iterator> iterator_type;
	iterator_type first(input.begin());
	iterator_type last(input.end());
	std::copy(first, last, std::back_inserter(output));
}

void wcs_to_cstr(const wchar_t* src, char* dest)
{
    static std::locale ascii_locale;
    std::use_facet<std::ctype<wchar_t> >(ascii_locale).narrow(src, src + std::wcslen(src), UNKNOWN_CHAR, &dest[0]);
}

void cstr_to_wcs(const char* src, wchar_t* dest)
{
    static std::locale ascii_locale;
    std::use_facet<std::ctype<wchar_t> >(ascii_locale).widen(src, src + strlen(src), &dest[0]);
}

std::wstring s_to_ws(std::string s)
{
    std::wstring ws;
    wchar_t* wcs = new wchar_t[s.length() + 1];
    wcs[s.length()] = L'\0';
    cstr_to_wcs(s.c_str(), wcs);
    ws = wcs;
    delete []wcs;
    return ws;
}

std::string ws_to_s(std::wstring ws)
{
    std::string s;
    char* cstr = new char[ws.length() + 1];
    cstr[ws.length()] = '\0';
    wcs_to_cstr(ws.c_str(), cstr);
    s = cstr;
    delete []cstr;
    return s;
}

}
