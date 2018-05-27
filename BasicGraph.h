//
// Created by 星落_月残 on 2017/6/6.
//

#ifndef BASICDATASTRUCTUREANDALGORITHMS_COMMONFUNCTION
#define BASICDATASTRUCTUREANDALGORITHMS_COMMONFUNCTION
template<class T>
void Swap(T &a,T&b)
{
	T tmp=a;
	a=b;
	b=tmp;
}
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "MultipurposeSequence.h"
#include "MultipurposeChain.h"

//TODO 修订Graph，酌情增加更多实用的函数，完善LoadConfig


constexpr int DefaultNumberOfVertices=2;

template<class VertexType>
class Vertex
{
public:
	Vertex();
	explicit Vertex(long long inIndex);
	Vertex(long long inIndex,VertexType inData);
	Vertex(const Vertex<VertexType> &another);
	long long getIndex() const;
	VertexType getData() const;
	void setIndex(long long index);
	void setData(VertexType data);
private:
	long long index;
	VertexType data;
};

template<class VertexType>
Vertex<VertexType>::Vertex()
{
	index=0;
}
template<class VertexType>
Vertex<VertexType>::Vertex(const long long inIndex)
		:index(inIndex)
{
}
template<class VertexType>
Vertex<VertexType>::Vertex(const long long inIndex,const VertexType inData)
		:index(inIndex),data(inData)
{
}
template<class VertexType>
Vertex<VertexType>::Vertex(const Vertex<VertexType> &another)
		:index(another.index),data(another.data)
{
}
template<class T>
long long Vertex<T>::getIndex() const
{
	return index;
}
template<class T>
T Vertex<T>::getData() const
{
	return data;
}
template<class VertexType>
void Vertex<VertexType>::setIndex(long long index)
{
	index=index;
}
template<class VertexType>
void Vertex<VertexType>::setData(VertexType data)
{
	data=data;
}

template<class EdgeType>
class Edge
{
public:
	Edge()=default;
	Edge(const Edge &another);
	Edge(const EdgeType &input);
	Edge <EdgeType> &operator=(const Edge &another);
	void Clear();
	ErrorCode_List AddEdge(EdgeType input);
	ErrorCode_List DeleteEdge(long long index);
	ErrorCode_List DeleteEdge(const EdgeType &input);
	long long getMultiplicity() const;
	EdgeType &getEdgeDataFromIndex(long long index);
	EdgeType &getEdgeDataFromData(const EdgeType &input);
	const Sequence<EdgeType> &getConstituent() const;
private:
	void makeInOrder(int (*cmp)(const EdgeType &,const EdgeType &));
	Sequence<EdgeType> constituent;//此处EdgeType可以表示权重，也可以表示某一步操作（编译原理子集构造法）
};

template<class EdgeType>
Edge<EdgeType>::Edge(const Edge &another)
		:constituent(another.constituent)
{
}

template<class EdgeType>
Edge<EdgeType>::Edge(const EdgeType &input)
{
	constituent.AddFinal(input);
}
template<class EdgeType>
Edge<EdgeType> &Edge<EdgeType>::operator=(const Edge &another)
{
	constituent=another.constituent;
	return *this;
}
template<class EdgeType>
void Edge<EdgeType>::Clear()
{
	constituent.Clear();
}
template<class EdgeType>
ErrorCode_List Edge<EdgeType>::AddEdge(const EdgeType input)
{
	ErrorCode_List result=constituent.AddFinal(input);
	return result;
}
template<class EdgeType>
ErrorCode_List Edge<EdgeType>::DeleteEdge(const long long index)
{
	ErrorCode_List result=constituent.Delete(index);
	return result;
}
template<class EdgeType>
ErrorCode_List Edge<EdgeType>::DeleteEdge(const EdgeType &input)
{
	ErrorCode_List result=constituent.Delete(constituent.getSubscriptFromData(input));
	return result;
}
template<class EdgeType>
long long Edge<EdgeType>::getMultiplicity() const
{
	return constituent.getUsage();
}
template<class EdgeType>
EdgeType &Edge<EdgeType>::getEdgeDataFromIndex(long long index)
{
	return constituent[index];
}
template<class EdgeType>
EdgeType &Edge<EdgeType>::getEdgeDataFromData(const EdgeType &input)
{
	return constituent[constituent.getSubscriptFromData(input)];
}
template<class EdgeType>
const Sequence<EdgeType> &Edge<EdgeType>::getConstituent() const
{
	return constituent;
}

