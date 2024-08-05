#include "sof.h"

SOF::SOF( const int fdMAX, const int attributeN ){ Initial( fdMAX, 0, attributeN );
}
SOF::SOF( const SOF& p ){
    fdMAX = p.fdMAX;
    fdN = p.fdN;
    attributeN = p.attributeN;
    fds = NULL;
    fds = new Fd*[fdMAX];
    for( int i=0;i< fdMAX;i++ ){
        if( NULL != p.fds[i] ){
            fds[i] = new Fd( *(p.fds[i]) ); // copy constructor..
        }
        else{
            fds[i] = NULL;
        }
    }
}
SOF::~SOF(){
    AllFree();
}
//protected:
void SOF::Initial( const int fdMAX, const int fdN, const int attributeN ){
    this->fdMAX = fdMAX;
    this->fdN = fdN;
    this->attributeN = attributeN;
    this->fds = new Fd*[fdMAX];

    for( int i=0;i < fdN;i++ ){
        this->fds[i] = new Fd( attributeN );
    }
    // NULL initial
    for( int i=fdN;i < fdMAX;i++ ){
        this->fds[i] = NULL;//new Fd( attributeN, FALSE );
    }
}
//void SOF::Reset(){ }
/* method */
//public:
/** To Do
* Clone, FdClone..
*/
SOF* SOF::Clone()const{
    SOF* temp;
    temp = new SOF( *this );
    return temp;
}
Fd* SOF::FdClone( const int fdNo, const CLONEOPTION op )const{
    Fd* temp = fds[fdNo]->Clone( op );
    return temp;
}

FDITEM SOF::Get( const int fdNo, const int attrNo )const{
    return fds[fdNo]->Get( attrNo );
}
void SOF::Set( const int fdNo, const int attrNo, const FDITEM flag ){
    fds[fdNo]->Set( attrNo, flag );
}
void SOF::Expand(){
    //cout << "sof expand "<< endl;
    Fd** temp;
    const int tempFdMax = fdMAX*2;
    temp = new Fd*[tempFdMax];
    for( int i=0;i < tempFdMax;i++ ){
        temp[i] = NULL;
    }
    // temp로 data를 옮긴다.
    for( int i=0;i < fdMAX;i++ ){
        temp[i] = fds[i];
    }
    // free and change.
    Free();
    fds = temp;
    fdMAX = fdMAX * 2;
}
int SOF::AddFd( Fd* fd, const int flag ){ // if flag is true, then, 이미 왼쪽이
//같은 것이 있으면 합친다.
    // 같은 것을 찾아서 있다면 합친다.
    int index = -1;// 같은 것의 index..
    if( flag == 0  ){
        for( int i=0;i < fdN;i++ ){
            /// 같은 것이 있다면,
            if( TRUE == fds[i]->isLeftPartEqual( fd ) ){
                for( int j=0;j < attributeN;j++ ){
                    if( RIGHT == fd->Get(j) ){
                        fds[i]->Set( j, RIGHT );
                    }
                }
                index = i;
                break;
            }
        }
    }
    /// 같은 것이 있다면,
    if( index > -1 ){
        delete fd;
        //cout << "같은 것이 있어서 fd삭제.." << endl;
        /*if( flag == 1 ){ // 같은 것을 아래로 가게 한다.
            //exchange fds[fdN-1], fds[i]
            Fd* temp = fds[fdN-1];
            fds[fdN-1] = fds[index];
            fds[index] = temp;

            //cout << "같은 것이 있다." << endl;
        }*/
        return index;
    }
    if( fdMAX == fdN ){
        Expand();
    }
    fds[fdN] = fd;
    fdN++;
    return fdN-1;
}

