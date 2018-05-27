//
// Created by 星落_月残 on 2017/4/13.
//

#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence,overwrite,duplicate,new_entry,unsuited
};
#endif
#ifndef BASICDATASTRUCTUREANDALGORITHMS_COMMONFUNCTION
#define BASICDATASTRUCTUREANDALGORITHMS_COMMONFUNCTION
template<class T>
void Swap(T &a,T &b)
{
	T tmp=a;
	a=b;
	b=tmp;
}
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
#define BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H

#include <cstdlib>


constexpr long long MaxChainLength=1000;
constexpr long long InsertionSortLimit_Chain=1;

template<class T>
struct Client
{
public:
	T Data;
	Client<T> *pLast;
	Client<T> *pNext;
	Client<T>();
	Client<T>(const T &toWrite,Client<T> *Last,Client<T> *Next);
};

template<class T>
Client<T>::Client()
{
	pLast=nullptr;
	pNext=nullptr;
}

template<class T>
Client<T>::Client(const T &toWrite,Client<T> *Last,Client<T> *Next)
{
	Data=toWrite;
	pLast=Last;
	pNext=Next;
}

template<class T>
class Chain
{
public:
	Chain();
	Chain(const Chain &original);//拆环
	~Chain();
	Chain<T> &operator=(const Chain &another);
	bool operator==(const Chain &another);
	long long getSize() const;
	bool isFull() const;
	bool isEmpty() const;
	bool isCircle();
	bool isExistCircle();
	bool isLine();
	bool hasData(const T &toCompare);
	Client<T> *getHead() const;
	Client<T> *getArbitary() const;
	Client<T> *getAddressFromPosition(long long position) const;
	Client<T> *getAddressFromData(const T &toFind) const;//顺序查找,假设无重复
	void Clear();
	void MakeUnique(int (*cmp)(const T &a,const T &b));
	void MakeUnique(long long (*hash)(const T &input),long long hashMax);
	void Traverse(void ( *visit)(T &));
	ErrorCode_List AddFirst(const T &toWrite);
	ErrorCode_List AddFinal(const T &toWrite);
	ErrorCode_List Delete(long long position);
	ErrorCode_List DeleteAddress(Client<T> *DeletePointer);
	ErrorCode_List DeleteFinal();
	ErrorCode_List Extract(long long position,T &output);//类似ServeAndRetrieve
	ErrorCode_List ExtractAddress(Client<T> *ExtractPointer,T &output);
	ErrorCode_List ExtractFinal(T &output);
	ErrorCode_List FormCircle();
	ErrorCode_List FormLine();//TODO 可以考虑内部有环的情况
	ErrorCode_List Insert(long long position,const T &toWrite);//postion位置之后插入
	ErrorCode_List Modify(long long position,const T &toWrite);
	ErrorCode_List Merge(const Chain<T> &another);
	ErrorCode_List Retrieve(long long position,T &output) const;
	ErrorCode_List SwapData(long long position1,long long position2);
	ErrorCode_List Sort(int (*cmp)(const T &a,const T &b));
	Client<T> *getTail() const;
	//ErrorCode_List TransformFromBinaryTree(BinaryTree<VertexType> &original);
private:
	long long count;
	Client<T> *pHead;
	Client<T> *pTail;
	Client<T> *ArbitraryPointer;//用于指向常用值,应用于Find
	ErrorCode_List sort(Client<T> *pFirst,Client<T> *pFinal,int (*cmp)(const T &a,const T &b));
};

template<class T>
Chain<T>::Chain()
{
	count=0;
	pHead=nullptr;
	pTail=nullptr;
	ArbitraryPointer=nullptr;
}