template<class EdgeType>
void Edge<EdgeType>::makeInOrder(int (*cmp)(const EdgeType &,const EdgeType &))
{
	constituent.Sort(cmp);
}


template<class VertexType,class EdgeType>
class Graph
{
public:
	Graph();
	explicit Graph(long long inSize);
	Graph(Edge<EdgeType> **matrix,long long int inSize,Vertex<VertexType> **inVerticesList);
	Graph(const Graph &another);
	~Graph();
	Graph <VertexType,EdgeType> &operator=(const Graph &another);
	long long getSize() const;
	Edge<EdgeType> &getEdgeFromPosition(long long From,long long To) const;
	Vertex<VertexType> *&getVertexPointerFromPosition(long long position) const;
	Sequence<VertexType> &getSpecialVertexListR();
	Sequence<EdgeType> &getSpecialEdgeListR();
	long long getMultiplicity(long long From,long long To) const;
	ErrorCode_List setEdgeFromPosition(long long From,long long To,const Edge<EdgeType> &toWrite);//set函数较麻烦,除了symmetrical
	ErrorCode_List setVertexPointerFromPosition(long long position,const Vertex<VertexType> *&toWrite);
	ErrorCode_List addEdgeSymmetrical(long long From,long long To,long long inEdgeData);
	bool isMultiplicity(long long From,long long To,long long toCompare);
	bool hasEgdeData(long long From,long long To,EdgeType toCompare);
	void Clear();
	void LoadConfig();//stdin仅适用于可输入对象
	void LoadConfig(const std::string &FileName);
	void LoadConfig(Edge<EdgeType> **matrix,long long inSize,Vertex<VertexType> **inVerticesList);
	void DepthFirstTraverse(Vertex<VertexType> *Home,void (*visit)(Vertex<VertexType> *));
	void BreadthFirstTraverse(Vertex<VertexType> *Home,void (*visit)(Vertex<VertexType> *));//都是给定入口开始遍历，未必能遍历到
	void DepthFirstSort(Vertex<VertexType> *Home,Sequence<Vertex<VertexType> *> &output);
	void BreadthFirstSort(Sequence<Vertex<VertexType> *> &output);

private:
	long long size;
	Edge<EdgeType> **AdjacencyMatrix;//[DefaultNumberOfVertices][DefaultNumberOfVertices];//存放值
	Vertex<VertexType> **VerticesList;//[DefaultNumberOfVertices];//存放指针，便于遍历
	Sequence<VertexType> specialVertexList;
	Sequence<EdgeType> specialEdgeList;
};

template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph()
{
	size=DefaultNumberOfVertices;
	AdjacencyMatrix=new Edge<EdgeType> *[DefaultNumberOfVertices];
	VerticesList=new Vertex<VertexType> *[DefaultNumberOfVertices];
	for(long long i=0;i<DefaultNumberOfVertices;i++)
	{
		AdjacencyMatrix[i]=new Edge<EdgeType>[DefaultNumberOfVertices];
		VerticesList[i]=new Vertex<VertexType>;
		VerticesList[i]->setIndex(i);
	}
}

template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph(const long long inSize)
{
	size=inSize;
	AdjacencyMatrix=new Edge<EdgeType> *[inSize];
	VerticesList=new Vertex<VertexType> *[inSize];
	for(long long i=0;i<inSize;i++)
	{
		AdjacencyMatrix[i]=new Edge<EdgeType>[inSize];
		VerticesList[i]=new Vertex<VertexType>;
		VerticesList[i]->setIndex(i);
	}
}
template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph(Edge<EdgeType> **matrix,long long int inSize,Vertex<VertexType> **inVerticesList)
{
	size=inSize;
	AdjacencyMatrix=new Edge<EdgeType> *[inSize];
	VerticesList=new Vertex<VertexType> *[inSize];
	for(long long i=0;i<inSize;i++)
	{
		AdjacencyMatrix[i]=new Edge<EdgeType>[inSize];
		for(long long j=0;j<inSize;j++)
		{
			AdjacencyMatrix[i][j]=matrix[i][j];
		}
		VerticesList[i]=new Vertex<VertexType>;
		VerticesList[i]->setIndex(inVerticesList[i]->getIndex());
		VerticesList[i]->setData(inVerticesList[i]->getData());
	}
}
template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph(const Graph &another)
{
	size=another.size;
	AdjacencyMatrix=new Edge<EdgeType> *[size];
	VerticesList=new Vertex<VertexType> *[size];
	for(long long i=0;i<size;i++)
	{
		AdjacencyMatrix[i]=new Edge<EdgeType>[size];
		VerticesList[i]=new Vertex<VertexType>(another.VerticesList[i]->getIndex());
		for(long long j=0;j<size;j++)
		{
			AdjacencyMatrix[i][j]=another.AdjacencyMatrix[i][j];
		}
	}
	specialEdgeList=another.specialEdgeList;
	specialVertexList=another.specialVertexList;
}

