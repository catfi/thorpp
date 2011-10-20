/**
 * Zillians
 * Copyright (C) 2011-2012 Zillians.com
 * For more information see http://www.zillians.com
 */

#include "cg/Prerequisite.h"
#include "cg/UnicodeUtil.h"

#include "cg/grammar/CppGrammar.h"
#include "cg/action/CppAction.h"
#include "cg/context/GeneratorContext.h"

#include <boost/filesystem.hpp>

using namespace zillians;
using namespace zillians::cg;

namespace classic = boost::spirit::classic;
namespace qi = boost::spirit::qi;

// since '\t' may be printed in spaces and I don't know a way to change std::wcout, we simply replace the tab with desired number of spaces
// so that we can have correct error pointing cursor
// (note that because '\t' equals to 4 spaces reported by spirit, we have to make sure it's printed in the exact same way)
static void expand_tabs(const std::wstring& input, std::wstring& output, int number_of_space_for_tab = 4)
{
	for(std::wstring::const_iterator it = input.begin(); it != input.end(); ++it)
	{
		if(*it == '\t')
			for(int i=0;i<number_of_space_for_tab;++i) output.push_back(L' ');
		else
			output.push_back(*it);
	}
}

int process_cpp_source(const std::string& to, const std::vector<std::string>& sources)
{
	if(to == "cpp")
	{
		for(auto i = sources.begin(); i != sources.end(); ++i)
		{
			boost::filesystem::path path(*i);
			if(strcmp(path.extension().c_str(), ".giggle") == 0)
			{
				std::string filename = *i;
				std::ifstream in(filename, std::ios_base::in);

				if(!in.good())
				{
					std::cerr << "Error: failed to open source file: " << filename << std::endl;
					return -1;
				}

				// ignore the BOM marking the beginning of a UTF-8 file in Windows
				if(in.peek() == '\xef')
				{
					char s[3];
					in >> s[0] >> s[1] >> s[2];
					s[3] = '\0';
					if (s != std::string("\xef\xbb\xbf"))
					{
						std::cerr << "Error: unexpected characters from input file: " << filename << std::endl;
						return -1;
					}
				}

				std::string source_code_raw;
				in.unsetf(std::ios::skipws); // disable white space skipping
				std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(), std::back_inserter(source_code_raw));

				// convert it from UTF8 into UCS4 as a string by using u8_to_u32_iterator
				std::wstring source_code;
				utf8_to_ucs4(source_code_raw, source_code);

				// try parse
				typedef classic::position_iterator2<std::wstring::iterator> pos_iterator_type;
				try
				{
					pos_iterator_type begin(source_code.begin(), source_code.end(), s_to_ws(filename));
					pos_iterator_type end;

					grammar::CppGrammar<pos_iterator_type, action::CppAction> parser;

					if(!qi::parse(
							begin, end,
							parser))
					{
						std::cerr << "Error: unrecognized parser error" << std::endl;
						return -1;
					}
					else
					{
						std::wcout << context::GeneratorContext::instance()->buffer.str();
					}
				}
				catch (const qi::expectation_failure<pos_iterator_type>& e)
				{
					const classic::file_position_base<std::wstring>& pos = e.first.get_position();

					std::wstring current_line;
					expand_tabs(e.first.get_currentline(), current_line);
					std::wcerr << L"Error: parse error at file " << pos.file << L" line " << pos.line
							<< L" column " << pos.column << std::endl
							<< L"'" << current_line << L"'" << std::endl
							<< std::setw(pos.column) << L" " << L"^- here" << std::endl;

					return -1;
				}
			}
		}

		return 0;
	}

	std::cerr << "unrecognized target language" << std::endl;
	return -1;
}
