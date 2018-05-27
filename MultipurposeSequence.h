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
#ifndef BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H
#define BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H

#include <cstdlib>
#include <iostream>


constexpr long long MaxSequenceLength=INTMAX_MAX;
constexpr long long InitialSequenceSize=100;
constexpr long long NewDynamicArrayIncrement=20;
constexpr long long InsertionSortLimit_Sequence=100;

template<class T>
class Sequence
{
public:
	Sequence();
	Sequence(const Sequence<T> &another);
	~Sequence();
	Sequence<T> &operator=(const Sequence<T> &another);
	bool operator==(const Sequence<T> &another);
	bool operator<(Sequence<T> &another);
	bool operator>=(Sequence<T> &another);
	T &operator[](long long subscript);
	bool isCurrentFull() const;
	bool isFull() const;
	bool isEmpty() const;
	bool hasData(const T &toCompare);
	long long getSize() const;//当前空间大小
	long long getUsage() const;//已用大小
	long long getSubscriptFromData(const T &toFind);//顺序查找,假设无重复
	long long getSubscriptFromCondition(bool (*judge)(const T &));//顺序查找
	long long getSubscriptFromCondition(long long startPosition,long long endPosition,bool (*judge)(const T &));
	long long getSubscriptFromCondition(bool (*judge)(const T &,const long long contrast),long long Contrast);
	long long getSubscriptFromCondition(long long startPosition,long long endPosition,bool (*judge)(const T &,const long long),long long Contrast);
	void Clear();
	void MakeUnique(int (*cmp)(const T &a,const T &b));
	void MakeUnique(long long (*hash)(const T &input),long long hashMax);
	void SyncSize();
	void Traverse(void ( *visit)(T &));
	ErrorCode_List AddFirst(const T &toWrite);//慎用,效率极低
	ErrorCode_List AddFinal(const T &toWrite);
	ErrorCode_List Delete(long long position);
	ErrorCode_List DeleteFinal();
	ErrorCode_List Extract(long long position,T &output);//类似ServeAndRetrieve
	ErrorCode_List ExtractFinal(T &output);
	ErrorCode_List Insert(long long position,const T &toWrite);//postion位置之后插入
	ErrorCode_List Modify(long long position,const T &toWrite);
	ErrorCode_List Merge(Sequence<T> &another);
	ErrorCode_List Retrieve(long long position,T &output) const;
	ErrorCode_List RetrieveFinal(T &output) const;
	ErrorCode_List Sort(int (*cmp)(const T &a,const T &b));
	ErrorCode_List SortPartial(long long startPosition,long long endPosition,int (*cmp)(const T &a,const T &b));//end为超尾
	ErrorCode_List SwapData(long long position1,long long position2);
	ErrorCode_List TransferToDesignatedSize(long long SizeToCreate);
	ErrorCode_List Write(long long position,const T &toWrite);
	T *getPositionDataPointer(long long position) const;
	T *getFinalDataPointer() const;
	long long CopyOfSize;

private:
	long long size;
	long long front;
	long long rear;
	long long count;
	T *Primacy;
	ErrorCode_List sort(T *list,long long length,long long startPosition,long long endPosition,int (*cmp)(const T &,const T &));
};

template<class T>
Sequence<T>::Sequence()
{
	size=InitialSequenceSize;
	rear=0;
	front=0;
	count=0;
	Primacy=new T[InitialSequenceSize];
	CopyOfSize=size;
}

template<class T>
Sequence<T>::~Sequence()
{
	delete[] Primacy;
}
template<class T>
Sequence<T>::Sequence(const Sequence<T> &another)
{
	size=another.size;
	rear=another.rear;
	front=another.front;
	count=another.count;
	Primacy=new T[size];
	for(long long i=0;i<size;i++)
		Primacy[i]=another.Primacy[i];

}
template<class T>
Sequence<T> &Sequence<T>::operator=(const Sequence<T> &another)
{
	if(another.Primacy==Primacy)
		return *this;
	else
	{
		//此处使用了swap来利用析构函数
		Sequence<T> tmp(another);
		size=another.size;
		rear=another.rear;
		front=another.front;
		count=another.count;

		Swap(this->Primacy,tmp.Primacy);
		return *this;
	}
}

