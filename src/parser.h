#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <stack>
#include "json.h"
#include "utility.h"
#include "json_exception.h"

class Parser
{
private:
	//json data
	Json json;
	//lexeme analysis variable
	std::vector<Lexeme> lexeme_stream;
	const char* document;
	int doc_length;
	int doc_position;
	int line_number;
	int line_position;
	//syntactic analysis variable
	std::stack<int> production_stack;
	//lexeme analysis function
	void lexical_analyse();
	void stringDFA();
	void numberDFA();
	void trueDFA();
	void falseDFA();
	void nullDFA();
	double assemble_number(bool negative, int part_int, double part_frac = 0, int part_exp = 0);
	int char_type(const char c);	
	int to_number(const char ch);
	//syntactic analysis function
	void syntactic_analyse();
public:
	Parser();
	//Parser(const Parser& p);
	//Parser& operator=(const Parser& p);
	~Parser();
	void parse(const char* doc);
	const Json& get_json();

};







#endif