#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include "json.h"

using namespace std;

Json::Json() : root(NULL), current(NULL), json_text(NULL) {}

Json::Json(const Json& j)
{
	if(j.root == NULL)
	{
		root = NULL;
		current = NULL;
		json_text = NULL;
	}
	else
	{
		root = current = NULL;
		json_text = NULL;
		copy(j.root);
	}
}

Json& Json::operator=(const Json& j)
{
	if(root != NULL)
	{
		destroy(root);
		delete root;
		if(json_text != NULL)
			delete [] json_text;
	}

	root = current = NULL;
	json_text = NULL;
	if(j.root != NULL)
		copy(j.root);
}

void Json::copy(Value* p)
{
	if(p->flag == V_OBJ)
	{
		new_obj();
		for(int i=0; i<p->size; i++)
		{
			new_key(p->keys[i].key);
			copy(p->keys[i].value);
		}
		end_obj();
	}
	else if(p->flag == V_ARRAY)
	{
		new_array();
		for(int i=0; i<p->size; i++)
			copy(&(p->values[i]));
		end_array();
	}
	else if(p->flag == V_STRING)
		new_string(p->string);
	else if(p->flag == V_NUMBER)
		new_number(p->number);
	else if(p->flag == V_TRUE)
		new_true();
	else if(p->flag == V_FALSE)
		new_false();
	else if(p->flag == V_NULL)
		new_null();
}

Json::~Json()
{
	if(root != NULL)
	{
		destroy(root);
		delete root;
	}
	if(json_text != NULL)
		delete [] json_text;
}

void Json::destroy(Value* p)
{
	if(p->flag == V_OBJ)
	{
		for(int i=0; i<p->size; i++)
		{
			destroy(p->keys[i].value);
			delete p->keys[i].value;
			delete [] p->keys[i].key;
		}
		delete [] p->keys;
	}
	else if(p->flag == V_ARRAY)
	{
		for(int i=0; i<p->size; i++)
			destroy(&p->values[i]);
		delete [] p->values;
	}
	else if(p->flag == V_STRING)
		delete [] p->string;
	else
		return;
}

void Json::clear()
{
	if(root != NULL)
		destroy(root);
	if(json_text != NULL)
		delete [] json_text;
	root = current = NULL;
	json_text = NULL;
}

void Json::new_array()
{
	if(current == NULL)
	{
		root = current = new Value(V_ARRAY);
		point_stack.push(current);
	}
	else if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].init_array();
		current->size++;

		point_stack.push(current);
		current = &current->values[current->size - 1];
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_ARRAY);
			current->size++;

			point_stack.push(current);
			current = current_key.value;
		}
		else
			throw 2002;
	}
	else
		throw 2001;
}

void Json::new_obj()
{
	if(current == NULL)
	{
		root = current = new Value(V_OBJ);
		point_stack.push(current);
	}
	else if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].init_obj();
		current->size++;

		point_stack.push(current);
		current = &current->values[current->size - 1];
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_OBJ);
			current->size++;

			point_stack.push(current);
			current = current_key.value;
		}
		else
			throw 2002;
	}
	else
		throw 2001;
}

void Json::new_key(char* s)
{
	if(current->flag == V_OBJ)
	{
		if(current->size >= current->capacity)
		{
			Key* tmp = new Key[current->capacity * 2];
			memcpy(tmp, current->keys, current->capacity * sizeof(Key));
			current->capacity *= 2;
			delete [] current->keys;
			current->keys = tmp;
		}
		char* new_s;
		if(s != NULL)
		{
			int len = strlen(s);
			new_s = new char[len + 1];
			memcpy(new_s, s, len+1);			
		}
		else
		{
			new_s = new char[1];
			new_s[0] = '\0';
		}

		current->keys[current->size].key = new_s;
	}
	else
		throw 2001;
}

