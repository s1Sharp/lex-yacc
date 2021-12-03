%option noyywrap 

%{
#include "recall.h"

#define IDENTIFIER_MAX_LEN 32

#define YY_INPUT(buf,result,max_size)  {\
    result = GetNextChar(buf, max_size); \
    if (  result <= 0  ) \
      result = YY_NULL; \
    }

%}


Identifier  [_a-zA-Z][_a-zA-Z0-9]*
Number      0|([1-9][0-9]*)
recall      [Rr][Ee][Cc][Aa][Ll][Ll]
next        [Nn][Ee][Xx][Tt]
rest        [Rr][Ee][Ss][Tt]
nooptimize  [Nn][Oo][Oo][Pp][Tt][Ii][Mm][Ii][Zz][Ee]
record      [Rr][Ee][Cc][Oo][Rr][Dd]
for         [Ff][Oo][Rr]
while       [Ww][Hh][Ii][Ll][Ee]
in          [Ii][Nn]
all         [Aa][Ll][Ll]
and         [Aa][Nn][Dd]
or          [Oo][Rr]
not         [Nn][Oo][Tt]


%%
\xD;
{recall}            { NewToken(yytext);  return(yRECALL);}
{next}              { NewToken(yytext);  return(yNEXT);}
{rest}              { NewToken(yytext);  return(yREST);}
{nooptimize}        { NewToken(yytext);  return(yNOOPTIMIZE);}
{for}               { NewToken(yytext);  return(yFOR);}
{record}            { NewToken(yytext);  return(yRECORD);}
{while}             { NewToken(yytext);  return(yWHILE);}
{in}                { NewToken(yytext);  return(yIN);}
{all}               { NewToken(yytext);  return(yALL);}
{and}               { NewToken(yytext);  return(yAND);}
{or}                { NewToken(yytext);  return(yOR);}
{not}               { NewToken(yytext);  return(yNOT);}
\/              { NewToken(yytext);  return(SIGNDIV);}
\+              { NewToken(yytext);  return(SIGNPLUS);}
\-              { NewToken(yytext);  return(SIGNMINUS);}
\*              { NewToken(yytext);  return(SIGNMULT);}
\^              { NewToken(yytext);  return(SIGNPOW);}
\<              { NewToken(yytext);  return(SIGNLESS);}
\>              { NewToken(yytext);  return(SIGNMORE);}
\=              { NewToken(yytext);  return(SIGNEQ);}
\(              { NewToken(yytext);  return(SYMLP);}
\)              { NewToken(yytext);  return(SYMRP);}
"<="            { NewToken(yytext);  return(SIGNLEQ);}
">="            { NewToken(yytext);  return(SIGNMEQ);}
"=="            { NewToken(yytext);  return(SIGNEQQ);}
"!="|"<>"       { NewToken(yytext);  return(SIGNNEQ);}

{Number}      {
                 NewToken(yytext); 
                 yylval.value = atoi(yytext);
                 return(NUMBER);
              }
{Identifier}  {
                if	(yyleng > IDENTIFIER_MAX_LEN) 
					PrintError("the name of variable is too long");
                else{
                	yylval.string = malloc(strlen(yytext)+1);
                	strcpy(yylval.string, yytext);NewToken(yytext); 
                	return (IDENTIFIER);
                }
              }

[ \r\t\n]+  { NewToken(yytext);};
\;        { NewToken(yytext); return(ySEMICOLON);}
.         { NewToken(yytext); return(yytext[0]);};

%%
