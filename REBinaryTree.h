//
// Created by 星落_月残 on 2018/5/16.
//

#ifndef COMPILER_CONSTANTCHARACTER
#define COMPILER_CONSTANTCHARACTER

constexpr char EPISILON=static_cast<char>(24);
constexpr char PARALLEL=static_cast<char>(23);
constexpr char CLUSTER=static_cast<char>(22);
constexpr char CLOSURE=static_cast<char>(21);
constexpr char RIGHT_BRACE=static_cast<char>(20);
constexpr char LEFT_BRACE=static_cast<char>(19);
constexpr char FINAL=static_cast<char>(18);
constexpr char TERMINATOR=static_cast<char>(17);
constexpr char DERIVATION=static_cast<char>(16);

#endif


#ifndef COMPILER_REBINARYTREE_H
#define COMPILER_REBINARYTREE_H

//#include "BasicBinaryTree.h"
#include "MultipurposeSequence.h"


bool isValidChar(char input);


template<class T>
class REBinaryClient
{
public:
	REBinaryClient();
	explicit REBinaryClient(const T &toWrite);
	REBinaryClient(const T &toWrite,long long inIndex);
	REBinaryClient(const T &toWrite,REBinaryClient<T> *parent,REBinaryClient<T> *left,REBinaryClient<T> *right);
	T &getDataR();
	long long int &getIndexR();
	REBinaryClient<T> *&getPSeniorR();
	REBinaryClient<T> *&getPLeftJuniorR();
	REBinaryClient<T> *&getPRightJuniorR();
	Sequence<long long int> &getFollowposR();
	Sequence<long long int> &getFirstposR();
	Sequence<long long int> &getLastposR();
	bool &getNullableR();
	T getData() const;
	long long int getIndex() const;
	REBinaryClient<T> *getPSenior() const;
	REBinaryClient<T> *getPLeftJunior() const;
	REBinaryClient<T> *getPRightJunior() const;
	const Sequence<long long int> &getFollowpos() const;
	const Sequence<long long int> &getFirstpos() const;
	const Sequence<long long int> &getLastpos() const;
	bool getNullable() const;
private:
	T Data;
	long long Index;
	REBinaryClient<T> *pSenior;
	REBinaryClient<T> *pLeftJunior;
	REBinaryClient<T> *pRightJunior;
	Sequence<long long> followpos;
	Sequence<long long> firstpos;
	Sequence<long long> lastpos;
	bool nullable;

};

template<class T>
REBinaryClient<T>::REBinaryClient()
{
	Index=0;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;
}
template<class T>
REBinaryClient<T>::REBinaryClient(const T &toWrite)
{
	Data=toWrite;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;
	Index=-2;
}
template<class T>
REBinaryClient<T>::REBinaryClient(const T &toWrite,long long inIndex)
{
	Data=toWrite;
	Index=inIndex;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;
}
template<class T>
REBinaryClient<T>::REBinaryClient(const T &toWrite,REBinaryClient<T> *parent,REBinaryClient<T> *left,REBinaryClient<T> *right)
{
	Data=toWrite;
	pSenior=parent;
	pLeftJunior=left;
	pRightJunior=right;
}
template<class T>
T &REBinaryClient<T>::getDataR()
{
	return Data;
}
template<class T>
long long int &REBinaryClient<T>::getIndexR()
{
	return Index;
}
template<class T>
REBinaryClient<T> *&REBinaryClient<T>::getPSeniorR()
{
	return pSenior;
}
template<class T>
REBinaryClient<T> *&REBinaryClient<T>::getPLeftJuniorR()
{
	return pLeftJunior;
}
template<class T>
REBinaryClient<T> *&REBinaryClient<T>::getPRightJuniorR()
{
	return pRightJunior;
}
template<class T>
Sequence<long long int> &REBinaryClient<T>::getFollowposR()
{
	return followpos;
}
template<class T>
Sequence<long long int> &REBinaryClient<T>::getFirstposR()
{
	return firstpos;
}
template<class T>
Sequence<long long int> &REBinaryClient<T>::getLastposR()
{
	return lastpos;
}
template<class T>
bool &REBinaryClient<T>::getNullableR()
{
	return nullable;
}

