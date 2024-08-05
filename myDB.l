%{
/**
* relation Checker..
*/
/* ex) ID (COMA ID)* FD ID (COMA ID)* */
/// left�� ���� ���� ���ٰ� ����.
/* header file */
#include <stdlib.h>
#include "y.tab.h"

extern void yyerror( const char* );

extern "C" {
    int yywrap();
}
%}

%%
#[^\n]* ;

"get" { return GET; }
"Get" { return GET; }
"GET" { return GET; }
[0-9]+ {
            yylval.ival = atoi( yytext );
            return NID;
       }
[A-Z][0-9]* { yylval.sval = yytext; return SID; }
(->) { return FD; }
[,] { return COMA; }
"and" { return AND; }
"AND" { return AND; }
"And" { return AND; }

%{
/* skip whitespace */
%}
[ \t\n] ;

%{
/* anything else is error */
%}
. {  }

%%
int yywrap(){ return 1; }
// end relationChecker.l

