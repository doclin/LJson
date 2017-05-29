#include <iostream>
#include <fstream>
#include "parser.h"
#include "json.h"

using namespace std;

const char* s = 
"{\
    \"users\":[\
        {\
            \"name\":\"Alice\",\
            \"sex\":\"female\"\
        },\
		{\
            \"name\":\"Bob\",\
            \"sex\":\"male\",\
            \"fuck\":123,\
            \"shit\": 2.5,\
            \"what\" : 0.001e8,\
            \"omg\": \"取反\",\
            \"shit\": null,\
            \"s\": false,\
            \"what\" : true\
        }\
    ]\
}";

const char* ss = 
"{\
    \"users\": 122,\
    \"a\" : \"123\"\
}";


/*
int main(int argc, char* argv[])
{
	const char* file_name = argv[1];
	ifstream input_file;
	input_file.open(file_name);
	string line;
	Parser parser;

	if(input_file.is_open())
	{
		int line_num = 1;
		while(getline(input_file, line))
		{
			try
			{
				parser.parse(line);
			}
			catch(int e)
			{
				cout << e << endl;
				break;
			}
			line_num++;
		}
		input_file.close();
	}
	else
		cout << "open err" << endl;

	return 0;
}
*/

int main()
{
	//const string s();
	Parser parser;
	try
	{
		parser.parse(s);
		const Json& json = parser.get_json();
		cout << json["users"][0]["name"].flag << endl;		
	}
	catch(int e)
	{
		cout << e << endl;
	}


	
}