template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::~Graph()
{
	Clear();
}
template<class VertexType,class EdgeType>
Graph<VertexType,EdgeType> &Graph<VertexType,EdgeType>::operator=(const Graph &another)
{
	Graph<VertexType,EdgeType> tmp(another);
	Swap(size,tmp.size);
	Swap(AdjacencyMatrix,tmp.AdjacencyMatrix);
	Swap(VerticesList,tmp.VerticesList);
	Swap(specialVertexList,tmp.specialVertexList);
	Swap(specialEdgeList,tmp.specialEdgeList);
	return *this;
}
template<class VertexType,class EdgeType>
long long Graph<VertexType,EdgeType>::getSize() const
{
	return size;
}
template<class VertexType,class EdgeType>
Edge<EdgeType> &Graph<VertexType,EdgeType>::getEdgeFromPosition(const long long From,const long long To) const
{
	if(From>=size||To>=size)
		return AdjacencyMatrix[size-1][size-1];
	else if(From<0||To<0)
		return AdjacencyMatrix[0][0];
	else
		return AdjacencyMatrix[From][To];
}

template<class VertexType,class EdgeType>
Vertex<VertexType> *&Graph<VertexType,EdgeType>::getVertexPointerFromPosition(long long position) const
{
	if(position>=size)
		return VerticesList[size-1];
	else if(position<0)
		return VerticesList[0];
	else
		return VerticesList[position];
}

template<class VertexType,class EdgeType>
Sequence<VertexType> &Graph<VertexType,EdgeType>::getSpecialVertexListR()
{
	return specialVertexList;
}

template<class VertexType,class EdgeType>
Sequence<EdgeType> &Graph<VertexType,EdgeType>::getSpecialEdgeListR()
{
	return specialEdgeList;
}

template<class VertexType,class EdgeType>
long long Graph<VertexType,EdgeType>::getMultiplicity(const long long From,const long long To) const
{
	if(From>=size||To>=size||From<0||To<0)
		return -1;
	else
		return AdjacencyMatrix[From][To].getMultiplicity();
}

template<class VertexType,class EdgeType>
ErrorCode_List Graph<VertexType,EdgeType>::setEdgeFromPosition(const long long From,const long long To,const Edge<EdgeType> &toWrite)
{
	if(From>=size||To>=size||From<0||To<0)
		return ErrorCode_List::range_error;
	else
	{
		AdjacencyMatrix[From][To]=toWrite;
		return success;
	}
}

template<class VertexType,class EdgeType>
ErrorCode_List Graph<VertexType,EdgeType>::setVertexPointerFromPosition(const long long position,const Vertex<VertexType> *&toWrite)
{
	if(position>=size||position<0)
		return ErrorCode_List::range_error;
	else
	{
		VerticesList[position]=const_cast<Vertex<VertexType> *>(toWrite);
		return success;
	}

}
template<class VertexType,class EdgeType>
ErrorCode_List Graph<VertexType,EdgeType>::addEdgeSymmetrical(const long long From,const long long To,const long long inEdgeData)
{
	if(From>=getSize()||To>=getSize()||From<0||To<0)
		return ErrorCode_List::range_error;
	else if(From!=To)
	{
		getEdgeFromPosition(From,To).AddEdge(inEdgeData);
		getEdgeFromPosition(To,From).AddEdge(inEdgeData);
	}
	else
	{
		getEdgeFromPosition(From,To).AddEdge(inEdgeData);
	}
}
template<class VertexType,class EdgeType>
bool Graph<VertexType,EdgeType>::isMultiplicity(const long long From,const long long To,const long long toCompare)
{
	return AdjacencyMatrix[From][To].getMultiplicity()==toCompare;
}
template<class VertexType,class EdgeType>
bool Graph<VertexType,EdgeType>::hasEgdeData(const long long From,const long long To,const EdgeType toCompare)
{
	long long multiplicity=AdjacencyMatrix[From][To].getMultiplicity();
	for(long long i=0;i<multiplicity;i++)
	{
		if(AdjacencyMatrix[From][To].getEdgeDataFromIndex(i)==toCompare)
			return true;
	}
	return false;
}
template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::Clear()
{
	for(long long i=0;i<size;i++)
	{
		delete[] AdjacencyMatrix[i];
		delete VerticesList[i];
	}
	delete[] AdjacencyMatrix;
	delete[] VerticesList;
}
template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::LoadConfig()
{
	for(long long i=0;i<DefaultNumberOfVertices;i++)
	{
		VertexType input;
		std::cin>>input;
		VerticesList[i]->setData(input);
	}

	for(long long i=0;i<DefaultNumberOfVertices;i++)
		for(long long j=0;j<DefaultNumberOfVertices;j++)
		{
			EdgeType input;
			std::cin>>input;
			AdjacencyMatrix[i][j].AddEdge(input);
		}
}

