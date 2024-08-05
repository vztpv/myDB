#ifndef FILESTREAM_H_INCLUDED
#define FILESTREAM_H_INCLUDED
//
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
#define FILE_READ_ERROR -55
class CInFile{
private:
    bool isOpen;
    ifstream* inFile;
    // 대입, 복사생성자 방지., 비교 연산 방지.
    CInFile( const CInFile& p ); // 별도 구현 없음.
    CInFile& operator=( const CInFile& p );
    bool operator==( const CInFile& p );
    // new, delete 금지..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    CInFile(){
        isOpen = false;
        inFile = new ifstream();
    }
    ~CInFile(){ // 상속해서는 안 된다.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
        if( inFile ){ delete inFile; }
    }
    bool open( string fileName ){ // 파일 이름.
        if( isOpen ){ // 닫지 않았다면..
            inFile->close();
        }
        inFile->open( fileName.data() );
        isOpen = !inFile->fail(); // 한번 실패하면 계속 실패상태이다.? //원래대로 못돌리나 ?
        if( !isOpen ){ if( inFile ) delete inFile; inFile = new ifstream(); }
        return isOpen; //실패하면 false.
    }
    void close(){
        if( isOpen ){
            inFile->close();
            isOpen = false;
        }
    }
    bool eof(){ return inFile->eof(); }
    CInFile& operator>>( int& x ){ //레퍼런스 조심.
        (*inFile) >> x;
        return *this; //  this 는 포인터.
    }
    CInFile& operator>>( char& x ){
        (*inFile) >> x;
        return * this;
    }
    CInFile& operator>>( string& x ){
        (*inFile) >> x;
        return *this;
    }
    CInFile& operator>>( char* p ){
        (*inFile) >> p;
        return *this;
    }
    void useSKIPWS(){
        if( inFile != NULL )
            inFile->unsetf( ios::skipws );
    }
    string getLine(){
        string temp;
        getline( *inFile, temp );
        return temp;
    }
    bool isOpened()const{
        return isOpen;
    }
};

class COutFile{
private:
    bool isOpen;
    ofstream* outFile;
    // 대입, 복사생성자 방지., 비교 연산 방지. //10.3
    COutFile( const COutFile& p ); // 별도 구현 없음.
    COutFile& operator=( const COutFile& p );
    bool operator==( const COutFile& p );
    // new, delete 금지..
    static void* operator new( size_t construcFunc );
    static void operator delete( void* d );
public:
    COutFile(){
        isOpen = false;
        outFile = new ofstream();
    }
    ~COutFile(){ // 상속해서는 안 된다.
        if( isOpen ){
            //cout << "close" << endl;
            close();
        }
        if( outFile ){ delete outFile; }
    }
    bool open( string fileName ){ // 파일 이름.
        if( isOpen ){ // 닫지 않았다면..
            outFile->close();
        }
        outFile->open( fileName.data() );
        isOpen = !outFile->fail(); // 한번 실패하면 계속 실패상태이다.? //원래대로 못돌리나 ?
        if( !isOpen ){ if( outFile ) delete outFile; outFile = new ofstream(); }
        return isOpen; //실패하면 false.
    }
    void close(){
        if( isOpen ){
            outFile->close();
            isOpen = false;
        }
    }
    bool eof()const{ return outFile->eof(); }

    COutFile& operator<<( const int x ){ //레퍼런스 조심.
        (*outFile) << x;
        return *this; //  this 는 포인터.
    }
    COutFile& operator<<( const char x ){ //레퍼런스 조심.
        (*outFile) << x;
        return *this; //  this 는 포인터.
    }
    COutFile& operator<<( const string& x ){
        (*outFile) << x;
        return *this;
    }
    COutFile& operator<<( const char* p ){
        (*outFile) << p;
        return *this;
    }
};
#endif // FILESTREAM_H_INCLUDED
