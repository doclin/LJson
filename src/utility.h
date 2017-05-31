#ifndef _UTILITY_H_
#define _UTILITY_H_

//--------Char-type--------
const int C_LOWERCASE 	= 0;
const int C_CAPITAL 	= 1;
const int C_NUMBER 		= 2;
const int C_BLANK		= 3;
const int C_CHARACTER 	= 4;
const int C_OTHER 		=-1;

//LL(1) Table:
const int T_STRING 		= 0;
const int T_NUMBER 		= 1;
const int T_TRUE 		= 2;
const int T_FALSE 		= 3;
const int T_NULL 		= 4;
const int T_O_BKT		= 5;	// [
const int T_O_BRC		= 6;	// {
const int T_COMMA		= 7;	// ,
const int T_COLON		= 8;	// :
const int T_C_BKT		= 9;	// ]
const int T_C_BRC		= 10;	// }
//---------Non-Terminal---------
const int N_S			= 100;
const int N_ARRAY		= 101;
const int N_OBJ			= 102;
const int N_VALS		= 103;
const int N_NEVALS		= 104;
const int N_AVALS		= 105;
const int N_MEMS		= 106;
const int N_NEMEMS		= 107;
const int N_AMEMS		= 108;
const int N_MEM			= 109;
const int N_VAL			= 110;
//---------Production---------

const int E_CBKT		= 19999;	// e]
const int E_CBRC		= 19990;	// e}
/*
const int P10005		= 101;
const int P10006		= 102;
const int P10105		= 10105;
const int P10206		= 10206;
const int P10399		= 104;
const int P10499		= 10499;
const int P10507		= 10507;
const int P10600		= 107;
const int P10700		= 10700;
const int P10807		= 10807;
const int P10900		= 10900;
const int P11005		= 101;
const int P11006		= 102;
const int P11000		= 0;
const int P11001		= 1;
const int P11002		= 2;
const int P11003		= 3;
const int P11004		= 4;
*/
//---------category---------
const int TERMINAL = 99;
const int NON_TERMINAL = 999;
//---------Table---------
const int TABLE[11][11] = {
	{-1		, -1	, -1	, -1	, -1	, 101		, 102	, -1	, -1	, -1	, -1	},
	{-1		, -1	, -1	, -1	, -1	, 10105		, -1	, -1	, -1	, -1	, -1	},
	{-1		, -1	, -1	, -1	, -1	, -1		, 10206	, -1	, -1	, -1	, -1	},
	{104	, 104	, 104	, 104	, 104	, 104		, 104	, -1	, -1	, 19999	, -1	},
	{10499	, 10499	, 10499	, 10499	, 10499	, 10499		, 10499	, -1	, -1	, -1	, -1	},
	{-1		, -1	, -1	, -1	, -1	, -1		, -1	, 10507	, -1	, 19999	, -1	},
	{107	, -1	, -1	, -1	, -1	, -1		, -1	, -1	, -1	, -1	, 19990	},
	{10700	, -1	, -1	, -1	, -1	, -1		, -1	, -1	, -1	, -1	, -1	},
	{-1		, -1	, -1	, -1	, -1	, -1		, -1	, 10807	, -1	, -1	, 19990	},
	{10900	, -1	, -1	, -1	, -1	, -1		, -1	, -1	, -1	, -1	, -1	},
	{0		,  1	,  2	,  3	,  4	, 101		, 102	, -1	, -1	, -1	, -1	},
};


struct Lexeme
{
	int lex_type;
	union
	{
		double lex_number;
		char* lex_string;		
	};
	int line;
	int position;
	Lexeme(int t, double n, int l, int p) : lex_type(t), lex_number(n), line(l), position(p) {}
	Lexeme(int t, char* s, int l, int p) : lex_type(t), lex_string(s), line(l), position(p) {}
	Lexeme(int t, int l, int p) : lex_type(t), line(l), position(p) {}
};

#endif