int SOF::AddFd2( Fd* fd, const int flag ){ //
    if( fdMAX == fdN ){
        Expand();
    }
    fds[fdN] = fd;
    fdN++;
    return fdN-1;
}
/** To Do
*
*/
/* void SOF::Sort(){ // Left가 적은 순으로 sort한다. O(N^2)
   int k=0;
    Array<INSORT> array( fdN );
    // Quick Sort?
    // now Bubble Sort..
    for( int i=0;i < fdN;i++ ){
        array[i].index = i;
        array[i].nol = fds[i]->numOfLEFT();
    }
    QuickSort<INSORT> qs;
    qs.sort( array, 0, array.size()-1 );
    Fd** temp = new Fd*[fdN];
    for( int i=0;i < fdN;i++ ){
        temp =
    }
}*/
void SOF::Print()const{
    if( NULL == fds ){ cout << "fds is NULL" << endl; return; }
    for( int i=0;i< fdMAX;i++ ){
        if( NULL == fds[i] ){
            cout << "NULL" << endl;
        }
        // PRINT FD
        else{
            fds[i]->Print();
            cout << endl;
        }
    }
    cout << endl;
}
/**
*
*/
void SOF::PrintFds( BOOL useAlphabet)const{
    if( NULL == fds ){ printf( "fds is NULL\n" ); return; }
    for( int i=0;i< fdMAX;i++ ){
        if( NULL != fds[i] ){
            fds[i]->PrintFd( useAlphabet );
        }
    }
    cout << endl;
}
void SOF::PrintKeys( BOOL useAlphabet )const{
    for( int i=0;i < fdN;i++ ){
        fds[i]->PrintKey( useAlphabet );
    }
    cout << endl;
}
void SOF::AllFree(){
    if( NULL != fds ){
        for( int i=0;i < fdMAX; i++ ){
            if( NULL != fds[i] ){
                delete fds[i];
            }
        }
        delete[] fds;
        fds = NULL;
    }
} /// 모든 FD의 attribute, FD array를 다 지운다.
/** To Do
*
*/
void SOF::EmptyEqualFd(){ // rename?
    BOOL temp = TRUE;
    for( int i=0;i < fdN-1;i++ ){
        for( int j=i+1;j < fdN;j++ ){
            temp = TRUE;
            for( int w=0;w < attributeN;w++ ){
                if( Get( i, w ) != Get( j, w ) ){
                    temp = FALSE;
                    break;
                }
            }
            if( TRUE == temp ){
                for( int k=0;k < attributeN;k++ ){
                    Set( j, k, NOTUSE );
                }
            }
        }
    }
}
void SOF::DeleteEmptyLR(){ // empty Left, right..
    // LEFT개수가 0이거나 RIGHT개수가 0인 fd를 제거하고 순서를 맞춘다.
    for( int i=0;i < fdN;i++ ){
        if( 0 == fds[i]->numOfLEFT() ||
            0 == fds[i]->numOfRIGHT() ){
           delete fds[i];
            fds[i] = NULL;
        }
    }
    //
    int size = 0;
    for( int i=0;i < fdN;i++ ){
        if( NULL != fds[i] ){
            size++;
        }
    }
    int j=-1;
    for( int i=0;i < size;i++ ){
        do{
            j++;
        }while( NULL == fds[j] ); /// 무한 루프..조심!!
        //
        fds[i] = fds[j];
    }
    for( int i=size;i < fdMAX;i++ ){
        fds[i] = NULL;
    }
    fdN = size; /// check...
}
//
void SOF::Simple(){
    Fd** temp = new Fd*[fdN];
    int trueFdN = fdN;
    for( int i=0;i < fdN;i++ ){
        temp[i] = fds[i];
        fds[i] = NULL;
    }
    fdN = 0;
    for( int i=0;i < trueFdN;i++ ){
        AddFd( temp[i], 0 );
    }
    delete[] temp;
    //
    EmptyEqualFd();
    DeleteEmptyLR();
} /// 중복, LEFT가 없는 경우, RIGHT가 없는 경우를 제거한다.
void SOF::SimpleKey(){
    // LEFT와 ADDLEFT..
    BOOL temp = TRUE, temp2 = FALSE;
    for( int i=0;i < fdN;i++ ){
        for( int j=0;j < fdN;j++ ){
            if( i != j ){
                temp = TRUE; temp2 = FALSE;
                for( int k=0;k < attributeN;k++ ){
                    if( LEFT == Get( j, k ) ||
                        ADDLEFT == Get( j, k ) ){
                        temp2 = TRUE;
                        if( !( LEFT == Get( i, k ) ||
                           ADDLEFT == Get( i, k ) ) ){
                            temp = FALSE;
                            break;
                        }
                    }
                }
                if( TRUE == temp && TRUE == temp2 ){
                    for( int w=0;w < attributeN;w++ ){
                        Set( i, w, NOTUSE );
                    }
                }
            }
        }
    }
    DeleteEmptyLR();
}
//Total Closure
/*void SOF::BeClosure(){
    // algorithms.
    BOOL can=TRUE;
    BOOL end=TRUE; //
	const int iterMax = 1000; int iter = 0; /// use ITER MAX = 1000 // make MACRO!!
    ////////////
    while( TRUE ){
        end = TRUE;
        for( int i=0;i < fdN; i++ ){ //fd의 set이 fd2의 LEFT에 포함되는가?
            for( int j=0;j < fdN; j++ ){
                can = FALSE;
                if( i != j ){
                    // closure에 추가 할 것이 있는지 찾는다.
                    for( int k=0;k < attributeN;k++ ){
                        // fdset가 하나라도 fd2의 LEFT에 없다면, FALSE
                        if( LEFT == fds[j]->Get(k) ){
                            can = TRUE;
                            if( NOTUSE == fds[i]->Get(k) ){
                                can = FALSE; break;
                            }
                        }
                    }
                    // can == TRUE, add closure dofd[i]->rightShort..
                    if( TRUE == can ){
                        for( int k=0;k< attributeN;k++ ){
                            if( RIGHT == fds[j]->Get(k) ){
                                if( NOTUSE == fds[i]->Get(k) ){
                                    fds[i]->Set( k, RIGHT );
                                    end = FALSE;
                                }
                            }
                        }
                    }
                }
            }
        }
        if( TRUE == end ){
            break;
        }
		iter++;
		if( iter >= iterMax ){ cout << "iterMAX over" << endl; exit( -5 ); }
    }
}*/
//Part Closure
void SOF::BeClosure( const int checki ){
    BOOL can=TRUE;
    BOOL end=FALSE;
    const int trueFdN = fdN-1;
	const int iterMax = 1000; int iter = 0; /// use ITER MAX = 1000 // make MACRO!!
    ////////////
    while( !end ){
        end = TRUE;
        for( int j=0;j < trueFdN; j++ ){
            can = FALSE;
            // closure에 추가 할 것이 있는지 찾는다.
            for( int k=0;k < attributeN;k++ ){
                // fdset가 하나라도 fd2의 LEFT에 없다면, FALSE
                if( LEFT == fds[j]->Get(k) ){
                    can = TRUE;
                    if( NOTUSE == fds[checki]->Get(k) ){
                        can = FALSE; break;
                    }
                }
            }
            // can == TRUE, add closure dofd[i]->rightShort..
            if( TRUE == can ){
                for( int k=0;k< attributeN;k++ ){
                    if( RIGHT == fds[j]->Get(k) ){
                        if( NOTUSE == fds[checki]->Get(k) ){
                            fds[checki]->Set( k, RIGHT );
                            end = FALSE;
                        }
                    }
                }
            }
        }
        iter++;
        if( iter >= iterMax ){
            cout << "iter >= iterMax" << endl;
            exit( -1 );
        }
    }
}
void SOF::ExchangeFdInMinimalCover( const int fdNo, const int right, const int left ){
    Fd* fdTemp = this->fds[fdNo]->Clone( LO );
    SOF* sof = this->Clone();
    int index;
    fdTemp->Set( left, NOTUSE );
    index = sof->AddFd( fdTemp, 0 );
    //
    sof->BeClosure( index );
    //
    if( RIGHT == sof->Get( index, right ) ){
        Fd* fdTemp = fds[fdNo]->Clone( LO );
        fdTemp->Set( left, NOTUSE );
        this->Set( fdNo, right, NOTUSE );
        fdTemp->Set( right, RIGHT );
        index = this->AddFd( fdTemp, 0 );
        //
        //cout << "AddFd" << endl;
    }
    if( sof->fdN > 1000 ){
        cout << "fdN is " << fdN << endl;
    }
    /// delete temp /// cf using clone then more easy!! // later change!!
    delete sof; sof = NULL;
    //delete fdTemp; fdTemp = NULL;

}
//get
void SOF::GetClosure( Fd* fd ){
	SOF* sof = this->Clone(); // new

	sof->AddFd( fd, -1 );
	sof->BeClosure( sof->fdN-1 );
	sof->fds[sof->fdN-1] = NULL;

	delete sof; // remove
}
//

