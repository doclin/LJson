# LJson C++ json解析/生成器 
by WHU Doclin

## Feature
* 支持符合ECMA-404标准的json解析，并提供友好的异常提示
* 支持UTF-8编码格式
* 支持对压缩传输格式的json串格式化
* 实现json数据的内部存储，并提供简洁接口供构建/查找json对象


## Installation
LJson使用cmake作为编译工具
###### On Linux:  
1. cd LJson/build
    cmake ..
2. make

在LJson/bin目录下即生成两个示例可执行文件main/main_addition

###### On Windows:
1. 使用cmake-gui创建solution
2. 使用生成的solution编译项目

输出可执行文件位于LJson/bin/Debug或LJson/bin/Release下

## Usage
Parse:

		#include "Parser.h"
		char* json_doc;
		Parser parser;
		parser.parse(json_doc);
Generator:

		#include "Json.h"
		Json json;
		json.new_array();
		json.new_number(1);
		json.end_array();
Query:

		const Json& newjson = parser.get_json();
		if(newjson["somekey"].flag == V_STRING)
			cout << newjson["somekey"].string << endl;
		if(json[0].flag == V_NUMBER)
			double i = json[0].number + 1;

## Architecture
		.
		├── json.cpp			// 实现Json类相关功能
		├── json_exception.h		// 提供自定义异常信息
		├── json.h			// 提供Json数据结构定义及外部接口
		├── lexical_analysis.cpp	// 实现Parser中词法分析部分
		├── parser.cpp			// 实现Parser对外接口
		├── parser.h			// 提供Parser定义及对外接口
		├── syntactic_analysis.cpp	// 实现Parser中语法分析部分
		└── utility.h			// 实现Parser中相关常量定义

