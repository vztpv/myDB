#ifndef SOF_H_INCLUDED
#define SOF_H_INCLUDED

#include "fd.h"

class INSORT{
public:
    int index;
    int nol;
public:
    bool operator<( const INSORT& p ){
        return this->nol < p.nol;
    }
};
class SOF{
/* default Functions */
public:
	explicit SOF( const int fdMAX, const int attributeN );
	SOF( const SOF& p );
    ~SOF();
protected:
    void Initial( const int fdMAX, const int fdN, const int attributeN  );
    //void Reset();
/* entry */
private:
    Fd** fds;
    int fdMAX; //
    int fdN; //
    int attributeN;
/* method */
public:
    SOF* Clone()const;
    Fd* FdClone( const int fdNo, const CLONEOPTION op )const;
    void Expand();
    int AddFd( Fd* fd, int flag=0 );
    int AddFd2( Fd* fd, int flag=0 ); 

    ///int flag ���, bool flag?
							/// flag == 0 then Left�� ���� ���� �ִٸ� ã�Ƽ���ģ��.
							/// flag == -1 then �׳� �������� �߰��Ѵ�.
    void Print()const;
	/** todo
	* print fd,key using alphabet!!
	// BOOL useAlphabet
	*/
    void PrintFds( BOOL useAlphabet = FALSE )const;
    void PrintKeys( BOOL useAlphabet = FALSE )const;
    void AllFree(); /// ��� FD�� attribute, FD array�� �� �����.
    void Simple(); /// �ߺ�, LEFT�� ���� ���, RIGHT�� ���� ��츦 �����Ѵ�.
    void SimpleKey();
    //
    void BeClosure(); /** fd�� �߰�, ������ ����. */
    void BeClosure( const int checki );
    void ExchangeFdInMinimalCover( const int fdNo, const int right, const int left );
    void BeMinimalCover(); /** fd�� �߰�, ������ �Ͼ �� �ִ�. */
	//
	void GetClosure( Fd* fd );
	// fd-�����Ҵ�.. fd�� ����Ű�� ���� �����Ѵ�. L->(L�� Ŭ���� )
	//
    void BeKey();
    //
    FDITEM Get( const int fdNo, const int attrNo )const;
    void Set( const int fdNo, const int attrNo, const FDITEM flag );

    int GetFdN()const;
/* only using in class function */
private:
    void Free(); /// FD array�� �����.
    void EmptyEqualFd(); // rename?
    void DeleteEmptyLR(); // empty Left, right..
/* do not */
private:
    /** To Do
    *
    */
	const SOF& operator=( const SOF& p ); /// �ʿ��� �� ����?
	/// attributeN�� ������...
};

#endif // SOF_H_INCLUDED