template<class T>
Chain<T>::Chain(const Chain &original)
{
	count=original.count;
	if(original.pHead==nullptr)
	{
		pHead=pTail=ArbitraryPointer=nullptr;
	}
	else
	{
		count=original.count;
		pHead=new Client<T>;
		pHead->Data=original.pHead->Data;
		Client<T> *tmpPointerOrigin=original.pHead;
		Client<T> *Final=original.pTail;
		Client<T> *tmpPointerNew=pHead;
		for(;tmpPointerOrigin!=Final;tmpPointerOrigin=tmpPointerOrigin->pNext)
		{
			tmpPointerNew->Data=tmpPointerOrigin->Data;
			tmpPointerNew->pNext=new Client<T>;
			tmpPointerNew->pNext->pLast=tmpPointerNew;
			tmpPointerNew=tmpPointerNew->pNext;
		}
		tmpPointerNew->Data=original.pTail->Data;
		tmpPointerNew->pNext=nullptr;
		pTail=tmpPointerNew;
	}
}

template<class T>
Chain<T>::~Chain()
{
	Clear();
}

template<class T>
Chain<T> &Chain<T>::operator=(const Chain &another)
{
	if(this==&another)
		return *this;
	else if(another.pHead==nullptr)
	{
		Clear();
		pHead=pTail=ArbitraryPointer=nullptr;
		count=0;
		return *this;
	}
	else
	{
//		Clear();
//		count=another.count;
//		pHead=new Client<T>;
//		pHead->Data=another.pHead->Data;
//		Client<T> *tmpPointerOrigin=another.pHead;
//		Client<T> *Final=another.pTail;
//		Client<T> *tmpPointerNew=pHead;
//		for(;tmpPointerOrigin!=Final;tmpPointerOrigin=tmpPointerOrigin->pNext)
//		{
//			tmpPointerNew->Data=tmpPointerOrigin->Data;
//			tmpPointerNew->pNext=new Client<T>;
//			tmpPointerNew->pNext->pLast=tmpPointerNew;
//			tmpPointerNew=tmpPointerNew->pNext;
//		}
//		tmpPointerNew->Data=another.pTail->Data;
//		tmpPointerNew->pNext=nullptr;
//		pTail=tmpPointerNew;
//		return *this;
		Chain<T> toDelete(another);
		Swap(this->pHead,toDelete.pHead);
		Swap(this->pTail,toDelete.pTail);
		Swap(this->ArbitraryPointer,toDelete.ArbitraryPointer);
		return *this;
	}
}

template<class T>
bool Chain<T>::operator==(const Chain &another)
{
	if(getSize()!=another.getSize())
		return false;
	Client<T> *pTraverse1=getHead();
	Client<T> *pTraverse2=another.getHead();
	for(;pTraverse1!=nullptr&&pTraverse2!=nullptr;
		 pTraverse1=pTraverse1->pNext,pTraverse2=pTraverse2->pNext)
	{
		if(pTraverse1->Data!=pTraverse2->Data)
			return false;
	}
	return !(pTraverse1!=nullptr||pTraverse2!=nullptr);
}

template<class T>
long long Chain<T>::getSize() const
{
	return count;
}

template<class T>
bool Chain<T>::isFull() const
{
	Client<T> *tmpPointer=new Client<T>;
	if(tmpPointer==nullptr)
	{
		return true;
	}
	else
	{
		delete tmpPointer;
	}
	return count>=MaxChainLength;
}

template<class T>
bool Chain<T>::isEmpty() const
{
	return count==0;
}

template<class T>
bool Chain<T>::isCircle()
{

	return (pTail->pNext==pHead&&pHead->pLast==pTail);
}

template<class T>
bool Chain<T>::isExistCircle()
{
	Client<T> *slow=pHead,*fast=pHead;
	for(;;)
	{
		if(slow!=nullptr)
			slow=slow->pNext;
		else
			return false;
		if(fast!=nullptr)
		{
			fast=fast->pNext;
			if(fast!=nullptr)
				fast=fast->pNext;
			else
				return false;
		}
		else
			return false;

		if(slow==fast)
			return true;
	}
}

template<class T>
bool Chain<T>::isLine()
{
	return (pTail->pNext==nullptr&&pHead->pLast==nullptr);
}

template<class T>
bool Chain<T>::hasData(const T &toCompare)
{
	return getAddressFromData(toCompare)!=nullptr;
}