void SOF::BeMinimalCover(){
    if( 0 == this->fdN ){ cout << "fdN==0" << endl; return; }
    //this->BeClosure(); // not use!!
    //this->Simple();

    /** check if (X-{B}}+ contains A? */
    int tempFDN = this->fdN;
    for( int i=0;i < tempFDN;i++ ){
        if( this->fds[i]->numOfLEFT() >= 2 ){
            /// DEBUG
            //cout << "check" << " i is " << i << endl;
            //getch();
            ///
            for( int A=0;A < this->attributeN;A++ ){ /// 순서 주의...
				if( RIGHT == this->Get( i, A ) ){
                    for( int B=0;B < this->attributeN;B++ ){
                        if( LEFT == this->Get( i, B )  //
                            ){ // is A
                            this->ExchangeFdInMinimalCover( i, A, B );
                            if( LEFT != this->Get( i, B ) ){
                                break; // do for next A
                            }
                        }
                    }
				}
            }

        }
    }
    //this->Simple();
    /** check if X+ contains A using ( G-{X->A} ) */
    for( int i=0;i < fdN;i++ ){
        for( int A=0;A< attributeN;A++ ){
            if( RIGHT == this->Get( i, A ) ){
                SOF* sofTemp = this->Clone();
                Fd* fdTemp = this->FdClone( i, LO );
                //fdTemp->Set( A, NOTUSE );
                sofTemp->Set( i, A, NOTUSE );
                sofTemp->AddFd( fdTemp, -1 );
                /////////////////////////////
                sofTemp->BeClosure( sofTemp->fdN - 1 );
                // check if X+
                if( RIGHT == sofTemp->Get( sofTemp->fdN-1, A ) ){
                    this->Set( i, A, NOTUSE );
                }
                delete sofTemp; sofTemp = NULL;
            }
        }
    }
    //this->Simple();
}
void SOF::BeKey(){ /// Be+하고나서..
    SOF* sofTemp = this->Clone();
    // add not exist in Closure.. attributes
    for( int i=0;i < fdN; i++ ){
    	for( int j=0;j < attributeN; j++ ){
    		if( NOTUSE == this->Get( i, j ) ){
    		    this->Set( i, j, ADDLEFT ); //
    		}
    	}
    }
    // if X->A, X in closure, A is ADDLEFT then remove A!!
    for( int i=0;i < fdN;i++ ){
        for( int A=0;A < attributeN;A++ ){
            if( ADDLEFT == this->Get( i, A ) ){
                // make function?
                SOF* temp = sofTemp->Clone();
                Fd* fdTemp = this->FdClone( i, ALL );

                for( int j=0;j< this->attributeN;j++ ){
                    if( RIGHT == fdTemp->Get( j ) || j == A ){
                        fdTemp->Set( j, NOTUSE );
                    }
                }
                temp->Set( i, A, NOTUSE ); /// remove X->A
                temp->AddFd( fdTemp, -1 );
                temp->BeClosure( temp->fdN - 1 );
                if( RIGHT == temp->Get( temp->fdN-1, A ) ){
                    this->Set( i, A, NOTUSE );
                }delete temp;
            }
        }
    }
    delete sofTemp;

    // 중복, LEFT가 EMPTY, RIGHT가 EMPTY인경우를 제외한다.
    //Simple();
    // LEFT와 ADDLEFT를 포함해서, 다른 FD들에 포함되면
    // 제외한다...
    //SimpleKey();
}
int SOF::GetFdN()const{
    return fdN;
}
/* only using in class function */
//private: not use?
void SOF::Free(){/// FD array만 지운다.
    if( NULL != fds ){
        delete[] fds;
        fds = NULL;
        fdN = 0;
    }
}
