#include <iostream>
#include <stack>
#include "utility.h"
#include "parser.h"

using namespace std;


void Parser::syntactic_analyse()
{
	production_stack.push(N_S);
	
	int stream_length = lexeme_stream.size();
	if(stream_length == 0)
		return;

	int i = 0;
	while(true)
	{
		if(i >= stream_length)
			break;
		if(production_stack.empty())
			throw ParseException(203, lexeme_stream[i].line, lexeme_stream[i].position);

		int word = production_stack.top();
		if(word >= TERMINAL)
			word -= 100;
		int next_word = lexeme_stream[i].lex_type;
		int production = TABLE[word][next_word];

		if(production == -1)
			throw ParseException(201, lexeme_stream[i].line, lexeme_stream[i].position);
		else if(production < NON_TERMINAL)
		{
			production_stack.pop();
			production_stack.push(production);
		}
		else if(production == E_CBKT or production == E_CBRC)
			production_stack.pop();
		else if(production == 10105)
		{
			production_stack.pop();
			production_stack.push(T_C_BKT);
			production_stack.push(N_VALS);
			production_stack.push(T_O_BKT);
		}
		else if(production == 10206)
		{
			production_stack.pop();
			production_stack.push(T_C_BRC);
			production_stack.push(N_MEMS);
			production_stack.push(T_O_BRC);
		}
		else if(production == 10499)
		{
			production_stack.pop();
			production_stack.push(N_AVALS);
			production_stack.push(N_VAL);
		}
		else if(production == 10507)
		{
			production_stack.pop();
			production_stack.push(N_NEVALS);
			production_stack.push(T_COMMA);
		}
		else if(production == 10700)
		{
			production_stack.pop();
			production_stack.push(N_AMEMS);
			production_stack.push(N_MEM);
		}
		else if(production == 10807)
		{
			production_stack.pop();
			production_stack.push(N_NEMEMS);
			production_stack.push(T_COMMA);
		}
		else if(production == 10900)
		{
			production_stack.pop();
			production_stack.push(N_VAL);
			production_stack.push(T_COLON);
			production_stack.push(T_STRING);
		}
		else
			throw ParseException(1, lexeme_stream[i].line, lexeme_stream[i].position);
		
		if(production == N_ARRAY)
			json.new_array();
		else if(production == N_OBJ)
			json.new_obj();
		else if(production == 10900)
			json.new_key(lexeme_stream[i].lex_string);
		else if(production == 0)
			json.new_string(lexeme_stream[i].lex_string);
		else if(production == 1)
			json.new_number(lexeme_stream[i].lex_number);
		else if(production == 2)
			json.new_true();
		else if(production == 3)
			json.new_false();
		else if(production == 4)
			json.new_null();
		else if(production == E_CBRC)
			json.end_obj();
		else if(production == E_CBKT)
			json.end_array();
		


		while(true)
		{
			if(i >= stream_length)
				break;
			if(production_stack.top() < TERMINAL)
			{
				if(production_stack.top() == lexeme_stream[i].lex_type)
				{
					production_stack.pop();
					i++;
				}
				else
					throw ParseException(201, lexeme_stream[i].line, lexeme_stream[i].position);
			}
			else
				break;
		}
	}

	if(!production_stack.empty())
		throw ParseException(200, lexeme_stream[i].line, lexeme_stream[i].position);
}