#ifndef Fd_H_INCLUDED
#define Fd_H_INCLUDED

#include <stdlib.h>
#include <iostream>
using namespace std;
/** todo 
* global.h - add BOOL
*/
enum BOOL { FALSE=0, TRUE=1, UNKNOWN=-2 };
typedef enum BOOL BOOL;
/* Fd items */
enum FDITEM { NOTUSE, ADDLEFT, LEFT, RIGHT };
typedef enum FDITEM FDITEM;
/* clone option */
enum CLONEOPTION { ALL, LO, RO }; // all, LeftOnly, RightOnly( 다른것은 다 NOTUSE로 )
typedef enum CLONEOPTION CLONEOPTION;

class Fd{
/* default Functions */
public:
    /** attrExist 제거? */
	explicit Fd( const int attributeN );
	Fd( const Fd& p );
    ~Fd();
    /* operator */
    Fd& operator=( const Fd& p ); // attributeN가 같을때 가능, p와 같아진다. 단,  Deep Copy!!
protected:
    void Initial( const int attributeN );
/* entry */
private:
    FDITEM* attribute;
    // 아래를 class로 묶는다.?
    BOOL isKey; // default is UNKNOWN
    BOOL isBCNF;
    BOOL is3NF;
    BOOL isSuperKey;
    BOOL isFullClosure;
    BOOL isUse; // if) minimalCover FALSE, TRUE., default is UNKNOWN..
    BOOL isChanged;// 사용하지 않는다?
    ////////////////
    int attributeN;
    int nol, nor;
/* method */
public:
    Fd* Clone( const CLONEOPTION op=ALL )const;
    void Print()const;
    void PrintFd( BOOL useAlphabet=FALSE )const;
    void PrintKey( BOOL useAlphabet=FALSE )const;
	//
    void Free(); /// attribute를 free하고, is~ false로, attribute=NULL, attrN = 0;
	//
    void Set( const int attrNo, const FDITEM flag );
    FDITEM Get( const int attrNo )const;
    //
    void Reset();
    /** AttributeReset() */
    /* is~ */
    BOOL isLeftPartEqual( Fd* fd )const;
    /* numOf~ */
    int numOfLEFT()const;
    int numOfRIGHT()const;
/* only using in class function */
private:
    void FdIsCopy( const Fd& fd );
    void FdAllIsFalse();
	//
	void PrintAlphabet( const int attrNo )const;
/* do not */
private:
};

#endif // Fd_H_INCLUDED
