#include "recall.h"
#include <math.h>
#include <stdlib.h>

extern int setvalnull;

//reduce add with check int overflow
extern  
int ReduceAdd(int a, int b, YYLTYPE *bloc) {
    if(b < 0)
		return ReduceSub(a, -b, bloc);
	if(INT_MAX - b < a) {
		PrintError("int overflow! Line %d:c%d to %d:c%d",
                        bloc->first_line, bloc->first_column,
                        bloc->last_line, bloc->last_column); 
        setvalnull = 1;
		return INT_MAX;
	}
	return a + b;
}

//reduce sub with check int overflow
extern
int ReduceSub(int a, int b, YYLTYPE *bloc) {
    if(b < 0)
		return ReduceAdd(a, -b, bloc);
	if(INT_MIN + b > a) {
		PrintError("int overflow! Line %d:c%d to %d:c%d",
                        bloc->first_line, bloc->first_column,
                        bloc->last_line, bloc->last_column);  
        setvalnull = 1;
		return INT_MIN;
	}
	return a - b;
}

//reduce mult with check int overflow
extern
int ReduceMult(int a, int b, YYLTYPE *bloc) {
    int sign = 1;
	if(a == 0 || b == 0) return 0;
	if(a < 0) { a = -a; sign = -sign; }
	if(b < 0) { b = -b; sign = -sign; }
	if(INT_MAX / b < a) {
        PrintError("int overflow! Line %d:c%d to %d:c%d",
                        bloc->first_line, bloc->first_column,
                        bloc->last_line, bloc->last_column); 
        setvalnull = 1; 
		return (sign > 0) ? INT_MAX : INT_MIN;
	}
	return sign * a * b;
}

//reduce div with check int overflow
extern
int ReduceDiv(int a, int b, YYLTYPE *bloc) {
    if (  b == 0  ) {
        PrintError("division by zero! Line %d:c%d to %d:c%d",
                        bloc->first_line, bloc->first_column,
                        bloc->last_line, bloc->last_column);      
    setvalnull = 1; 
    return INT_MAX;
    }
  return a / b;
}

//reduce pow with check int overflow
extern
int ReducePow(int a, int b, YYLTYPE *bloc) {
    int sign = 1;
    int tmpa = a,tmpb = b;
	if( b == 0) return 1;
	if(tmpa < 0) { tmpa = -a; sign = -sign; }
	if(tmpb < 0) { tmpb = -b; sign = -sign; }
	if((int)pow(INT_MAX, (double)1/tmpb) < tmpa) {
        PrintError("int overflow! Line %d:c%d to %d:c%d",
                        bloc->first_line, bloc->first_column,
                        bloc->last_line, bloc->last_column); 
        setvalnull = 1; 
		return (sign > 0) ? INT_MAX : INT_MIN;
	}
	return (int)pow((int)a, (int)b);
}