void Json::new_string(char* s)
{
	if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}

		char* new_s;
		if(s != NULL)
		{
			int len = strlen(s);
			new_s = new char[len + 1];
			memcpy(new_s, s, len+1);			
		}
		else
		{
			new_s = new char[1];
			new_s[0] = '\0';
		}

		current->values[current->size].flag = V_STRING;
		current->values[current->size].string = new_s;
		current->size++;
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			char* new_s;
			if(s != NULL)
			{
				int len = strlen(s);
				new_s = new char[len + 1];
				memcpy(new_s, s, len+1);			
			}
			else
			{
				new_s = new char[1];
				new_s[0] = '\0';
			}

			current_key.value = new Value(V_STRING, new_s);
			current->size++;
		}
		else
			throw 2002;
	}
	else
		throw 2001;	
}

void Json::new_number(double n)
{
	if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].flag = V_NUMBER;
		current->values[current->size].number = n;
		current->size++;
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_NUMBER, n);
			current->size++;
		}
		else
			throw 2002;
	}
	else
		throw 2001;	
}

void Json::new_true()
{
	if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].flag = V_TRUE;
		current->size++;
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_TRUE);
			current->size++;
		}
		else
			throw 2002;
	}
	else
		throw 2001;	
}

void Json::new_false()
{
	if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].flag = V_FALSE;
		current->size++;
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_FALSE);
			current->size++;
		}
		else
			throw 2002;
	}
	else
		throw 2001;	
}

void Json::new_null()
{
	if(current->flag == V_ARRAY)
	{
		if(current->size >= current->capacity)
		{
			Value* tmp = new Value[current->capacity * 2];
			memcpy(tmp, current->values, current->capacity * sizeof(Value));
			current->capacity *= 2;
			delete [] current->values;
			current->values = tmp;
		}
		current->values[current->size].flag = V_NULL;
		current->size++;
	}
	else if(current->flag == V_OBJ)
	{
		Key& current_key = current->keys[current->size];
		if(current->size < current->capacity and current_key.key != NULL and current_key.value == NULL)
		{
			current_key.value = new Value(V_NULL);
			current->size++;
		}
		else
			throw 2002;
	}
	else
		throw 2001;	
}

void Json::end_array()
{
	if(point_stack.empty())
		throw 2003;
	current = point_stack.top();
	point_stack.pop();
}

void Json::end_obj()
{
	if(point_stack.empty())
		throw 2003;
	current = point_stack.top();
	point_stack.pop();
}

const Value& Json::operator[](const char* s) const
{
	if(root == NULL)
		throw 4001;
	return (*root)[s];
}

const Value& Json::operator[](int i) const
{
	if(root == NULL)
		throw 4001;
	return (*root)[i];
}

char* Json::get_pretty_json()
{
	if(root == NULL)
		return json_text;
	if(json_text != NULL)
		delete [] json_text;
	string tmp;
	joint_string(tmp, 0, root);
	int string_length = tmp.size();
	json_text = new char[string_length + 1];
	tmp.copy(json_text, string_length);
	json_text[string_length] = '\0';

	return json_text;
}

void Json::joint_string(string& tmp, int indent, Value* p)
{
	if(p->flag == V_OBJ)
	{
		tmp.append("{\n");
		indent++;
		for(int i=0; i<p->size; i++)
		{
			tmp.append(indent, '\t');

			tmp.append("\"");
			tmp.append(p->keys[i].key);
			tmp.append("\": ");

			joint_string(tmp, indent, p->keys[i].value);
			if(i != p->size - 1)
				tmp.append(",");
			tmp.append("\n");
		}
		tmp.append(indent-1, '\t');
		tmp.append("}");
	}
	else if(p->flag == V_ARRAY)
	{
		tmp.append("[\n");
		indent++;
		for(int i=0; i<p->size; i++)
		{
			tmp.append(indent, '\t');
			joint_string(tmp, indent, &(p->values[i]));
			if(i != p->size - 1)
				tmp.append(",");
			tmp.append("\n");
		}
		tmp.append(indent-1, '\t');
		tmp.append("]");
	}
	else if(p->flag == V_STRING)
	{
		tmp.append("\"");
		tmp.append(p->string);
		tmp.append("\"");
	}
	else if(p->flag == V_NUMBER)
	{
		ostringstream d_strings;
		d_strings << p->number;
		tmp.append(d_strings.str());
	}
	else if(p->flag == V_TRUE)
		tmp.append("true");
	else if(p->flag == V_FALSE)
		tmp.append("false");
	else if(p->flag == V_NULL)
		tmp.append("null");
}

