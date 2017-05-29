#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "utility.h"
#include "parser.h"

using namespace std;



void Parser::lexical_analyse()
{
	while(doc_position < doc_length)
	{
		int current_type = char_type(document[doc_position]);

		if(current_type == C_BLANK)
			;
		else if(document[doc_position] == '{')
			lexeme_stream.push_back(Lexeme(T_O_BRC));
		else if(document[doc_position] == '}')
			lexeme_stream.push_back(Lexeme(T_C_BRC));
		else if(document[doc_position] == '[')
			lexeme_stream.push_back(Lexeme(T_O_BKT));
		else if(document[doc_position] == ']')
			lexeme_stream.push_back(Lexeme(T_C_BKT));
		else if(document[doc_position] == ',')
			lexeme_stream.push_back(Lexeme(T_COMMA));
		else if(document[doc_position] == ':')
			lexeme_stream.push_back(Lexeme(T_COLON));														

		else if(document[doc_position] == '\"')
			stringDFA();
		else if(document[doc_position] == '-' or current_type == C_NUMBER)
			numberDFA();
		else if(document[doc_position] == 't')
			trueDFA();
		else if(document[doc_position] == 'f')
			falseDFA();
		else if(document[doc_position] == 'n')
			nullDFA();
		else
			throw 101;

		doc_position++;
	}
	return;
}


void Parser::stringDFA()
{
	int start_position = doc_position;
	int end_position = -1;
	doc_position++;
	string tmp;
	while(doc_position < doc_length)
	{
		if(document[doc_position] == '\\')
		{
			doc_position++;	
			if(doc_position >= doc_length)
				throw 102;
			if(document[doc_position] == '\"')
				tmp.push_back('\"');
			else if(document[doc_position] == '\\')
				tmp.push_back('\\');
			else if(document[doc_position] == '/')
				tmp.push_back('/');
			else if(document[doc_position] == 'b')
				tmp.push_back('\b');
			else if(document[doc_position] == 'f')
				tmp.push_back('\f');			
			else if(document[doc_position] == 'n')
				tmp.push_back('\n');
			else if(document[doc_position] == 'r')
				tmp.push_back('\r');
			else if(document[doc_position] == 't')
				tmp.push_back('\t');
			else if(document[doc_position] == 'u')
			{
				for(int i=0; i<4; i++)
				{
					doc_position++;
					if(doc_position >= doc_length)
						throw 102;
					if (!((document[doc_position]>='0' and document[doc_position]<='9') 
						or (document[doc_position]>='a' and document[doc_position]<='f')
						or (document[doc_position]>='A' and document[doc_position]<='F')))
					{
						throw 1021;
					}
				}
				//tmp.append(decode(string, doc_position-3, doc_position)); //-------------
			}
		}
		else if(document[doc_position] == '\"')
		{
			end_position = doc_position;
			break;
		}
		else
			tmp.push_back(document[doc_position]);
		doc_position++;
	}
	if(end_position == -1)
		throw 102;

	int string_length = tmp.size();
	char* string_val = new char[string_length + 1];
	tmp.copy(string_val, string_length);
	string_val[string_length] = '\0';
	lexeme_stream.push_back(Lexeme(T_STRING, string_val));
}



