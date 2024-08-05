%{
/**
* Relation Checker
*
*/
/** To Do
* ID - 숫자번호 범위 오버 문제!! - 세심하게..?
* 지금은 구현하지 않았음..- 올바르게 숫자가 적혀있다고 가정..
*/
/** To Do
* delete 문제 - 이건 비쥬얼 스튜디오를 이용해서..!!
*/
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
using namespace std;
#define DEBUG 1 // 1이면 디버그..
#define TEST 1

#include "fd.h"
#include "sof.h"
#include "NewFileStream.h"
/* yacc */
int yyparse( void );
int yylex( void );
/* variables */
    // temp for grammar function and initial fd
Fd* fdTemp;
int attributeN;
    // main value..
SOF* mainSOF=NULL;
const int FDMAX = 20;
/* function interface */
void yyerror( const char* s ); // const char, ERROR //
void debug( char* text );
void checkIdRange( const int id );
%}

%union {
    int ival; // integer를 float형태로 저장한다.
    char* sval;
}

%{
    // AND is 구분자..
%}
%token <ival> NID
%token <sval> SID
%token FD
%token COMA
%token AND
%token GET

%type <ival> ID
%%
program:
    AttrN FDRules GET { /// todo - ex) get: get MINIMAL of 1,2,3, ///
        delete fdTemp; fdTemp = NULL; //cf) Free( <T>** ){ delete <T>*, <T>* = NULL ? }
        // get...
        SOF* temp = ( mainSOF->Clone() );
        temp->PrintFds( TRUE );
        delete temp; temp = NULL;
        temp = ( mainSOF->Clone() );

        cout << "minimal cover" << endl;
        temp->BeMinimalCover();
        SOF* temp2 = temp->Clone();
        temp2->Simple();
        temp2->PrintFds( TRUE );
		temp2->PrintFds();
        delete temp2;

        cout << "keys" << endl;
        temp2 = temp->Clone();
		temp2->BeKey();
        temp2->Simple();
        temp2->SimpleKey();
		temp2->PrintKeys( TRUE );
        temp2->PrintKeys();
		delete temp2;
       // cout << "getClosure test" << endl;
     //   Fd* fd;
       // fd = new Fd( attributeN );
      //  fd->Set( 0, LEFT );
       // fd->Set( 1, LEFT );

        //temp->BeMinimalCover();
       // temp->GetClosure( fd );

        //fd->Print();
       // fd->PrintFd( TRUE );
        delete temp;
       // delete fd;
    }
AttrN:
    NID {
        attributeN = $1;//
        if( attributeN < 0 ){ yyerror( "attributeN < 0" ); }
        // fdTemp, mainSOF MAKE
        fdTemp = new Fd( attributeN );
        mainSOF = new SOF( FDMAX, attributeN );
    }
    |
    SID {
        if( strlen( $1 ) == 1 ){ // ONLY Large Alphabet
            attributeN = $1[0] - 'A';
        }
        else{
            int number = atoi( $1 +1 );
            char alphabet = $1[0];
            attributeN = alphabet - 'A' + number * ( 'Z' - 'A' + 1 );
        }
        attributeN = attributeN + 1;
        //
        if( attributeN < 0 ){ yyerror( "attributeN < 0" ); }
        // fdTemp, mainSOF MAKE
        fdTemp = new Fd( attributeN );
        mainSOF = new SOF( FDMAX, attributeN );
    }
FDRules:
    FDRule AND FDRules
    |
    ;
FDRule:
    LeftIDs FD RightIDs {
        fdTemp->Reset(); // for next
    }
LeftIDs:
    ID COMA LeftIDs {
        checkIdRange( $1 );
        fdTemp->Set( $1, LEFT );
    }
    | ID {
        checkIdRange( $1 );
        fdTemp->Set( $1, LEFT );
    }
    |

    ;
RightIDs:
    ID COMA RightIDs {
        checkIdRange( $1 );
        Fd* temp = fdTemp->Clone();
        
        temp->Set( $1, RIGHT );

        mainSOF->AddFd2( temp ); //
    }
    | ID {
        checkIdRange( $1 );  
        Fd* temp = fdTemp->Clone();
        
        temp->Set( $1, RIGHT );

        mainSOF->AddFd2( temp ); //
    }
    |
    ;
ID:
    NID {
        $$ = $1;//
    }
    |
    SID {
        if( strlen( $1 ) == 1 ){ // ONLY Large Alphabet
            $$ = $1[0] - 'A';
        }
        else{
            int number = atoi( $1 +1 );
            char alphabet = $1[0];
            $$ = alphabet - 'A' + number * ( 'Z' - 'A' + 1 );
        }
        int index;
    }
%%
/* main */
int main( int argc, char** argv ){
    FILE* file;
    extern FILE* yyin;

    if( argc != 2 ){ return -1; }
	file = fopen( argv[1], "rt" );
	if( !file ){ printf( "file read error\n" ); return -2; }

	// FIND
	yyin = file;

    yyparse();

	// FILE END
	fclose( yyin );
    return 0;
}

/* function definition */
void yyerror( const char* s ){
    printf( "yyerror %s\n", s );
    exit( -1 );
}
void debug( char* text ){
    #if DEBUG == 1
    printf( "debug text is %s\n", text );
    #endif
}
void checkIdRange( const int id ){
    if( id < 0 || id >= attributeN ){
        cout << "ID is " << id << ", over range [0,attributeN)" << "-";
        yyerror( "" );
    }
}
// end program
