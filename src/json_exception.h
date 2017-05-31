#ifndef _JSON_EXCEPTION_H_
#define _JSON_EXCEPTION_H_

/*
const char* const E_InnerError = "parser crashed";
const char* const E_101_illegalChar = "this character is illegal in this position";
const char* const E_102_unfinishedText = "this line is finished abnormal";
const char* const E_110_illegalEscapeChar = "the escape character is illegal";
const char* const E_120_illegalNumber = "the number's format is illegal";
const char* const E_130_illegalTrue = "the value true's format is illegal";
const char* const E_140_illegalFalse = "the value false's format is illegal";
const char* const E_150_illegalNull = "the value null's format is illegal";
const char* const E_200_unfinishedText = "this text is finished abnormal";
const char* const E_201_illegalWord = "this word is illegal in this position";
const char* const E_203_illegalText = "this text is illegal";
*/
/*
1001		search failed when query an object
1002		illegal query for an object
1011		search failed when query an array
1012		illegal query for an array
1020		illegal query with empty json object
2001		new array failed with bad position
2002		new array failed as bad value position
2011		new object failed with bad position
2012		new object failed as bad value position
2021		new key failed with bad position
2031		new string failed with bad position
2032		new string failed as bad value position
2041		new number failed with bad position
2042		new number failed as bad value position
2051		new true failed with bad position
2052		new true failed as bad value position
2061		new false failed with bad position
2062		new false failed as bad value position
2071		new null failed with bad position
2072		new null failed as bad value position
2081		end array faled
2091		end object failed
*/
struct ParseException
{
	int code;
	int line;
	int position;
	ParseException(int c, int l, int p) : code(c), line(l), position(p) {}
	const char* get_msg()
	{
		switch(code)
		{
			case 1 		: return "parser crashed";
			case 101	: return "this character is illegal in this position";
			case 102	: return "this line is finished abnormal";
			case 110	: return "the escape character is illegal";
			case 120	: return "the number's format is illegal";
			case 130	: return "the value true's format is illegal";
			case 140	: return "the value false's format is illegal";
			case 150	: return "the value null's format is illegal";
			case 200	: return "this text is finished abnormal";
			case 201	: return "this word is illegal in this position";
			case 203	: return "this text is illegal";
			default		: return NULL;
		}
	}
};

struct JsonException
{
	int code;
	JsonException(int c) : code(c) {}
};

#endif