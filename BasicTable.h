//
// Created by 星落_月残 on 2018/3/28.
//
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

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICTABLE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICTABLE_H

#include <algorithm>

constexpr int DefaultRow=1;
constexpr int DefaultCol=1;

template<class IndexType,class ContentType>
class Table
{
public:
	Table();
	Table(long long row,long long col);
	Table(const Table &another);
	virtual ~Table();
	Table<IndexType,ContentType> &operator=(const Table &another);
	void Clear();
	long long getRow() const;
	long long getCol() const;
	ContentType &getDataByIndex(long long row,long long col);
	ContentType &getDataByAttr(const IndexType &row,const IndexType &col);//危险，可能返回错误值
	void setDataByAttr(const IndexType &row,const IndexType &col,ContentType input);
	void setDataById(long long rowIndex,long long colIndex,ContentType input);
	void setRowAttr(long long index,IndexType input);
	void setColAttr(long long index,IndexType input);
	IndexType * getRowAttr(long long index);
	IndexType * getColAttr(long long index);
protected:
	long long rowSize;
	long long colSize;
	IndexType *RowAttr;
	IndexType *ColAttr;
	ContentType **Primacy;
};

template<class IndexType,class ContentType>
Table<IndexType,ContentType>::Table()
{
	rowSize=DefaultRow;
	colSize=DefaultCol;
	RowAttr=new IndexType[DefaultRow];
	ColAttr=new IndexType[DefaultCol];
	Primacy=new ContentType *[DefaultRow];
	for(long long i=0;i<DefaultRow;i++)
		Primacy[i]=new ContentType[DefaultCol];
}
template<class IndexType,class ContentType>
Table<IndexType,ContentType>::Table(long long row,long long col)
		:rowSize(row),colSize(col)
{
	RowAttr=new IndexType[row];
	ColAttr=new IndexType[col];
	Primacy=new ContentType *[row];
	for(long long i=0;i<row;i++)
		Primacy[i]=new ContentType[col];
}
template<class IndexType,class ContentType>
Table<IndexType,ContentType>::Table(const Table &another)
		:rowSize(another.rowSize),colSize(another.colSize)
{
	RowAttr=new IndexType[another.rowSize];
	for(long long i=0;i<rowSize;i++)
		RowAttr[i]=another.RowAttr[i];
	ColAttr=new IndexType[another.colSize];
	for(long long i=0;i<colSize;i++)
		ColAttr[i]=another.ColAttr[i];
	Primacy=new ContentType *[rowSize];
	for(long long i=0;i<rowSize;i++)
	{
		Primacy[i]=new ContentType[colSize];
		for(long long j=0;j<colSize;j++)
			Primacy[i][j]=another.Primacy[i][j];
	}
}
template<class IndexType,class ContentType>
Table<IndexType,ContentType>::~Table()
{
	Clear();
}
template<class IndexType,class ContentType>
Table<IndexType,ContentType> &Table<IndexType,ContentType>::operator=(const Table &another)
{
	Table<IndexType,ContentType> tmp(another);
	Swap(this->RowAttr,tmp.RowAttr);
	Swap(this->ColAttr,tmp.ColAttr);
	Swap(this->Primacy,tmp.Primacy);
	Swap(this->colSize,tmp.colSize);
	Swap(this->rowSize,tmp.rowSize);
	return *this;

}
template<class IndexType,class ContentType>
void Table<IndexType,ContentType>::Clear()
{
	delete[] RowAttr;
	delete[] ColAttr;
	for(long long i=0;i<rowSize;i++)
		if(Primacy[i]!=nullptr)
			delete Primacy[i];
	delete[] Primacy;
	colSize=rowSize=0;
}
template<class IndexType,class ContentType>
long long Table<IndexType,ContentType>::getRow() const
{
	return rowSize;
}
template<class IndexType,class ContentType>
long long Table<IndexType,ContentType>::getCol() const
{
	return colSize;
}
template<class IndexType,class ContentType>
ContentType &Table<IndexType,ContentType>::getDataByIndex(long long row,long long col)
{
	if(row>=rowSize||col>=colSize)
		return Primacy[rowSize-1][colSize-1];
	return Primacy[row][col];
}
template<class IndexType,class ContentType>
ContentType &Table<IndexType,ContentType>::getDataByAttr(const IndexType &row,const IndexType &col)
{
	long long rowIndex=-1,colIndex=-1;
	for(long long i=0;i<rowSize;i++)
	{
		if(RowAttr[i]==row)
		{
			rowIndex=i;
			break;
		}
	}
	for(long long i=0;i<colSize;i++)
	{
		if(ColAttr[i]==col)
		{
			colIndex=i;
			break;
		}
	}
	if(rowIndex<0||colIndex<0)
		return Primacy[rowSize-1][colSize-1];
	return Primacy[rowIndex][colIndex];
}
template<class IndexType,class ContentType>
void Table<IndexType,ContentType>::setDataByAttr(const IndexType &row,const IndexType &col,ContentType input)
{
	long long rowIndex=-1,colIndex=-1;
	for(long long i=0;i<rowSize;i++)
	{
		if(RowAttr[i]==row)
		{
			rowIndex=i;
			break;
		}
	}
	for(long long i=0;i<colSize;i++)
	{
		if(ColAttr[i]==col)
		{
			colIndex=i;
			break;
		}
	}
	if(rowIndex<0||colIndex<0)
		return;
	Primacy[rowIndex][colIndex]=std::move(input);
}
template<class IndexType,class ContentType>
void Table<IndexType,ContentType>::setDataById(long long rowIndex,long long colIndex,ContentType input)
{
	if(rowIndex<0||colIndex<0)
		return;
	Primacy[rowIndex][colIndex]=std::move(input);
}
template<class IndexType,class ContentType>
void Table<IndexType,ContentType>::setRowAttr(long long index,IndexType input)
{
	if(index>=rowSize)
		return;
	RowAttr[index]=input;
}
template<class IndexType,class ContentType>
void Table<IndexType,ContentType>::setColAttr(long long index,IndexType input)
{
	if(index>=colSize)
		return;
	ColAttr[index]=input;
}
template<class IndexType,class ContentType>
IndexType * Table<IndexType,ContentType>::getRowAttr(long long index)
{
	if(index>=rowSize)
		return nullptr;
	return &RowAttr[index];
}
template<class IndexType,class ContentType>
IndexType * Table<IndexType,ContentType>::getColAttr(long long index)
{
	if(index>=colSize)
		return nullptr;
	return &ColAttr[index];
}

#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICTABLE_H