template<class T>
T REBinaryClient<T>::getData() const
{
	return Data;
}
template<class T>
long long int REBinaryClient<T>::getIndex() const
{
	return Index;
}
template<class T>
REBinaryClient<T> *REBinaryClient<T>::getPSenior() const
{
	return pSenior;
}
template<class T>
REBinaryClient<T> *REBinaryClient<T>::getPLeftJunior() const
{
	return pLeftJunior;
}
template<class T>
REBinaryClient<T> *REBinaryClient<T>::getPRightJunior() const
{
	return pRightJunior;
}
template<class T>
const Sequence<long long int> &REBinaryClient<T>::getFollowpos() const
{
	return followpos;
}
template<class T>
const Sequence<long long int> &REBinaryClient<T>::getFirstpos() const
{
	return firstpos;
}
template<class T>
const Sequence<long long int> &REBinaryClient<T>::getLastpos() const
{
	return lastpos;
}
template<class T>
bool REBinaryClient<T>::getNullable() const
{
	return nullable;
}


template<class T>
class REBinaryTree
{
public:
	REBinaryTree();
	explicit REBinaryTree(REBinaryClient<T> *inRoot);
	explicit REBinaryTree(const REBinaryTree <T> &another);
	REBinaryTree <T> &operator=(const REBinaryTree <T> &another);
	REBinaryClient<T> *getRERoot() const;
	~REBinaryTree();
	REBinaryClient<T> *getAddressFromData(REBinaryClient<T> *Entrance,const T &toFind);
	REBinaryClient<T> *getAddressFromIndex(REBinaryClient<T> *Entrance,long long inIndex);
	int getHeightRecursive(REBinaryClient<T> *Entrance);
	int getNumberOfLeavesRecursive(REBinaryClient<T> *Entrance);
	int getNumberofNonLeavesRecursive(REBinaryClient<T> *Entrance);
	void PreorderTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef);//此处便于传入参数实现更多功能
	void InorderTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef);
	void PostTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef);
	void LevelTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef);
	void LevelTraverseRecursive(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef,int level);
	void InvertREBinaryTree(REBinaryClient<T> *Entrance);
	ErrorCode_List DeleteSubTree(REBinaryClient<T> *Entrance);
	ErrorCode_List DeleteSingleClient(REBinaryClient<T> *Entrance);//类似二分查找树

protected:
	void REcopy(REBinaryClient<T> *From,REBinaryClient<T> *&To,REBinaryClient<T> *To_Senior);
	int REcount;
	REBinaryClient<T> *RERoot;
};