template<class T>
Client<T> *Chain<T>::getArbitary() const
{
	return ArbitraryPointer;
}

template<class T>
ErrorCode_List Chain<T>::DeleteFinal()
{
	if(isEmpty())
		return underflow;
	else if(count==1)
	{
		delete pTail;
		pHead=pTail=nullptr;
	}
	else
	{
		Client<T> *tmpPointer=pTail;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pTail=pTail->pLast;
		if(Circle)
			pTail->pNext=pHead;
		else
			pTail->pNext=nullptr;
		delete tmpPointer;
	}
	count--;
	return success;
}

template<class T>
void Chain<T>::Clear()
{
	for(;!isEmpty();)
	{
		DeleteFinal();
	}
}

template<class T>
Client<T> *Chain<T>::getAddressFromPosition(long long position) const
{
	if(isEmpty())
		return nullptr;
	else if(position<0||position>=count)
		return nullptr;
	else
	{
		if(position<count/2)
		{
			long long counter=0,target=position;
			Client<T> *returnPointer=pHead;
			for(;counter<target;counter++)
			{
				returnPointer=returnPointer->pNext;
			}
			return returnPointer;
		}
		else
		{
			long long counter=0,target=count-position-1;
			Client<T> *returnPointer=pTail;
			for(;counter<target;counter++)
			{
				returnPointer=returnPointer->pLast;
			}
			return returnPointer;
		}
	}

}

template<class T>
Client<T> *Chain<T>::getAddressFromData(const T &toFind) const
{
	if(isEmpty())
		return nullptr;
	else
	{
		Client<T> *FindPointer=pHead;
		do
		{
			if(FindPointer->Data==toFind)
				return FindPointer;
			else
				FindPointer=FindPointer->pNext;
		}
		while(FindPointer!=nullptr);
		return nullptr;
	}
}

