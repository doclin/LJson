#include <iostream>
#include <cstring>
#include "json.h"

Json::Json() : root(NULL), current(NULL), json_text(NULL)
{
	point_stack.push(root);
}

Json::Json(const Json& j)
{
	if(j.root == NULL)
	{
		root = NULL;
		current = NULL;
		json_text = NULL;
	}
	else
		copy(j.root);
}

void Json::copy(Value* p)
{
	if(current == NULL)
		root = current;//
}

Json::~Json()
{
	if(root != NULL)
	{
		destroy(root);
		delete root;
	}
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

void Json::new_array()
{
	if(current == NULL)
		root = current = new Value(V_ARRAY);
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
		root = current = new Value(V_OBJ);
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
	;
}

