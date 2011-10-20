/**
 * Zillians MMO
 * Copyright (C) 2007-2011 Zillians.com, Inc.
 * For more information see http://www.zillians.com
 *
 * Zillians MMO is the library and runtime for massive multiplayer online game
 * development in utility computing model, which runs as a service for every
 * developer to build their virtual world running on our GPU-assisted machines.
 *
 * This is a close source library intended to be used solely within Zillians.com
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
