%{
#include "recall.h"

static Variable *var;
void HandleError(char*s);
int setvalnull = 0;
int count = 0;

%}

%union {
  int   value;
  char  *string;
}

%token <string>   IDENTIFIER
%token <value>    NUMBER
%type <value>     expr
%token yIN yFOR yALL yNEXT yREST yRECORD yWHILE yRECALL yNOOPTIMIZE SYMLP SYMRP yAND yNOT yOR ySEMICOLON SIGNEQ 
/*      in  for  all  next  rest  record  while  recall  nooptimize   (     )    and  not  or       ;       =  */
%left SIGNEQQ SIGNNEQ
/*      ==     != <>     */ 
%left SIGNLESS SIGNMORE SIGNLEQ SIGNMEQ
/*        <        >       <=      >=     */
%left SIGNPLUS SIGNMINUS
/*       +         -      */
%left SIGNMULT SIGNDIV
/*        *       /      */
%start program

%%
program: 
         stat ySEMICOLON {count = 0; } |
         program stat ySEMICOLON {count = 0; };

stat    : error {HandleError("wrong syntax");}
        | yRECALL scope forexpr whileexpr nooptim inn 
        | IDENTIFIER { var = VarGet($1, &@1);} SIGNEQ expr { if ($4 == -1) setvalnull=1; VarSetValue(var, $4);} 
        | IDENTIFIER error{HandleError("wrong identifier");}
        ;

scope   :              
        | yALL         
        | yREST       
        | yNEXT expr   
        | yRECORD expr
        ;

forexpr :                
        |  yFOR lexpr
        ;
  
whileexpr   :               
            | yWHILE lexpr
            ;

inn :
    | yIN expr
    ;

nooptim :
        | yNOOPTIMIZE
        ;     

lexpr   : SYMLP lexpr SYMRP
        | expr SIGNEQQ expr
        | expr SIGNNEQ expr
        | expr SIGNLESS expr
        | expr SIGNMORE expr
        | expr SIGNLEQ expr
        | expr SIGNMEQ expr
        | expr yAND expr
        | expr yOR expr
        | expr yNOT expr ;
     
expr    : SYMLP expr SYMRP      {   $$=  $2;  }
        | SIGNMINUS expr        {   $$= -$2;  }
        | expr SIGNPLUS expr    {   $$=ReduceAdd($1, $3, &@3);  }  
        | expr SIGNMINUS expr   {   $$=ReduceSub($1, $3, &@3);  }  
        | expr SIGNMULT expr    {   $$=ReduceMult($1, $3, &@3); }  
        | expr SIGNDIV  expr    {   $$=ReduceDiv($1, $3, &@3);  }  
        | NUMBER                {   $$= $1;  }     
        | NUMBER error          {   HandleError("wrong number"); $$=-1; }
        | error                 {   HandleError("wrong arifmetic expression"); $$=-1;  }
        | IDENTIFIER error      {   HandleError("wrong identifier"); $$=-1; }
        | IDENTIFIER            {   $$ = VarGetValue($1, &@1); }
        ;        
%%

void HandleError(char*s)
{
if(count==0)
{PrintError(s);}
count++;
}

/*
static int
yyreport_syntax_error (const yypcontext_t *ctx)
{
  int res = 0;
  return res;
}
*/

extern 
void yyerror(char *s)
{
    PrintError(s);
}
