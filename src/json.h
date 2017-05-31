#ifndef _JSON_H_
#define _JSON_H_

#include <stack>
#include <cstring>
#include <string>
#include "json_exception.h"

const int V_STRING 	= 0;
const int V_NUMBER 	= 1;
const int V_TRUE	= 2;
const int V_FALSE	= 3;
const int V_NULL	= 4;
const int V_ARRAY 	= 101;
const int V_OBJ 	= 102;

struct Value;

struct Key
{
	char* key;
	Value* value;
	Key() : key(NULL), value(NULL) {}
	Key(char* k) : key(k), value(NULL) {}
};


struct Value
{
	union
	{
		char* string;			// string
		Key* keys;				// object
		Value* values;			// array
		double number;          // number
	};
	int size;
	int capacity;
	int flag;

	Value() {}
	Value(int f) : flag(f)
	{
		if(f == V_OBJ)
			init_obj();
		else if(f == V_ARRAY)
			init_array();
	}
	Value(int f, double n) : flag(f), number(n) {}			// number
	Value(int f, char* s) : flag(f), string(s) {}			// string
	void init_array()
	{
		flag = V_ARRAY;
		size = 0;
		capacity = 5;
		values = new Value[capacity];
	}
	void init_obj()
	{
		flag = V_OBJ;
		size = 0;
		capacity = 5;
		keys = new Key[capacity];
	}
	//
	const Value& operator[](const char* s) const
	{
		if(flag == V_OBJ)
		{
			for(int i=0; i<size; i++)
			{
				if(0 == strcmp(s, keys[i].key))
					return *(keys[i].value);
			}
			throw JsonException(1001);
		}
		else
			throw JsonException(1002);
	}
	const Value& operator[](int i) const
	{
		if(flag == V_ARRAY)
		{
			if(i < size)
				return values[i];
			else
				throw JsonException(1011);
		}
		else
			throw JsonException(1012);
	}
};


class Json
{
private:
	Value* root;
	Value* current;
	std::stack<Value*> point_stack;
	char* json_text;
	void destroy(Value* p);
	void copy(Value* p);
	void joint_string(std::string& tmp, int indent, Value* p);
public:
	Json();
	Json(const Json& j);
	Json& operator=(const Json& j);
	~Json();
	void clear();
	//
	void new_array();
	void new_obj();
	void new_key(const char* s);
	void new_string(const char* s);
	void new_number(double n);
	void new_true();
	void new_false();
	void new_null();
	void end_obj();
	void end_array();
	//
	const Value& operator[](const char* s) const;
	const Value& operator[](int i) const;
	char* get_pretty_json();

};







#endif