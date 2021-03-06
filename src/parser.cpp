#include <cstring>
#include "parser.h"
#include "utility.h"

using namespace std;

Parser::Parser() : document(NULL), doc_length(0), doc_position(0), line_number(1), line_position(0) {}

Parser::~Parser()
{
	for(size_t i=0; i<lexeme_stream.size(); i++)
	{
		if(lexeme_stream[i].lex_type == T_STRING)
			delete [] lexeme_stream[i].lex_string;
	}
}


void Parser::parse(const char* doc)
{
	//init
	if(document != NULL)
	{
		json.clear();
		for(size_t i=0; i<lexeme_stream.size(); i++)
		{
			if(lexeme_stream[i].lex_type == T_STRING)
				delete [] lexeme_stream[i].lex_string;
		}
		lexeme_stream.clear();
	}
	document = doc;
	doc_length = strlen(document);
	doc_position = 0;
	line_number = 1;
	line_position = 0;
	if(document == NULL)
		return;
	
	lexical_analyse();
	syntactic_analyse();
}

const Json& Parser::get_json()
{
	return json;
}