template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::LoadConfig(const std::string &FileName)
{
	std::ifstream fin(FileName);
	if(!fin.is_open())
		std::cerr<<"Input Error-Can not open file "<<FileName<<"."<<std::endl;
	else
	{
		for(long long i=0;i<DefaultNumberOfVertices;i++)
		{
			VertexType input;
			fin>>input;
			VerticesList[i]->setData(input);
		}

		for(long long i=0;i<DefaultNumberOfVertices;i++)
			for(long long j=0;j<DefaultNumberOfVertices;j++)
			{
				EdgeType input;
				fin>>input;
				AdjacencyMatrix[i][j].AddEdge(input);
			}
	}
}
template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::LoadConfig(Edge<EdgeType> **matrix,long long inSize,Vertex<VertexType> **inVerticesList)
{
	size=inSize;
	for(long long i=0;i<inSize;i++)
	{
		AdjacencyMatrix[i]=new Edge<EdgeType>[inSize];
		for(long long j=0;j<inSize;j++)
		{
			AdjacencyMatrix[i][j]=matrix[i][j];
		}
		VerticesList[i]->setIndex(inVerticesList[i]->getIndex());
		VerticesList[i]->setData(inVerticesList[i]->getData());
	}
}
template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::DepthFirstTraverse(Vertex<VertexType> *Home,void (*visit)(Vertex<VertexType> *))
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	//访问起始位置
	long long HomeIndex=Home->getIndex();
	visit(Home);
	isVisited[HomeIndex]=true;
	Sequence<Vertex<VertexType> *> TraverseStack;
	for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].getMultiplicity()>0&&!isVisited[i])
			TraverseStack.AddFinal(VerticesList[i]);

	//开始遍历
	Vertex<VertexType> *current=nullptr;
	for(;!TraverseStack.isEmpty();)
	{
		TraverseStack.ExtractFinal(current);
		if(!isVisited[current->getIndex()])
		{
			long long currentIndex=current->getIndex();
			visit(current);
			isVisited[currentIndex]=true;
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].getMultiplicity()>0&&!isVisited[i])
					TraverseStack.AddFinal(VerticesList[i]);
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			visit(VerticesList[j]);
			isVisited[j]=true;
			TraverseStack.Clear();
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=j&&AdjacencyMatrix[j][i].getMultiplicity()>0&&!isVisited[i])
					TraverseStack.AddFinal(VerticesList[i]);

			Vertex<VertexType> *current2=nullptr;
			for(;!TraverseStack.isEmpty();)
			{
				TraverseStack.ExtractFinal(current2);
				if(!isVisited[current2->getIndex()])
				{
					long long currentIndex=current2->getIndex();
					visit(current2);
					isVisited[currentIndex]=true;
					for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
						if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].getMultiplicity()>0&&!isVisited[i])
							TraverseStack.AddFinal(VerticesList[i]);
				}
			}
		}
	}
}

template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::BreadthFirstTraverse(Vertex<VertexType> *Home,void (*visit)(Vertex<VertexType> *))
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	//访问起始位置
	long long HomeIndex=Home->getIndex();
	visit(Home);
	isVisited[HomeIndex]=true;
	Sequence<Vertex<VertexType> *> TraverseQueue;
	for(long long i=0;i<DefaultNumberOfVertices;i++)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].getMultiplicity()>0)
			TraverseQueue.AddFinal(VerticesList[i]);

	//开始遍历
	Vertex<VertexType> *current=nullptr;
	for(;!TraverseQueue.isEmpty();)
	{
		TraverseQueue.Extract(0,current);
		if(!isVisited[current->getIndex()])
		{
			long long currentIndex=current->getIndex();
			visit(current);
			isVisited[currentIndex]=true;
			for(long long i=0;i<DefaultNumberOfVertices;i++)
				if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].getMultiplicity()>0)
					TraverseQueue.AddFinal(VerticesList[i]);
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			visit(VerticesList[j]);
			isVisited[j]=true;
			TraverseQueue.Clear();
			for(long long i=0;i<DefaultNumberOfVertices;i++)
				if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].getMultiplicity()>0)
					TraverseQueue.AddFinal(VerticesList[i]);

			Vertex<VertexType> *current2=nullptr;
			for(;!TraverseQueue.isEmpty();)
			{
				TraverseQueue.Extract(0,current2);
				if(!isVisited[current2->getIndex()])
				{
					long long currentIndex=current2->getIndex();
					visit(current2);
					isVisited[currentIndex]=true;
					for(long long i=0;i<DefaultNumberOfVertices;i++)
						if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].getMultiplicity()>0)
							TraverseQueue.AddFinal(VerticesList[i]);
				}
			}
		}
	}
}