void Parser::numberDFA()
{
	bool negative = false;
	int part_int = 0;
	double part_frac = 0.0;
	int part_exp = 0;

	if(document[doc_position] == '-')
	{
		doc_position++;
		if(doc_position >= doc_length)
			throw 402;
		negative = true;
	}

	if(document[doc_position] == '0')
	{
		doc_position++;
		if(doc_position >= doc_length)
		{
			lexeme_stream.push_back(Lexeme(T_NUMBER, 0.0));
			return;
		}
	}
	else if(char_type(document[doc_position]) == C_NUMBER)
	{
		part_int = 10 * part_int + to_number(document[doc_position]);
		while(true)
		{
		    doc_position++;
		    if(doc_position >= doc_length)
		    {
			    lexeme_stream.push_back(Lexeme(T_NUMBER, assemble_number(negative, part_int)));
			    return;
		    }
		    if(char_type(document[doc_position]) == C_NUMBER)
		    	part_int = 10 * part_int + to_number(document[doc_position]);
		    else
		    	break;
		}
	}
	else
		throw 401;

	if(document[doc_position] == '.')
	{
		double digits = 0.1;
		doc_position++;
		if(doc_position >= doc_length)
			throw 402;
		if(char_type(document[doc_position]) == C_NUMBER)
		{
			part_frac = part_frac + digits * to_number(document[doc_position]);
			digits *= 0.1;
		}
		else
			throw 401;

		while(true)
		{
			doc_position++;
			if(doc_position >= doc_length)
			{
				lexeme_stream.push_back(Lexeme(T_NUMBER, assemble_number(negative, part_int, part_frac)));
				return;
			}
			if(char_type(document[doc_position]) == C_NUMBER)
			{
				part_frac = part_frac + digits * to_number(document[doc_position]);
				digits *= 0.1;
			}
			else
				break;
		}
	}

	if(document[doc_position] == 'e' or document[doc_position] == 'E')
	{
		bool exp_negative = false;
		doc_position++;
		if(doc_position >= doc_length)
			throw 402;

		if(document[doc_position] == '+')
			doc_position++;
		else if(document[doc_position] == '-')
		{
			exp_negative = true;
			doc_position++;
		}
		if(doc_position >= doc_length)
			throw 402;

		if(char_type(document[doc_position]) == C_NUMBER)
			part_exp = 10 * part_exp + to_number(document[doc_position]);
		else
			throw 401;

		while(true)
		{
			doc_position++;
			if(doc_position >= doc_length)
			{
				if(exp_negative)
					part_exp *= -1;
				lexeme_stream.push_back(Lexeme(T_NUMBER, assemble_number(negative, part_int, part_frac, part_exp)));
			}
			if(char_type(document[doc_position]) == C_NUMBER)
				part_exp = 10 * part_exp + to_number(document[doc_position]);
			else
				break;
		}

		if(exp_negative)
			part_exp *= -1;
	}

	lexeme_stream.push_back(Lexeme(T_NUMBER, assemble_number(negative, part_int, part_frac, part_exp)));
	doc_position--;
}



void Parser::trueDFA()
{
	const char* s_true = "true";
	for(int i=1; i<4; i++)
	{
		doc_position++;
		if(doc_position >= doc_length)
			throw 302;		
		if (document[doc_position] != s_true[i])
			throw 301;
	}
	lexeme_stream.push_back(Lexeme(T_TRUE));
}

void Parser::falseDFA()
{
	const char* s_false = "false";
	for(int i=1; i<5; i++)
	{
		doc_position++;
		if(doc_position >= doc_length)
			throw 302;		
		if (document[doc_position] != s_false[i])
			throw 301;
	}
	lexeme_stream.push_back(Lexeme(T_FALSE));
}

void Parser::nullDFA()
{
	const char* s_null = "null";
	for(int i=1; i<4; i++)
	{
		doc_position++;
		if(doc_position >= doc_length)
			throw 302;		
		if (document[doc_position] != s_null[i])
			throw 301;
	}
	lexeme_stream.push_back(Lexeme(T_NULL));
}

int Parser::to_number(const char ch)
{
	switch(ch)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
	}
	return -1;
}

int Parser::char_type(const char c)
{
	if(c>='a' and c<='z')
		return C_LOWERCASE;
	if(c>='A' and c<='Z')
		return C_CAPITAL;
	if(c>='0' and c<='9')
		return C_NUMBER;
	if(c==' ' or c=='\t' or c=='\r' or c=='\n')
		return C_BLANK;
	if(c=='{' or c=='}' or c=='[' or c==']' or c==',' or c==':')
		return C_CHARACTER;

	return C_OTHER;
}

double Parser::assemble_number(bool negative, int part_int, double part_frac, int part_exp)
{
	double result = (part_int + part_frac) * pow(10, part_exp);
	if(negative)
		return -1 * result;
	return result;
}