template<class T>
bool Sequence<T>::operator==(const Sequence<T> &another)
{
	if(getUsage()!=another.getUsage())
		return false;
	for(long long i=0;i<getUsage();i++)
		if(Primacy[i]!=another.Primacy[i])
			return false;
	return true;
}

template<class T>
bool Sequence<T>::operator<(Sequence<T> &another)
{
	for(long long i=0;i<this->getUsage()&&i<another.getUsage();i++)
	{
		if(this->operator[](i)>another.operator[](i))
			return true;
	}
	return this->getUsage()<another.getUsage();
}

template<class T>
bool Sequence<T>::operator>=(Sequence<T> &another)
{
	return !operator<(another);
}

template<class T>
T &Sequence<T>::operator[](const long long subscript)
{
	long long realPosition=subscript+front;
	if(isEmpty())
		return Primacy[front];
	else if(realPosition<front||realPosition>=rear)
		return Primacy[front];
	else
		return Primacy[realPosition];
}

template<class T>
bool Sequence<T>::isEmpty() const
{
	return count==0;
}

template<class T>
bool Sequence<T>::hasData(const T &toCompare)
{
	return getSubscriptFromData(toCompare)!=-1;
}

template<class T>
long long Sequence<T>::getSize() const
{
	return size;
}

template<class T>
long long Sequence<T>::getUsage() const
{
	return count;
}

template<class T>
bool Sequence<T>::isCurrentFull() const
{
	return rear>=size;
}

template<class T>
bool Sequence<T>::isFull() const
{
	if(isCurrentFull())
	{
		if(size>=MaxSequenceLength||size+NewDynamicArrayIncrement>=MaxSequenceLength)
			return true;
		else
		{
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			if(NewArray==nullptr)
				return true;
			else
			{
				delete[] NewArray;
				return false;
			}
		}
	}
	return false;
}

template<class T>
long long Sequence<T>::getSubscriptFromData(const T &toFind)
{
	if(isEmpty())
		return -1;
	else
	{
		TransferToDesignatedSize(getSize());
		for(long long i=front;i<rear;i++)
			if(Primacy[i]==toFind)
				return i;
		return -1;
	}
}


template<class T>
long long Sequence<T>::getSubscriptFromCondition(const long long startPosition,const long long endPosition,bool (*judge)(const T &))
{
	if(isEmpty())
		return -1;
	else if(endPosition<startPosition||endPosition>count||startPosition<0)
		return -1;
	else
	{
		TransferToDesignatedSize(getSize());
		for(long long i=startPosition;i<endPosition;i++)
			if(judge(Primacy[i]))
				return i;
		return -1;
	}
}

template<class T>
long long Sequence<T>::getSubscriptFromCondition(bool (*judge)(const T &))
{
	return getSubscriptFromCondition(front,rear,judge);
}

template<class T>
long long Sequence<T>::getSubscriptFromCondition(const long long startPosition,const long long endPosition,bool (*judge)(const T &,const long long),const long long Contrast)
{
	if(isEmpty())
		return -1;
	else if(endPosition<startPosition||endPosition>count||startPosition<0)
		return -1;
	else
	{
		if(front!=0)
			TransferToDesignatedSize(getSize());
		for(long long i=startPosition;i<endPosition;i++)
			if(judge(Primacy[i],Contrast))
				return i;
		return -1;
	}
}
template<class T>
long long Sequence<T>::getSubscriptFromCondition(bool (*judge)(const T &,const long long),const long long Contrast)
{
	return getSubscriptFromCondition(front,rear,judge,Contrast);
}

