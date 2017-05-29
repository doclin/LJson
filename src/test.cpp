#include <iostream>
#include <cstring>

using namespace std;
/*
struct Key
{
	char* key;
	Value* value;
	Key(char* k) : key(k), value(NULL) {}
};


struct Value
{
	union
	{
		char* string;			// string
		Key* keys;				// object
		Value* values;			// array
		double number;              // number
	};

	int size;
	int capacity;

	int flag;
	
	Value() {}
	Value(int f) : flag(f)
	{
		if(f == V_OBJ)
		{
			size = 0;
			capacity = 5;
			keys = new Key[capacity];
		}
		else if(f == V_ARRAY)
		{
			size = 0;
			capacity = 5;
			values = new Value[capacity];
		}
	}
	Value(int f, double n) : flag(f), number(n) {}			// number
	Value(int f, char* s) : flag(f), string(s) {}		// string
};


void new_array()
{
	Value* current = new Value(V_ARRAY);
	current->values[0].flag = -1;
	current->values[1].flag = -2;
	current->values[2].flag = -3;
	current->values[3].flag = -4;
	current->values[4].flag = -5;
	current->size = 5;


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
		current->values[current->size].flag = -6;
		current->size++;
	}
	cout << current->size << endl;
}*/

int main(int argc, char const *argv[])
{
	int* newint = new int[10];
	int a[3] = {1,2,3};
	memcpy(newint, a, 3*sizeof(int));
	cout << newint[2] << newint[3] << newint[4] << endl;
}