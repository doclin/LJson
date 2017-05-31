#include <iostream>
#include "json.h"

using namespace std;

/*
{
	"user" : ["a", "b"],
	"property" :{
		"gg" : 1,
		"hh" : true
	}
}
*/

int main()
{
	Json json;
	try
	{
		json.new_obj();
		json.new_key("user");
		json.new_array();
		json.new_string("a");
		json.new_string("b");
		json.end_array();
		json.new_key("property");
		json.new_obj();
		json.new_key("gg");
		json.new_number(1);
		json.new_key("hh");
		json.new_true();
		json.end_obj();
		json.end_obj();		
	}
	catch(JsonException e)
	{
		cout << e.code << endl;
	}
	try
	{
		cout << "json[\"user\"] type is: " << json["user"].flag << endl;
		cout << "json[\"property\"][\"gg\"] value is: " << json["property"]["gg"].number << endl;
		cout << "json pretty text is:\n" << json.get_pretty_json() << endl;
	}
	catch(JsonException e)
	{
		cout << e.code << endl;
	}

	return 0;
}