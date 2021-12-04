#ifndef RECALL_H
#define RECALL_H

#include "includes.h"
#include "y.tab.h"

//=============file recall.c===============
//control output 
extern int debug;
//control operator '=' 
extern int overflow;

//default yacc func
extern int yylex(void);
extern int yyparse(void);
extern void yyerror(char*);

//control input
extern void DumpRow(void); 
extern int  GetNextChar(char *b, int maxBuffer);
extern void NewToken(char*);
extern void PrintError(char *s, ...);


//===============file math.c================
//simple math func
//this math function wokring only with 'int' values
extern int ReduceAdd(int, int, YYLTYPE*);
extern int ReduceSub(int, int, YYLTYPE*);
extern int ReduceMult(int, int, YYLTYPE*);
extern int ReduceDiv(int, int, YYLTYPE*);
//note func pow return 0, if the base of the degree is negative.
//this is related to integers
extern int ReducePow(int, int, YYLTYPE*);



//===============file var.c=================
//using dump values
typedef struct Variable {
  char* name;
  int   value;
  int   init; 
} Variable;

//use for work with variables stash
extern Variable *VarGet(char*, YYLTYPE*);
extern void      VarSetValue(Variable*, int);
extern int       VarGetValue(char*, YYLTYPE*);
extern void      DumpAllVariables(int errorcount);

#endif
