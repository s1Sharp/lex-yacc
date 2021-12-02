#include "recall.h"

#define true 1
#define false 0

int debug=0;

//init static variables for control tokens and variables 
static FILE *file;
static int eof = 0;
static int nRow = 0;
static int nBuffer = 0;
static int lBuffer = 0;
static int nTokenStart = 0;
static int nTokenLength = 0;
static int nTokenNextStart = 0;
static int lMaxBuffer = 1000;
static int errorcount = 0;
static char *buffer;

static
char dumpChar(char c) {
	if (  isprint(c)  )
		return c;
	return '@';
}

static
char *dumpString(char *s) {
	static char buf[101];
	int i;
	int n = strlen(s);

	if (  n > 100  )
		n = 100;

	for (i=0; i<n; i++)
		buf[i] = dumpChar(s[i]);
	buf[i] = 0;
	return buf;
}

extern
void DumpRow(void) {
	if (  nRow == 0  ) {
		int i;
		fprintf(stdout, "-------|");
		for (i=1; i<71; i++)
			fprintf(stdout, ".");
		fprintf(stdout, "\n"); 
	}
	else 
		fprintf(stdout, "%6d |%.*s", nRow, lBuffer, buffer);
}

extern
void PrintError(char *errorstring, ...) {
	errorcount ++;
	static char errmsg[10000];
	va_list args;

	int start=nTokenStart;
	int end=start + nTokenLength - 1;
	int i;

	if (  eof  ) {
		fprintf(stdout, "...... !");
		for (i=0; i<lBuffer; i++)
			fprintf(stdout, ".");
		fprintf(stdout, "^-EOF\n");
	}
	else {
		fprintf(stdout, "...... !");
		for (i=1; i<start; i++)
			fprintf(stdout, ".");
		for (i=start; i<=end; i++)
			fprintf(stdout, "^");
		for (i=end+1; i<lBuffer; i++)
			fprintf(stdout, ".");
		fprintf(stdout, "   position: %d\n", end);
	}
	va_start(args, errorstring);
	vsprintf(errmsg, errorstring, args);
	va_end(args);

	fprintf(stdout, "Error: %s\n", errmsg);
}

static
int getNextLine(void) {
	int i;
	char *p;
	
	nBuffer = 0;
	nTokenStart = -1;
	nTokenNextStart = 1;
	eof = false;

	p = fgets(buffer, lMaxBuffer, file);
	if (  p == NULL  ) {
		if (  ferror(file)  )
			return -1;
		eof = true;
		return 1;
	}

	nRow += 1;
	lBuffer = strlen(buffer);
	DumpRow();
	return 0;
}

extern
int GetNextChar(char *b, int maxBuffer) {
	int frc;

	if (  eof  )
		return 0;
 
	while (  nBuffer >= lBuffer  ) {
		frc = getNextLine();
		if (  frc != 0  )
			return 0;
		}

	b[0] = buffer[nBuffer];
	nBuffer += 1;

	if (  debug  )
		printf("GetNextChar() => '%c'0x%02x at %d\n",
												dumpChar(b[0]), b[0], nBuffer);
	return b[0]==0?0:1;
}

extern
void NewToken(char *t) {
	nTokenStart = nTokenNextStart;
	nTokenLength = strlen(t);
	nTokenNextStart = nBuffer;

	yylloc.first_line = nRow;
	yylloc.first_column = nTokenStart;
	yylloc.last_line = nRow;
	yylloc.last_column = nTokenStart + nTokenLength - 1;

	if (  debug  ) {
		printf("Token '%s' at %d:%d next at %d\n", dumpString(t),
												yylloc.first_column,
												yylloc.last_column, nTokenNextStart);
	}
}
	
extern
int main(int argc, char *argv[]) {
	int i;
	char *infile=NULL;

	debug = 0;
	printf("  \n");

	for (i=1; i<argc; i++) {
		if (  strcmp(argv[i], "-d") == 0  ) {
			printf("debugging activated\n");
			debug = 1;
		}
		else
			infile = argv[i];
	}

	if (  infile == NULL  )
		infile = "input.txt";

	printf("reading file '%s'\n", infile);
	file = fopen(infile, "r");
	if (  file == NULL  ) {
		printf("cannot open input\n");
		return 12;
		}

	buffer = malloc(lMaxBuffer);
	if (  buffer == NULL  ) {
		printf("cannot allocate %d bytes of memory\n", lMaxBuffer);
		fclose(file);
		return 12;
	}
	
	DumpRow();
	if (  getNextLine() == 0  )
		yyparse();

	free(buffer);
	fclose(file);
	printf("\nfinal content of variables\n");

	//watch all variables
	DumpAllVariables("  ",errorcount);
	return 0;
}