template<class T>
ErrorCode_List Chain<T>::Extract(long long position,T &output)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *ExtractPointer=getAddressFromPosition(position);
		output=ExtractPointer->Data;
		if(ExtractPointer!=pHead&&ExtractPointer!=pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
		}
		else if(ExtractPointer==pHead)
		{
			if(ExtractPointer->pNext!=nullptr)
				ExtractPointer->pNext->pLast=ExtractPointer->pLast;
			pHead=ExtractPointer->pNext;
		}
		else if(ExtractPointer==pTail)
		{
			if(ExtractPointer->pLast!=nullptr)
				ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			pTail=ExtractPointer->pLast;
		}
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}

}
template<class T>
ErrorCode_List Chain<T>::AddFirst(const T &toWrite)
{
	if(isFull())
		return overflow;
	if(count>0)
	{
		Client<T> *NewClient=new Client<T>;
		NewClient->Data=toWrite;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pHead->pLast=NewClient;
		NewClient->pNext=pHead;
		pHead=pHead->pLast;
		if(Circle)
		{
			pHead->pLast=pTail;
			pTail->pNext=pHead;
		}
		else
		{
			pHead->pLast=nullptr;
		}
		count++;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::AddFinal(const T &toWrite)
{
	if(isFull())
		return overflow;
	if(count>0)
	{
		Client<T> *NewClient=new Client<T>;
		NewClient->Data=toWrite;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pTail->pNext=NewClient;
		NewClient->pLast=pTail;
		pTail=pTail->pNext;
		if(Circle)
		{
			pTail->pNext=pHead;
			pHead->pLast=pTail;
		}
		else
			pTail->pNext=nullptr;
	}
	else
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=nullptr;
		pHead->pNext=nullptr;
		pTail=pHead;
	}
	count++;
	return success;
}

template<class T>
ErrorCode_List Chain<T>::Delete(long long position)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *DeletePointer=getAddressFromPosition(position);
		if(DeletePointer!=pHead&&DeletePointer!=pTail)
		{
			DeletePointer->pLast->pNext=DeletePointer->pNext;
			DeletePointer->pNext->pLast=DeletePointer->pLast;
		}
		else if(DeletePointer==pHead&&DeletePointer!=pTail)
		{
			DeletePointer->pNext->pLast=DeletePointer->pLast;
			pHead=DeletePointer->pNext;
		}
		else if(DeletePointer==pTail&&DeletePointer!=pHead)
		{
			DeletePointer->pLast->pNext=DeletePointer->pNext;
			pTail=DeletePointer->pLast;
		}
		else
		{
			pHead=pTail=nullptr;
		}
		delete DeletePointer;
		DeletePointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::DeleteAddress(Client<T> *DeletePointer)
{
	if(DeletePointer==nullptr||isEmpty())
		return ErrorCode_List::range_error;
	else if(DeletePointer!=pHead&&DeletePointer!=pTail)
	{
		DeletePointer->pLast->pNext=DeletePointer->pNext;
		DeletePointer->pNext->pLast=DeletePointer->pLast;
	}
	else if(DeletePointer==pHead)
	{
		if(pHead->pNext==nullptr)//只有一个元素
		{
			delete DeletePointer;
			count=0;
			pHead=pTail=nullptr;
			return success;
		}
		DeletePointer->pNext->pLast=DeletePointer->pLast;
		pHead=DeletePointer->pNext;
	}
	else if(DeletePointer==pTail)
	{
		if(pTail->pLast==nullptr)//只有一个元素
		{
			delete DeletePointer;
			count=0;
			pHead=pTail=nullptr;
			return success;
		}
		DeletePointer->pLast->pNext=DeletePointer->pNext;
		pTail=DeletePointer->pLast;
	}
	delete DeletePointer;
	DeletePointer=nullptr;
	count--;
	return success;
}

template<class T>
ErrorCode_List Chain<T>::Insert(long long position,const T &toWrite)
{
	if(isFull())
		return overflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else if(count==0)
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=nullptr;
		pHead->pNext=nullptr;
		pTail=pHead;
	}
	else
	{
		Client<T> *InsertPointer=getAddressFromPosition(position);
		if(InsertPointer!=pTail)
		{
			Client<T> *Next=InsertPointer->pNext;
			Client<T> *NewClient=new Client<T>;
			NewClient->Data=toWrite;
			InsertPointer->pNext=NewClient;
			NewClient->pNext=Next;
			Next->pLast=NewClient;
			NewClient->pLast=InsertPointer;
		}
		else
		{
			Client<T> *NewClient=new Client<T>;
			NewClient->Data=toWrite;
			bool Circle=false;
			if(isCircle())
				Circle=true;
			pTail->pNext=NewClient;
			NewClient->pLast=pTail;
			pTail=pTail->pNext;
			if(Circle)
			{
				pTail->pNext=pHead;
				pHead->pLast=pTail;
			}
			else
			{
				pTail->pNext=nullptr;
			}
		}
		count++;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Modify(long long position,const T &toWrite)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *ModifyPointer=getAddressFromPosition(position);
		ModifyPointer->Data=toWrite;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Merge(const Chain<T> &another)
{
	if(&another==this)
		return ErrorCode_List::unsuited;
	Chain<T> tmp(another);
	this->FormLine();
	tmp.FormLine();
	count+=tmp.count;
	this->pTail->pNext=tmp.pHead;
	tmp.pHead->pLast=this->pTail;
	this->pTail=tmp.pTail;
	tmp.pHead=nullptr;
	tmp.pTail=nullptr;
	tmp.count=0;

}

template<class T>
ErrorCode_List Chain<T>::Retrieve(long long position,T &output) const
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *RetrievePointer=getAddressFromPosition(position);
		output=RetrievePointer->Data;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::SwapData(long long position1,long long position2)
{
	if(isEmpty())
		return underflow;
	else if(position1<0||position1>=count||position2<0||position2>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *SwapPointer1=getAddressFromPosition(position1);
		Client<T> *SwapPointer2=getAddressFromPosition(position2);
		T tmp;
		tmp=SwapPointer1->Data;
		SwapPointer1->Data=SwapPointer2->Data;
		SwapPointer2->Data=tmp;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Sort(int (*cmp)(const T &,const T &))
{
	return sort(pHead,pTail,cmp);
}

template<class T>
ErrorCode_List Chain<T>::sort(Client<T> *pFirst,Client<T> *pFinal,int (*cmp)(const T &,const T &))
{
	if(isCircle())
		FormLine();
	Client<T> *TracePointer=pFirst;
	Client<T> *MiddlePointer=pFirst;
	long long halfCount=0;
	for(;TracePointer!=pFinal&&TracePointer!=nullptr;)
	{
		TracePointer=TracePointer->pNext;
		MiddlePointer=MiddlePointer->pNext;
		TracePointer=TracePointer->pNext;
		halfCount++;
	}
	if(halfCount<=(InsertionSortLimit_Chain/2))//插入排序
		for(Client<T> *TraversePointer=pFirst->pNext;TraversePointer!=nullptr;TraversePointer=TraversePointer->pNext)
			if(cmp(TraversePointer->pLast->Data,TraversePointer->Data)>0)
			{
				T storage=TraversePointer->Data;
				Client<T> *WaitPointer=TraversePointer->pLast;
				for(;WaitPointer!=nullptr&&cmp(WaitPointer->Data,storage)>0;WaitPointer=WaitPointer->pLast)
					WaitPointer->pNext->Data=WaitPointer->Data;
				if(WaitPointer==nullptr)
					pFirst->Data=storage;
				else
					WaitPointer->pNext->Data=storage;
			}
			else;
	else//归并排序
	{
		Client<T> *MidLastPointer=MiddlePointer->pLast;
		MidLastPointer->pNext=nullptr;
		MiddlePointer->pLast=nullptr;
		sort(pFirst,MidLastPointer,cmp);
		sort(MiddlePointer,pFinal,cmp);
		Client<T> *ResultPointer=nullptr,*TrackerPointer1=pFirst,*TrackerPointer2=MiddlePointer;

		if(cmp(TrackerPointer1->Data,TrackerPointer2->Data)<0)
		{
			ResultPointer=TrackerPointer1;
			TrackerPointer1=TrackerPointer1->pNext;
			ResultPointer->pLast=nullptr;
			ResultPointer->pNext=nullptr;
		}
		else
		{
			ResultPointer=TrackerPointer2;
			TrackerPointer2=TrackerPointer2->pNext;
			ResultPointer->pLast=nullptr;
			ResultPointer->pNext=nullptr;
		}
		for(;;)
		{
			if(TrackerPointer1==nullptr)
			{
				ResultPointer->pNext=TrackerPointer2;
				TrackerPointer2->pLast=ResultPointer;
				break;
			}
			if(TrackerPointer2==nullptr)
			{
				ResultPointer->pNext=TrackerPointer1;
				TrackerPointer1->pLast=ResultPointer;
				break;
			}
			if(cmp(TrackerPointer1->Data,TrackerPointer2->Data)<0)
			{
				ResultPointer->pNext=TrackerPointer1;
				TrackerPointer1->pLast=ResultPointer;
				TrackerPointer1=TrackerPointer1->pNext;
				ResultPointer=ResultPointer->pNext;
				ResultPointer->pNext=nullptr;
			}
			else
			{
				ResultPointer->pNext=TrackerPointer2;
				TrackerPointer2->pLast=ResultPointer;
				TrackerPointer2=TrackerPointer2->pNext;
				ResultPointer=ResultPointer->pNext;
				ResultPointer->pNext=nullptr;
			}
		}
	}
	return success;
}

template<class T>
ErrorCode_List Chain<T>::ExtractAddress(Client<T> *ExtractPointer,T &output)
{
	if(ExtractPointer==nullptr||isEmpty())
		return ErrorCode_List::underflow;
	else
	{
		output=ExtractPointer->Data;
		if(ExtractPointer!=pHead&&ExtractPointer!=pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
		}
		else if(ExtractPointer==pHead)
		{
			if(pHead->pNext==nullptr)//只有一个元素
			{
				delete ExtractPointer;
				count=0;
				pHead=pTail=nullptr;
				return success;
			}
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
			pHead=ExtractPointer->pNext;
		}
		else if(ExtractPointer==pTail)
		{
			if(pTail->pLast==nullptr)//只有一个元素
			{
				delete ExtractPointer;
				count=0;
				pHead=pTail=nullptr;
				return success;
			}
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			pTail=ExtractPointer->pLast;
		}
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::ExtractFinal(T &output)
{
	if(isEmpty())
		return underflow;
	else
	{
		Client<T> *ExtractPointer=pTail;
		pTail=pTail->pLast;
		output=ExtractPointer->Data;
		if(ExtractPointer->pLast!=nullptr)
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
		else//此时删除了pHead
			pHead=nullptr;
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::FormCircle()
{
	if(isEmpty())
		return underflow;
	else if(isCircle())
		return success;
	else
	{
		pTail->pNext=pHead;
		pHead->pLast=pTail;
		isCircle();
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::FormLine()
{
	if(isEmpty())
		return underflow;
	else
	{
		if(count==1)
		{
			pHead->pLast=nullptr;
			pHead->pNext=nullptr;
			pTail=pHead;
		}
		else
		{
			pHead->pLast=nullptr;
			pTail->pNext=nullptr;
		}
		isLine();
		return success;
	}
}

template<class T>
void Chain<T>::MakeUnique(int (*cmp)(const T &,const T &))
{
	FormLine();
	Sort(cmp);
	if(count<=1)
		return;
	for(Client<T> *TraversePointer=pHead,*AheadPointer=pHead->pNext;
		AheadPointer!=nullptr;
		TraversePointer=TraversePointer->pNext,AheadPointer=AheadPointer->pNext)
	{
		for(;AheadPointer!=nullptr&&TraversePointer->Data==AheadPointer->Data;)
		{
			Client<T> *pDel=AheadPointer;
			AheadPointer=AheadPointer->pNext;
			DeleteAddress(pDel);
		}
	}
}

template<class T>
void Chain<T>::MakeUnique(long long (*hash)(const T &),long long hashMax)
{
	FormLine();
	//Client<T> *hashTable[hashMax+1][count];
	Client<T> ***hashTable=new Client<T> **[hashMax+1];
	for(long long i=0;i<hashMax+1;i++)
	{
		hashTable[i]=new Client<T> *[count];
		for(long long j=0;j<count;j++)
		{
			hashTable[i][j]=nullptr;
		}
	}

	for(Client<T> *TraversePointer=pHead;TraversePointer!=nullptr;)
	{
		long long hashIndex=hash(TraversePointer->Data);
		long long i=0,flag=1;
		for(;i<count&&hashTable[hashIndex][i]!=nullptr;i++)
		{
			if(hashTable[hashIndex][i]->Data==TraversePointer->Data)
			{
				Client<T> *pDel=TraversePointer;
				TraversePointer=TraversePointer->pNext;
				DeleteAddress(pDel);
				flag=0;
				break;
			}
		}
		if(flag)
		{
			hashTable[hashIndex][i]=TraversePointer;
			TraversePointer=TraversePointer->pNext;
		}
	}

	for(long long i=0;i<hashMax+1;i++)
	{
		delete hashTable[i];
	}
	delete[] hashTable;
}

template<class T>
void Chain<T>::Traverse(void ( *visit)(T &))
{
	if(isEmpty())
		return;
	Client<T> *TraversePointer=pHead;
	if(isCircle())
	{
		do
		{
			visit(TraversePointer->Data);
			TraversePointer=TraversePointer->pNext;
		}
		while(TraversePointer!=pHead);
	}
	else
	{
		do
		{
			visit(TraversePointer->Data);
			TraversePointer=TraversePointer->pNext;
		}
		while(TraversePointer!=nullptr);
	}
}
template<class T>
Client<T> *Chain<T>::getHead() const
{
	return pHead;
}

template<class T>
Client<T> *Chain<T>::getTail() const
{
	return pTail;
}
#endif //BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
