#include "recall.h"

#define MAX_NAME_LEN 32
#define MAXVARS 11

//number current variable in block
static int nVars = 0;
//number of blocks
static int N = 0;
//block ptr for variables table
static Variable** vars=NULL;
extern int setvalnull;

static
Variable* findVar(char* varname) {
    int i, j;
    if (varname == NULL)
        return NULL;
    if (N == 1) {
        for (j = 0; j < nVars; j++) {
            if (strcmp(vars[0][j].name, varname) == 0)
                return (*(vars)+j);          
        }
    }
    else if (N == 0)  return NULL;
    
    else
    {
        for (i = 0; i < N; i++)
            for (j = 0; j < MAXVARS; j++) 
            {
                if (i * MAXVARS + j >= nVars + (N - 1) * MAXVARS)
                    return NULL;

                if (strcmp(vars[i][j].name, varname) == 0) 
                    return (*(vars + i) + j);
            }
    }
    return NULL;
} 

static
Variable* addVar(char* varname) {
    //empty name
    if (varname == NULL)
        return NULL;
    //end of block or start 
    if ((nVars == MAXVARS)||(N == 0)) {
        nVars = 0;
        //realloc memory: add prt for next Variable block
        vars = (Variable**)realloc(vars, (N + 1) * sizeof(Variable*));
        //allocate MAXVARS Variables
        vars[N] = (Variable*)malloc(MAXVARS * sizeof(Variable));
        //reduce count of block
        N += 1;
    }

    //insert default values
    vars[N-1][nVars].value = 0;
    vars[N-1][nVars].init = 0;
    int len = strlen(varname) + 1;
    if (len > MAX_NAME_LEN)
        vars[N-1][nVars].name = malloc(strlen(varname) + 1);
    else
        vars[N-1][nVars].name = malloc(MAX_NAME_LEN);

    //check allocate for name 
    if (vars[N-1][nVars].name == NULL) {
        PrintError("internal error creating variable '%s'", varname);
        return NULL;
    }
    
    //copy name of var
    strcpy(vars[N-1][nVars].name, varname);
    //upd count of var
    nVars += 1;
    //return ptr for Variable
    return (*(vars +  (N - 1)) + (nVars - 1));
}

extern
Variable* VarGet(char* varname, YYLTYPE* bloc) {

    Variable* var;
    
    //find Variable
    var = findVar(varname);   
    //if hasn`t Varialbe by "varname" do create new variable
    if (var == NULL)
        var = addVar(varname);
    return var;
}

extern
void VarSetValue(Variable* var, int value) {
    if ( var == NULL )
        return;
    if ( setvalnull == 1)
    {
        var->value = 0;   
        var->init = 0; 
        setvalnull = 0;
        return;
    }
    var->value = value;   
    var->init = 1;
    return;
}

extern
int VarGetValue(char* varname, YYLTYPE* bloc) {

    Variable* var = NULL;

    //find Variable
    var = findVar(varname);
    //if hasn`t Varialbe by "varname" do create new variable
    if (var == NULL) {
        PrintError("reference to unknown variable '%s'", varname);
        var = addVar(varname);
        setvalnull = 1;
        return 0;
    }
    if (var->init == 0){
        PrintError("variable not init %s", varname);
        setvalnull = 1;
        return 0;
    }
    return var->value;
}  


extern
void DumpAllVariables(int errorcount) {
    int i,j;
    int used = 0;
    char formatsymbols[MAX_NAME_LEN-3];
    for (int i =0;i<MAX_NAME_LEN-3;i++)
    {
        formatsymbols[i] = '-';
    }
    printf("\tName%.*s Value----------\n",MAX_NAME_LEN-3,formatsymbols);
    for (i = 0; i < N-1; i++) {
        for (j = 0; j < MAXVARS; j++) {
            if (vars[i][j].init == 1)
            {
            printf("%d\t: %-*.*s: %d\n", i*MAXVARS+j+1,MAX_NAME_LEN,MAX_NAME_LEN,
                vars[i][j].name, vars[i][j].value);
                used++;
            }
            else 
            printf("%d\t: %-*.*s: %s\n", i*MAXVARS+j+1,MAX_NAME_LEN,MAX_NAME_LEN,
                vars[i][j].name, "NULL");
        }
    }
    for (i = N-1; i < N; i++) {
        for (j = 0; j < nVars; j++) {
            if (vars[i][j].init == 1)
            {
            printf("%d\t: %-*.*s: %d\n", i*MAXVARS+j+1,MAX_NAME_LEN,MAX_NAME_LEN,
                vars[i][j].name, vars[i][j].value);
                used++;
            }
            else 
            printf("%d\t: %-*.*s: %s\n", i*MAXVARS+j+1,MAX_NAME_LEN,MAX_NAME_LEN,
                vars[i][j].name, "NULL");
        }
    }
    printf("==== count of variables - %d ====\n", (N-1)*MAXVARS+j );
    printf("==== count of used var - %d ====\n", used);
    printf("==== count of unused var - %d ====\n", (N-1)*MAXVARS+j - used);
    printf("==== count of errors - %d ====\n",errorcount);
    return;
}