template<class T>
void Sequence<T>::Clear()
{
	size=InitialSequenceSize;
	rear=0;
	count=0;
	front=0;
	if(Primacy==nullptr)
		delete[] Primacy;
	Primacy=new T[InitialSequenceSize];
	CopyOfSize=0;
}

template<class T>
void Sequence<T>::MakeUnique(int (*cmp)(const T &,const T &))
{
	Sort(cmp);
	if(count<=1)
		return;
	TransferToDesignatedSize(getSize());
	for(long long i=front;i<rear-1;i++)
	{
		for(;Primacy[i]==Primacy[i+1]&&i<rear-1;)
		{
			Delete(i+1);
		}
	}

}

template<class T>
void Sequence<T>::MakeUnique(long long (*hash)(const T &),long long hashMax)
{
	T ***hashTable=new T **[hashMax+1];
	for(long long i=0;i<hashMax+1;i++)
	{
		hashTable[i]=new T *[count];
		for(long long j=0;j<count;j++)
		{
			hashTable[i][j]=nullptr;
		}
	}
	for(long long i=front;i<rear;)
	{
		long long hashIndex=hash(Primacy[i]);
		long long j=0,flag=1;
		for(;j<count&&hashTable[hashIndex][j]!=nullptr;j++)
		{
			if(hashTable[hashIndex][j]==Primacy[i])
			{
				Delete(i);
				flag=0;
				break;
			}
		}
		if(flag)
		{
			hashTable[hashIndex][j]=&(Primacy[i]);
			i++;
		}
	}

	for(long long i=0;i<hashMax+1;i++)
	{
		delete hashTable[i];
	}
	delete[] hashTable;
}

template<class T>
void Sequence<T>::SyncSize()
{
	CopyOfSize=size;
}

template<class T>
void Sequence<T>::Traverse(void (*visit)(T &))
{
	if(isEmpty())
		return;
	else
	{
		for(long long i=front;i<rear;i++)
		{
			visit(Primacy[i]);
		}
	}
}