template<class T>
REBinaryTree<T>::REBinaryTree()
{
	REcount=0;
	RERoot=nullptr;
}
template<class T>
REBinaryTree<T>::REBinaryTree(REBinaryClient<T> *inRoot)
{
	RERoot=inRoot;
	REcount=getNumberOfLeavesRecursive(RERoot)+getNumberofNonLeavesRecursive(RERoot);
}
template<class T>
REBinaryTree<T>::REBinaryTree(const REBinaryTree <T> &another)
{
	REcount=another.REcount;
	RERoot=new REBinaryClient<T>;
	REcopy(another.RERoot,RERoot,nullptr);
}
template<class T>
REBinaryTree<T> &REBinaryTree<T>::operator=(const REBinaryTree <T> &another)
{
	if(&another==this)
		return *this;
	else if(another.RERoot==nullptr)
	{
		RERoot=nullptr;
		REcount=0;
	}
	else
	{
		//利用析构函数
		REBinaryTree<T> toDelete(another);
		Swap(toDelete.RERoot,this->RERoot);
		Swap(toDelete.REcount,this->REcount);
	}
	return *this;
}
template<class T>
REBinaryTree<T>::~REBinaryTree()
{
	DeleteSubTree(RERoot);
}
template<class T>
REBinaryClient<T> *REBinaryTree<T>::getAddressFromData(REBinaryClient<T> *Entrance,const T &toFind)
{
	if(Entrance==nullptr)
		return nullptr;
	else
	{
		if(Entrance->getData()==toFind)
			return Entrance;

		REBinaryClient<T> *LeftAnswer=getAddressFromData(Entrance->getPLeftJunior(),toFind);
		if(LeftAnswer!=nullptr)
			return LeftAnswer;
		REBinaryClient<T> *RightAnswer=getAddressFromData(Entrance->getPRightJunior(),toFind);
		if(RightAnswer!=nullptr)
			return RightAnswer;
		return nullptr;
	}
}
template<class T>
REBinaryClient<T> *REBinaryTree<T>::getAddressFromIndex(REBinaryClient<T> *Entrance,long long inIndex)
{
	if(Entrance==nullptr)
		return nullptr;
	if(Entrance->getPLeftJunior()==nullptr&&Entrance->getPRightJunior()==nullptr)
	{
		if(Entrance->getIndex()!=inIndex)
			return nullptr;
		else
			return Entrance;
	}
	REBinaryClient<T> *leftResult=getAddressFromIndex(Entrance->getPLeftJunior(),inIndex);
	REBinaryClient<T> *rightResult=getAddressFromIndex(Entrance->getPRightJunior(),inIndex);
	if(leftResult!=nullptr)
		return leftResult;
	else if(rightResult!=nullptr)
		return rightResult;
	else
		return nullptr;

}
template<class T>
int REBinaryTree<T>::getHeightRecursive(REBinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return 0;
	int leftHeight=getHeightRecursive(Entrance->getPLeftJuniorR());
	int rightHeight=getHeightRecursive(Entrance->getPRightJuniorR());
	if(leftHeight>rightHeight)
		return leftHeight+1;
	else
		return rightHeight+1;
}
template<class T>
int REBinaryTree<T>::getNumberOfLeavesRecursive(REBinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return 0;
	else if(Entrance->getPLeftJuniorR()==nullptr&&Entrance->getPRightJuniorR()==nullptr)
	{
		return 1;
	}
	else
	{
		int tmpCount=0;
		tmpCount+=getNumberOfLeavesRecursive(Entrance->getPLeftJuniorR());
		tmpCount+=getNumberOfLeavesRecursive(Entrance->getPRightJuniorR());
		return tmpCount;
	}
}
template<class T>
int REBinaryTree<T>::getNumberofNonLeavesRecursive(REBinaryClient<T> *Entrance)
{
	if(Entrance==nullptr||(Entrance->getPLeftJuniorR()==nullptr&&Entrance->getPRightJuniorR()==nullptr))
		return 0;
	else
	{
		int tmpCount=1;
		tmpCount+=getNumberofNonLeavesRecursive(Entrance->getPLeftJuniorR());
		tmpCount+=getNumberofNonLeavesRecursive(Entrance->getPRightJuniorR());
		return tmpCount;
	}

}
template<class T>
void REBinaryTree<T>::PreorderTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef)
{
	if(Entrance==nullptr)
		return;
	else
	{
		visit(Entrance,paraRef);
		PreorderTraverse(Entrance->getPLeftJuniorR(),visit,paraRef);
		PreorderTraverse(Entrance->getPRightJuniorR(),visit,paraRef);
	}
}
template<class T>
void REBinaryTree<T>::InorderTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef)
{
	if(Entrance==nullptr)
		return;
	else
	{
		InorderTraverse(Entrance->getPLeftJuniorR(),visit,paraRef);
		visit(Entrance,paraRef);
		InorderTraverse(Entrance->getPRightJuniorR(),visit,paraRef);
	}
}
template<class T>
void REBinaryTree<T>::PostTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef)
{
	if(Entrance==nullptr)
		return;
	else
	{
		PostTraverse(Entrance->getPLeftJuniorR(),visit,paraRef);
		PostTraverse(Entrance->getPRightJuniorR(),visit,paraRef);
		visit(Entrance,paraRef);
	}
}
template<class T>
void REBinaryTree<T>::LevelTraverse(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef)
{
	int level=getHeightRecursive(Entrance);
	if(Entrance==nullptr||level<0)
		return;
	else
		for(int i=0;i<level;i++)
		{
			LevelTraverseRecursive(Entrance,visit,paraRef,i);
		}
}
template<class T>
void REBinaryTree<T>::LevelTraverseRecursive(REBinaryClient<T> *Entrance,void (*visit)(REBinaryClient<T> *,void *),void *paraRef,int level)
{
	if(Entrance==nullptr||level<0)
		return;
	else if(level==0)
		visit(Entrance,paraRef);
	else
	{
		LevelTraverseRecursive(Entrance->getPLeftJuniorR(),visit,paraRef,level-1);
		LevelTraverseRecursive(Entrance->getPRightJuniorR(),visit,paraRef,level-1);
	}
}
template<class T>
void REBinaryTree<T>::InvertREBinaryTree(REBinaryClient<T> *Entrance)
{
	if(Entrance!=nullptr)
	{
		REBinaryClient<T> *tmpPointer;
		if(Entrance->getPLeftJuniorR()!=nullptr||Entrance->getPRightJuniorR()!=nullptr)
		{
			tmpPointer=Entrance->getPLeftJuniorR();
			Entrance->getPLeftJuniorR()=Entrance->getPRightJuniorR();
			Entrance->getPRightJuniorR()=tmpPointer;
			InvertREBinaryTree(Entrance->getPLeftJuniorR());
			InvertREBinaryTree(Entrance->getPRightJuniorR());
		}
	}
}
template<class T>
ErrorCode_List REBinaryTree<T>::DeleteSubTree(REBinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return success;
	else
	{
		int sum=0;
		sum+=getNumberOfLeavesRecursive(Entrance);
		sum+=getNumberofNonLeavesRecursive(Entrance);
		REcount-=sum;
		DeleteSubTree(Entrance->getPLeftJuniorR());
		DeleteSubTree(Entrance->getPRightJuniorR());
		delete Entrance;
		Entrance=nullptr;
		return success;
	}
}
template<class T>
ErrorCode_List REBinaryTree<T>::DeleteSingleClient(REBinaryClient<T> *Entrance)
{
	if(Entrance==REBinaryTree<T>::RERoot)
	{
		REBinaryClient<T> *Precursor=RERoot->getPLeftJuniorR();
		for(;Precursor->getPRightJuniorR()!=nullptr;Precursor=Precursor->getPRightJuniorR());
		REBinaryTree<T>::RERoot->getDataR()=Precursor->getDataR();
		REBinaryTree<T>::RERoot->getIndexR()=Precursor->getIndexR();
		DeleteSingleClient(Precursor);
	}
	else if(Entrance->getPLeftJuniorR()==nullptr&&Entrance->getPRightJuniorR()==nullptr)
	{
		if(Entrance->getPSeniorR()->getPLeftJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPLeftJuniorR()=nullptr;
		else if(Entrance->getPSeniorR()->getPRightJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPRightJuniorR()=nullptr;
		delete Entrance;
	}
	else if(Entrance->getPLeftJuniorR()!=nullptr&&Entrance->getPRightJuniorR()==nullptr)
	{
		if(Entrance->getPSeniorR()->getPLeftJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPLeftJuniorR()=Entrance->getPLeftJuniorR();
		else if(Entrance->getPSeniorR()->getPRightJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPRightJuniorR()=Entrance->getPLeftJuniorR();
		Entrance->getPLeftJuniorR()->getPSeniorR()=Entrance->getPSeniorR();
		delete Entrance;
	}
	else if(Entrance->getPRightJuniorR()!=nullptr&&Entrance->getPLeftJuniorR()==nullptr)
	{
		if(Entrance->getPSeniorR()->getPLeftJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPLeftJuniorR()=Entrance->getPRightJuniorR();
		else if(Entrance->getPSeniorR()->getPRightJuniorR()==Entrance)
			Entrance->getPSeniorR()->getPRightJuniorR()=Entrance->getPRightJuniorR();
		Entrance->getPRightJuniorR()->getPSeniorR()=Entrance->getPSeniorR();
		delete Entrance;
	}
	else if(Entrance->getPLeftJuniorR()!=nullptr&&Entrance->getPRightJuniorR()!=nullptr)
	{
		REBinaryClient<T> *Precursor=Entrance->getPLeftJuniorR();
		for(;Precursor->getPRightJuniorR()!=nullptr;Precursor=Precursor->getPRightJuniorR());
		Entrance->getDataR()=Precursor->getDataR();
		Entrance->getIndexR()=Precursor->getIndexR();
		DeleteSingleClient(Precursor);
	}
}
template<class T>
void REBinaryTree<T>::REcopy(REBinaryClient<T> *From,REBinaryClient<T> *&To,REBinaryClient<T> *To_Senior)
{
	if(From==nullptr)
	{
		To=nullptr;
		return;
	}
	else
	{
		if(To==nullptr)
			To=new REBinaryClient<T>;
		To->getDataR()=From->getDataR();
		To->getIndexR()=From->getIndexR();
		To->getFollowposR()=From->getFollowpos();
		To->getFirstposR()=From->getFirstpos();
		To->getLastposR()=From->getLastpos();
		To->getNullableR()=From->getNullable();
		To->getPSeniorR()=To_Senior;
		REcopy(From->getPLeftJuniorR(),To->getPLeftJuniorR(),To);
		REcopy(From->getPRightJuniorR(),To->getPRightJuniorR(),To);
		return;
	}
}
template<class T>
REBinaryClient<T> *REBinaryTree<T>::getRERoot() const
{
	return RERoot;
}


#endif //COMPILER_REBINARYTREE_H
