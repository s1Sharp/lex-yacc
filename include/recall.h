#ifndef RECALL_H
#define RECALL_H

#include "includes.h"
#include "y.tab.h"

//file recal.c
//control input 
extern int debug;
extern int overflow;

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(char*);

extern void DumpRow(void); 
extern int  GetNextChar(char *b, int maxBuffer);
extern void NewToken(char*);
extern void PrintError(char *s, ...);


//file math.c
//simple math func
extern int ReduceAdd(int, int, YYLTYPE*);
extern int ReduceSub(int, int, YYLTYPE*);
extern int ReduceMult(int, int, YYLTYPE*);
extern int ReduceDiv(int, int, YYLTYPE*);
extern int ReducePow(int, int, YYLTYPE*);


//file var.c
//using dump values
typedef struct Variable {
  char* name;
  int   value;
  int   init; 
} Variable;

extern Variable *VarGet(char*, YYLTYPE*);
extern void VarSetValue(Variable*, int);
extern int VarGetValue(char*, YYLTYPE*);
extern void DumpAllVariables(char* prefix, int errorcount);


#endif
