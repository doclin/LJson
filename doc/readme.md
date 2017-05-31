LJson	C++ json解析/生成器
by WHU Doclin

Feature:
支持符合ECMA-404标准的json解析，并提供友好的异常提示
支持UTF-8编码格式
支持对压缩传输格式的json串格式化
实现json数据的内部存储，并提供简洁接口供构建/查找json对象
内存占用小，且通过内存泄露测试
代码组织良好，可读性高

Installation:
LJson使用cmake作为编译工具
On Linux
1.
cd LJson/build
cmake ..
2.
make
在LJson/bin目录下即生成两个示例可执行文件main/main_addition
On Windows
1.使用cmake-gui创建solution
2.使用生成的solution编译项目

Usage:
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
也可查看Json/test目录下的main.cpp/main_addition.cpp了解使用

Architecture:
.
├── json.cpp			// 实现Json类相关功能
├── json_exception.h		// 提供自定义异常信息
├── json.h			// 提供Json数据结构定义及外部接口
├── lexical_analysis.cpp	// 实现Parser中词法分析部分
├── parser.cpp			// 实现Parser对外接口
├── parser.h			// 提供Parser定义及对外接口
├── syntactic_analysis.cpp	// 实现Parser中语法分析部分
└── utility.h			// 实现Parser中相关常量定义

因为偷懒(不会)所以没有UML图例，只好文字叙述内部结构：

LJson共有两部分对外接口，一部分是封装于parser.h的解析部分，另一部分是封装于json.h的json数据类，而parser的后端依赖于json类的构建接口。

当调用一个parser对象的parse接口时。parser对象会在进行部分初始化操作后执行词法分析。LJson的词法分析采用直接编码分析器的方式，直接基于字节流以兼容
UTF-8编码，最终输出基于顺序存储的自定义词素流。在进行语法分析时，LJson尝试使用了表驱动的构建方式读取词素流，使用预定义常量表示词素类别和转移码，并利用栈存储
推导中间过程。在语法逐步推导的同时，Json类的构建接口按照推导的顺序被调用。最终parser生成的json对象会被存储于成员变量中，可通过get_json()接口获取。

Json数据类的存储单元是结构体Value，可同时存储json所有值类型。Json数据类提供一系列构造接口可用于构建完整的树状json数据结构，构造中使用指针栈存储父节点便于回溯。在构造过程中，数组与对象的存储采用了动态数组的方案，字符串的存储选择了复制原字符串。为防止内存泄露，Json类严格定义了析构函数与拷贝构造函数。Json类同时重载了operator[]用于查询json值。类成员函数get_pretty_json()可生成格式化的json字符串，实现中主要应用递归完成字符串拼接。

实现过程中，在Json存储单元内存布局和对外接口设计上借鉴了Milo Yip的RapidJSON。