template<class T>
ErrorCode_List Sequence<T>::AddFirst(const T &toWrite)
{
	if(front>0)
	{
		Primacy[front-1]=toWrite;
		front--;
		count++;
	}
	else if(size>0&&rear<size)
	{
		for(long long i=rear-1;i>=front;i--)
		{
			Primacy[i+1]=Primacy[i];
		}
		Primacy[front]=toWrite;
		rear++;
		count++;
	}
	else if(size>0&&rear>=size)
	{
		if(isFull())
			return overflow;
		T *NewArray=new T[size+NewDynamicArrayIncrement];
		size=size+NewDynamicArrayIncrement;
		NewArray[0]=toWrite;
		for(long long i=front;i<rear;i++)
		{
			NewArray[i-front+1]=Primacy[i];
		}
		delete[]Primacy;
		Primacy=NewArray;
		front=0;
		rear++;
		count++;
	}
	else if(size==0)
	{
		if(Primacy!=nullptr)
		{
			std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
			std::cerr<<"Data may be lost."<<std::endl;
			delete[]Primacy;
		}
		T *NewArray=new T[InitialSequenceSize];
		size=InitialSequenceSize;
		front=0;
		count=0;
		rear=0;
		Primacy=NewArray;
		Primacy[0]=toWrite;
		rear++;
		count++;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::AddFinal(const T &toWrite)
{
	if(size>0&&rear<size)
	{
		Primacy[rear]=toWrite;
		rear++;
		count++;
	}
	else if(size>0&&rear>=size)
	{
		if(front>0)
		{
			for(long long i=front;i<rear;i++)
			{
				Primacy[i-1]=Primacy[i];
			}
			Primacy[rear-1]=toWrite;
			front--;
			count++;
		}
		else
		{
			if(isFull())
				return overflow;
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			size=size+NewDynamicArrayIncrement;
			for(long long i=front;i<rear;i++)
			{
				NewArray[i-front]=Primacy[i];
			}
			NewArray[rear]=toWrite;
			front=0;
			rear++;
			count++;
			delete[]Primacy;
			Primacy=NewArray;
		}
	}
	else if(size==0)
	{
		if(Primacy!=nullptr)
		{
			std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
			std::cerr<<"Data may be lost."<<std::endl;
			delete[]Primacy;
		}
		T *NewArray=new T[InitialSequenceSize];
		size=InitialSequenceSize;
		front=0;
		count=0;
		rear=0;
		Primacy=NewArray;
		Primacy[0]=toWrite;
		rear++;
		count++;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Delete(long long position)
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		for(long long i=realPosition;i<rear-1;i++)
		{
			Primacy[i]=Primacy[i+1];
		}
		rear--;
		count--;
		SyncSize();
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::DeleteFinal()
{
	if(isEmpty())
		return underflow;
	else
	{
		rear--;
		count--;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Extract(long long position,T &output)
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		output=Primacy[position];
		for(long long i=realPosition;i<rear-1;i++)
		{
			Primacy[i]=Primacy[i+1];
		}
		rear--;
		count--;
		SyncSize();
		return success;
	}
}
template<class T>
ErrorCode_List Sequence<T>::ExtractFinal(T &output)
{
	if(isEmpty())
		return underflow;
	else
	{
		output=Primacy[rear-1];
		rear--;
		count--;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Insert(long long position,const T &toWrite)
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		if(size>0&&front>0)
		{
			for(long long i=front;i<=realPosition;i++)
			{
				Primacy[i-1]=Primacy[i];
			}
			Primacy[realPosition+1]=toWrite;
			front--;
			count++;
		}
		else if(size>0&&rear<size)
		{
			for(long long i=rear-1;i>realPosition;i--)
			{
				Primacy[i+1]=Primacy[i];
			}
			Primacy[realPosition+1]=toWrite;
			rear++;
			count++;
		}
		else if(size>0&&rear>=size)
		{
			if(isFull())
				return overflow;
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			size=size+NewDynamicArrayIncrement;
			for(long long i=front;i<=realPosition;i++)
			{
				NewArray[i-front]=Primacy[i];
			}
			NewArray[position+1]=toWrite;
			for(long long i=realPosition+1;i<rear;i++)
			{
				NewArray[i-front+1]=Primacy[i];
			}
			delete[]Primacy;
			Primacy=NewArray;
			rear=count+1;
			front=0;
			count++;
		}
		else if(size==0)
		{
			if(Primacy!=nullptr)
			{
				std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
				std::cerr<<"Data may be lost."<<std::endl;
				delete[]Primacy;
			}
			T *NewArray=new T[InitialSequenceSize];
			size=InitialSequenceSize;
			front=0;
			count=0;
			rear=0;
			Primacy=NewArray;
			Primacy[0]=toWrite;
			rear++;
			count++;
		}
		SyncSize();
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Modify(long long position,const T &toWrite)
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		Primacy[realPosition]=toWrite;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Merge(Sequence<T> &another)
{
	if(another.Primacy==Primacy)
		return ErrorCode_List::unsuited;
	long long anotherSize=another.getUsage();
	if(this->front!=0)
		this->TransferToDesignatedSize(this->size);
	if(this->size-this->rear<=anotherSize)
		this->TransferToDesignatedSize(getSize()+(anotherSize>NewDynamicArrayIncrement?anotherSize:NewDynamicArrayIncrement));
	for(long long i=0;i<anotherSize;i++)
		this->AddFinal(another[i]);
	return ErrorCode_List::success;
}

template<class T>
ErrorCode_List Sequence<T>::Retrieve(long long position,T &output) const
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		output=Primacy[realPosition];
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::RetrieveFinal(T &output) const
{
	if(isEmpty())
		return underflow;
	else
		output=Primacy[rear-1];
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::sort(T *list,long long length,long long startPosition,long long endPosition,int (*cmp)(const T &,const T &))
{
	if(front!=0)
		TransferToDesignatedSize(getSize());//对齐数据
	long long sortLength=endPosition-startPosition;
	if(startPosition<0||endPosition>length||startPosition>=endPosition)
		return ErrorCode_List::range_error;
	else if(sortLength<=InsertionSortLimit_Sequence)//插入排序
	{
		for(long long i=startPosition+1;i<endPosition;i++)
			if(cmp(list[i-1],list[i])>0)//负数定义为a比b小
			{
				T storage=list[i];
				long long j=0;
				for(j=i-1;j>=startPosition&&cmp(list[j],storage)>0;j--)
					list[j+1]=list[j];
				list[j+1]=storage;
			}
		return success;
	}
	else
	{
		long long halfLength=sortLength/2;
		T *sublist1=new T[halfLength];
		T *sublist2=new T[halfLength+1];
		long long sublength1=halfLength;
		long long sublength2=sortLength-halfLength;

		for(long long i=startPosition;i<startPosition+sublength1;i++)
			sublist1[i-startPosition]=list[i];
		for(long long i=startPosition+sublength1;i<endPosition;i++)
			sublist2[i-sublength1-startPosition]=list[i];

		sort(sublist1,sublength1,0,sublength1,cmp);
		sort(sublist2,sublength2,0,sublength2,cmp);

		for(long long i=0,j=0,k=startPosition;k<endPosition;k++)
		{
			if(i>=sublength1)
			{
				for(;j<sublength2;j++,k++)
					list[k]=sublist2[j];
				break;
			}
			if(j>=sublength2)
			{
				for(;i<sublength1;i++,k++)
					list[k]=sublist1[i];
				break;
			}

			if(cmp(sublist1[i],sublist2[j])<0)
			{
				list[k]=sublist1[i];
				i++;
			}
			else
			{
				list[k]=sublist2[j];
				j++;
			}
		}

		delete[] sublist1;
		delete[] sublist2;
	}
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Sort(int (*cmp)(const T &,const T &))
{
	return sort(Primacy,count,0,count,cmp);
}

template<class T>
ErrorCode_List Sequence<T>::SortPartial(long long startPosition,long long endPosition,int (*cmp)(const T &,const T &))
{
	return sort(Primacy,count,startPosition,endPosition,cmp);
}
template<class T>
ErrorCode_List Sequence<T>::SwapData(long long position1,long long position2)
{
	long long realPosition1=position1+front;
	long long realPosition2=position2+front;
	if(isEmpty())
		return underflow;
	else if(realPosition1<front||realPosition1>=rear||realPosition2<front||realPosition2>=rear)
		return ErrorCode_List::range_error;
	else
	{
		T tmp=Primacy[realPosition1];
		Primacy[realPosition1]=Primacy[realPosition2];
		Primacy[realPosition2]=tmp;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::TransferToDesignatedSize(const long long SizeToCreate)
{
	if(count>SizeToCreate)
		return underflow;
	else if(isFull())
		return overflow;
	else if(SizeToCreate>MaxSequenceLength)
		return overflow;
	else
	{
		T *NewArray=new T[SizeToCreate];
		size=SizeToCreate;
		for(long long i=front;i<rear;i++)
		{
			NewArray[i-front]=Primacy[i];
		}
		delete[] Primacy;
		front=0;
		rear=count;
		Primacy=NewArray;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Write(long long position,const T &toWrite)
{
	long long realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return ErrorCode_List::range_error;
	else
	{
		Primacy[position]=toWrite;
	}
}

template<class T>
T *Sequence<T>::getPositionDataPointer(long long position) const
{
	return Primacy+position;
}

template<class T>
T *Sequence<T>::getFinalDataPointer() const
{
	return &Primacy[rear-1];
}
#endif //BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H
