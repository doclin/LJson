#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "parser.h"
#include "json.h"

using namespace std;

int main(int argc, char* argv[])
{
	bool pretty = false;
	const char* path = NULL;

	if(argc == 2)
		path = argv[1];
	else if(argc == 3)
	{
		if(strcmp("-pretty", argv[1]) == 0)
			pretty = true;
		else
		{
			cout << "please input valid command" << endl;
			return 0;
		}
		path = argv[2];
	}
	else
	{
		cout << "please input valid command" << endl;
		return 0;
	}

	string document;
	ifstream input_file;
	input_file.open(path);
	if(input_file.is_open())
	{
		stringstream sstr;
		sstr << input_file.rdbuf();
		document = sstr.str();
		input_file.close();
	}
	else
	{
		cout << "unable to open file" << endl;
		return 0;
	}

	Parser parser;
	try
	{
		parser.parse(document.c_str());
		cout << "valid" << endl;
	}
	catch(ParseException e)
	{
		cout << e.code << "(line " << e.line << ", position " << e.position << ") : " << e.get_msg() << endl;
		return 0;
	}
	catch(JsonException e)
	{
		cout << e.code << ": something wrong...";
		return 0;
	}

	if(pretty)
	{
		Json j = parser.get_json();
		string new_path = path;
		new_path.erase(new_path.size()-5, 5);
		new_path.append(".pretty.json");

		ofstream output_file;
		output_file.open(new_path.c_str());
		if(output_file.is_open())
		{
			output_file.write(j.get_pretty_json(), strlen(j.get_pretty_json()));
			output_file.close();
			cout << j.get_pretty_json() << endl;
		}
		else
		{
			cout << "cannot write pretty into file" << endl;
			return 0;
		}
	}
	

	return 0;
}