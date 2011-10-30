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

#include "cg/Prerequisite.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>

namespace po = boost::program_options;

extern int process_cpp_source(const std::string& to, const std::vector<std::string>& sources);
extern int process_java_source(const std::string& to, const std::vector<std::string>& sources);
extern int process_python_source(const std::string& to, const std::vector<std::string>& sources);

int process(const std::string& from, const std::string& to, const std::vector<std::string>& sources)
{
	// here we dispatch to different translator according to the from/to type
	if(from == "cpp")
	{
		return process_cpp_source(to, sources);
	}
	else if(from == "java")
	{
		return process_java_source(to, sources);
	}
	else if(from == "python")
	{
		return process_python_source(to, sources);
	}

	return -1;
}

int main(int argc, char** argv)
{
	po::options_description options_desc("Usage");
	options_desc.add_options()
			("help,h", "show this help")
			("input,i", po::value<std::vector<std::string>>(), "input files")
			("from,f", po::value<std::string>(), "given source file type type")
			("to,t", po::value<std::string>(), "target language for the stub function");

	po::positional_options_description positional_options;
	positional_options.add("input", -1);

	po::variables_map vm;
    try
    {
    	po::store(po::command_line_parser(argc, argv).options(options_desc).positional(positional_options).run(), vm);
    }
    catch(const po::error& e)
    {
    	std::cerr << "failed to parse command line: " << e.what() << std::endl;
    	std::cerr << std::endl << options_desc << std::endl;
    	return -1;
    }

	if(vm.count("help") > 0 || argc < 2)
	{
    	std::cerr << std::endl << options_desc << std::endl;
    	return 0;
	}

	if(vm.count("from") != 1)
	{
		std::cerr << "source format is not specified" << std::endl;
		std::cerr << std::endl << options_desc << std::endl;
		return -1;
	}

	if(vm.count("to") != 1)
	{
		std::cerr << "target language is not specified" << std::endl;
		std::cerr << std::endl << options_desc << std::endl;
		return -1;
	}

	if(vm.count("input") == 0)
	{
		std::cerr << "Error: no input file is given" << std::endl;
		std::cerr << std::endl << options_desc << std::endl;
		return -1;
	}

	return process(vm["from"].as<std::string>(), vm["to"].as<std::string>(), vm["input"].as<std::vector<std::string>>());
}
