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
