#include "fd.h"

FD::FD( int attributeN ){
    Initial( attributeN );
}
FD::FD( const FD& p ){
    this->attributeN = p.attributeN;

    FDIsCopy( p );

    if( 0 < p.attributeN ){
        this->attribute = new FDITEM[attributeN];
        for( int i=0;i < attributeN;i++ ){
            this->attribute[i] = p.attribute[i];
        }
    }else{
        this->attribute = NULL;
    }
    this->nol = p.nol;
    this->nor = p.nor;
}
FD::~FD(){
    Free();
}
FD& FD::operator=( const FD& p ){  //
    if( this->attributeN != p.attributeN ){ exit( -1 ); }
    if( NULL == p.attribute && NULL == this->attribute ){
    }
    else if( NULL == p.attribute ){
        Free();
    }
    else{
        if( NULL == this->attribute ){
            this->attribute = new FDITEM[attributeN];
        }
        //set
        for( int i=0;i< attributeN;i++ ){
            this->attribute[i] = p.attribute[i];
        }
        FDIsCopy( p );
        this->nol = p.nol;
        this->nor = p.nor;
    }
    return *this;
}
/** To Do
*  Reset과 중복 제거..
*/
void FD::Initial( const int attributeN ){
    attribute = new FDITEM[attributeN];
    this->attributeN = attributeN;
    Reset();
}
/* method */
FD* FD::Clone( const CLONEOPTION op )const{
    FD* temp;
    temp = new FD( *this );
    //op!!
    for( int i=0;i < attributeN;i++ ){
        if( LO == op && LEFT != temp->attribute[i] ){
            temp->attribute[i] = NOTUSE;
        }
        else if( RO == op && RIGHT != temp->attribute[i] ){
            temp->attribute[i] = NOTUSE;
        }
        else if( ALL == op ){
            break;
        }
    }
    if( LO == op ){
        temp->nor = 0;
    }else if( RO == op ){
        temp->nol = 0;
    }

    return temp;
}
void FD::Print()const{
    for( int i=0;i < attributeN;i++ ){
        switch( attribute[i] ){
        case NOTUSE:    //
            printf( "NOTUSE  " );
            break;
        case ADDLEFT:
            printf( "ADDLEFT " );
            break;
        case LEFT:
            printf( "LEFT    " );
            break;
        case RIGHT:
            printf( "RIGHT   " );
            break;
        }
    }
    cout << endl;
}
void FD::PrintAlphabet( const int attrNo )const{
	const int size = 'z' - 'a' + 1;
	const int number = attrNo / size;
	const char alpha = ( attrNo % size ) + 'A';
	cout << alpha;
	if( number > 0 ){ cout << number; }
}
void FD::PrintFD( BOOL useAlphabet )const{
    // lefts -> rights \n
    for( int i=0;i < attributeN;i++ ){
        if( LEFT == attribute[i] ){
			/** todo
			* 중복된다.. 근데 함수이름을 뭐라고 정하지??
			*/
            if( TRUE == useAlphabet ){
				PrintAlphabet( i );//
			}
			else{
				cout << i;
			}
			cout << " ";
		}
    }
    printf( "->" );
    for( int i=0;i < attributeN;i++ ){
        if( RIGHT == attribute[i] ){
            if( TRUE == useAlphabet ){
				PrintAlphabet( i );//
			}
			else{
				cout << i;
			}
			cout << " ";
        }
    }
    printf( "\n" );
    //cout << "nor is " << nor << endl;
}
void FD::PrintKey( BOOL useAlphabet )const{
    for( int i=0;i < attributeN;i++ ){
        if( LEFT == attribute[i]
           || ADDLEFT == attribute[i] ){
           if( TRUE == useAlphabet ){
				PrintAlphabet( i );//
			}
			else{
				cout << i;
			}
			cout << " ";
        }
    }
    cout << endl;
}
void FD::Free(){
    //cout << "free.." << endl;
    if( NULL != attribute ){
        delete[] attribute;
        attribute = NULL;
        attributeN = 0;
        nol = 0; nor = 0;
        FDAllIsFalse(); /// empty -> all False!!
    }
    //cout << "freed.." << endl;
}
void FD::Set( const int attrNo, const FDITEM flag ){
    // attribute[attrNo] != flag AND
    const FDITEM bef = attribute[attrNo];
    if( flag == bef ){ return; }
    // Left->RIGHT, RIGHT->LEFT, LEFT->NOT RIGHT, RIGHT->NOT LEFT
    if( LEFT == bef ){
        nol--;
    }
    else if( RIGHT == bef ){
        nor--;
    }
    if( LEFT == flag ){
        nol++;
    }
    else if( RIGHT == flag ){
        nor++;
    }
    //
    attribute[attrNo] = flag;
}
FDITEM FD::Get( const int attrNo )const{
    return attribute[attrNo];
}
//
void FD::Reset(){
    /* attribute reset */
    if( NULL != attribute ){
        for( int i=0;i < attributeN;i++ ){
            attribute[i] = NOTUSE;
        }
    }
    //
    nol = 0;
    nor = 0;
    //
    isKey = UNKNOWN;
    isBCNF = UNKNOWN;
    is3NF = UNKNOWN;
    isSuperKey = UNKNOWN;
    isFullClosure = UNKNOWN;
    isUse = TRUE; // NULLÀÌ¸é notUSE!!
    isChanged = UNKNOWN;
}
BOOL FD::isLeftPartEqual( FD* fd )const{
    if( NULL == fd ){ return FALSE; }
    if( fd->attributeN != this->attributeN ){ return UNKNOWN; } /// 추후 생각한다.
    BOOL result = TRUE;
    BOOL b1,b2;
    for( int i=0;i < attributeN;i++ ){
        if( LEFT == attribute[i] ){
            b1 = TRUE;
        }else{ b1 = FALSE; }
        if( LEFT == fd->attribute[i] ){
            b2 = TRUE;
        }else{ b2 = FALSE; }
        /* 한쪽이 LEFT이면 다른쪽도 LEFT,
        한쪽이 LEFT가 아니면 다른쪽도 LEFT가 아니어야 한다.*/
        if( b1 != b2 ){
            result = FALSE;
            break;
        }
    }
    return result;
}
/** To Do
* return nol, return nor;
*/
int FD::numOfLEFT()const{
    /*int num = 0;
    for( int i=0;i < attributeN;i++ ){
        if( LEFT == attribute[i] ){
            num++;
        }
    }
    return num;*/
    return nol;
}
int FD::numOfRIGHT()const{
    /*int num = 0;
    for( int i=0;i < attributeN;i++ ){
        if( RIGHT == attribute[i] ){
            num++;
        }
    }
    return num;*/
    return nor;
}
/* private method */
void FD::FDIsCopy( const FD& fd ){
    isKey = fd.isKey;
    isBCNF = fd.isBCNF;
    is3NF = fd.is3NF;
    isSuperKey = fd.isSuperKey;
    isFullClosure = fd.isFullClosure;
    isUse = fd.isUse;
    isChanged = fd.isChanged;
}
void FD::FDAllIsFalse(){
    isKey = FALSE;
    isBCNF = FALSE;
    is3NF = FALSE;
    isSuperKey = FALSE;
    isFullClosure = FALSE;
    isUse = FALSE;
    isChanged = FALSE;
}
