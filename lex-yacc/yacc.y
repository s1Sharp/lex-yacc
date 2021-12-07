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
    /*  in  for  all  next  rest  record  while  recall  nooptimize   (     )    and  not  or       ;       =  */

%left yOR
%left yAND
%left yNOT

%left SIGNEQQ SIGNNEQ
    /*  ==     != <>                                    */ 
%left SIGNLESS SIGNMORE SIGNLEQ SIGNMEQ
    /*   <        >       <=      >=                    */
%left SIGNPLUS SIGNMINUS
    /*   +         -                                    */
%left SIGNMULT SIGNDIV
    /*   *       /                                      */
%left SIGNPOW
    /*   ^                                              */

%start program

%%
program : stat ySEMICOLON {count = 0; setvalnull=0; } 
        | program stat ySEMICOLON {count = 0; setvalnull=0; };

stat    : error {HandleError("wrong syntax");}
        | yRECALL scope forexpr whileexpr nooptim inn
        | IDENTIFIER { var = VarGet($1, &@1);} SIGNEQ expr { VarSetValue(var, $4);} 
        | IDENTIFIER error{HandleError("wrong identifier");}
        ;

scope   :              
        | yALL         
        | yREST       
        | yNEXT expr   
        | yRECORD expr
        ;

forexpr :                
        |  yFOR expr
        ;
  
whileexpr   :               
            | yWHILE expr
            ;

inn :
    | yIN expr
    ;

nooptim :
        | yNOOPTIMIZE
        ;

     
expr    : SYMLP expr SYMRP      {   $$=  $2;  }
        | SYMLP expr error      {   HandleError("wrong expression, forget '(' or ')'"); setvalnull=1;  }
        | error expr SYMLP      {   HandleError("wrong expression, forget '(' or ')'"); setvalnull=1;  }
        | SIGNMINUS expr        {   $$= -$2;  }
        | expr SIGNPLUS expr    {   $$=ReduceAdd($1, $3, &@3);  }  
        | expr SIGNMINUS expr   {   $$=ReduceSub($1, $3, &@3);  }  
        | expr SIGNMULT expr    {   $$=ReduceMult($1, $3, &@3); }  
        | expr SIGNDIV expr     {   $$=ReduceDiv($1, $3, &@3);  }  
        | expr SIGNPOW expr     {   $$=ReducePow($1, $3, &@3);  }
        | NUMBER                {   $$= $1;  }     
        | NUMBER error          {   HandleError("wrong number"); setvalnull=1; }
        | error                 {   HandleError("wrong arifmetic expression"); setvalnull=1;  }
        | IDENTIFIER error      {   HandleError("wrong identifier"); setvalnull=1; }
        | IDENTIFIER            {   $$ = VarGetValue($1, &@1); }
        | expr SIGNEQQ expr     {   $$=$1==$3; }
        | expr SIGNNEQ expr     {   $$=$1!=$3; }
        | expr SIGNLESS expr    {   $$=$1<$3 ; }
        | expr SIGNMORE expr    {   $$=$1>$3 ; }
        | expr SIGNLEQ expr     {   $$=$1<=$3; }
        | expr SIGNMEQ expr     {   $$=$1>=$3; }
        | expr yAND expr        {   $$=$1&&$3; }
        | expr yOR expr         {   $$=$1||$3; }
        | yNOT expr             {   $$=!$2   ; }
        ;        
%%

void HandleError(char*s)
{
if(count==0)
{PrintError(s);}
count++;
}


extern 
void yyerror(char *s)
{
    PrintError(s);
}