template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::DepthFirstSort(Vertex<VertexType> *Home,Sequence<Vertex<VertexType> *> &output)
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	Sequence<Vertex<VertexType> *> TraverseStack;

	//起始位置
	long long HomeIndex=Home->getIndex();
	TraverseStack.AddFinal(VerticesList[HomeIndex]);

	for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].getMultiplicity()>0)
			TraverseStack.AddFinal(VerticesList[i]);
	long long flag=0;
	Vertex<VertexType> *current;
	for(;!TraverseStack.isEmpty();)
	{
		for(;;)
		{
			TraverseStack.RetrieveFinal(current);
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=current->getIndex()&&AdjacencyMatrix[current->getIndex()][i].getMultiplicity()>0&&!isVisited[i])
				{
					TraverseStack.AddFinal(VerticesList[i]);
					flag=1;
				}

			if(flag==1)
				flag=0;
			else
				break;
		}

		Vertex<VertexType> *transition;
		TraverseStack.ExtractFinal(transition);
		if(!isVisited[transition->getIndex()])
		{
			output.AddFinal(transition);
			isVisited[transition->getIndex()]=true;
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			HomeIndex=j;
			TraverseStack.AddFinal(VerticesList[HomeIndex]);

			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].getMultiplicity()>0)
					TraverseStack.AddFinal(VerticesList[i]);
			long long flag2=0;
			Vertex<VertexType> *current2;
			for(;!TraverseStack.isEmpty();)
			{
				for(;;)
				{
					TraverseStack.RetrieveFinal(current2);
					for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
						if(i!=current2->getIndex()&&AdjacencyMatrix[current2->getIndex()][i].getMultiplicity()>0&&!isVisited[i])
						{
							TraverseStack.AddFinal(VerticesList[i]);
							flag2=1;
						}

					if(flag2==1)
						flag2=0;
					else
						break;
				}

				Vertex<VertexType> *transition;
				TraverseStack.ExtractFinal(transition);
				if(!isVisited[transition->getIndex()])
				{
					output.AddFinal(transition);
					isVisited[transition->getIndex()]=true;
				}
			}
		}
	}
}

template<class VertexType,class EdgeType>
void Graph<VertexType,EdgeType>::BreadthFirstSort(Sequence<Vertex<VertexType> *> &output)
{
	//复制邻接矩阵
	Edge<EdgeType> CopyOfAdjacencyMatrix[DefaultNumberOfVertices][DefaultNumberOfVertices];
	for(long long i=0;i<DefaultNumberOfVertices;i++)
		for(long long j=0;j<DefaultNumberOfVertices;j++)
			CopyOfAdjacencyMatrix[i][j]=AdjacencyMatrix[i][j];

	Sequence<Vertex<VertexType> *> TraverseQueue;
	bool isVisited[DefaultNumberOfVertices]={0};
	long long flag=0;
	for(;;)
	{
		for(long long i=0;i<DefaultNumberOfVertices;i++)
		{
			for(long long j=0;j<DefaultNumberOfVertices;j++)
				if(CopyOfAdjacencyMatrix[j][i].getMultiplicity()==1)
				{
					flag=1;
					break;
				}
			if(flag==0&&!isVisited[i])
			{
				TraverseQueue.AddFinal(VerticesList[i]);
				isVisited[i]=true;
			}
			else
				flag=0;
		}

		if(TraverseQueue.isEmpty())
			break;//放在这是为了防止当前队列和当前输出等长而提前退出循环

		Vertex<VertexType> *transition;
		TraverseQueue.Extract(0,transition);
		long long currentIndex=transition->getIndex();
		for(long long i=0;i<DefaultNumberOfVertices;i++)
			CopyOfAdjacencyMatrix[currentIndex][i].Clear();//清除后继
		output.AddFinal(transition);
	}

}


#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H
