//
// Created by 星落_月残 on 2018/5/16.
//

#include <tuple>
#include "RegularExpressionParser.h"


#define DEBUG_DISPLAY_STATE 0
#define DEBUG_DISPLAY_CURRENTCHARINDEX 0
#define DEBUG_DISPLAY_FOLLOWPOS 0
#define DEBUG_DISPLAY_STATETRANSFERLIST 0
#define DEBUG_DISPLAY_DFA 0


bool isValidChar(char input)
{
	return (input>='A'&&input<='Z')
		   ||(input>='a'&&input<='z')
		   ||(input>='0'&&input<='9')
		   ||(input==EPISILON)
		   ||(input=='+')
		   ||(input=='-')
		   ||(input=='*')
		   ||(input=='/')
		   ||(input=='=')
		   ||(input=='<')
		   ||(input=='>')
		   ||(input=='!')
		   ||(input=='(')
		   ||(input==')')
		   ||(input=='{')
		   ||(input=='}')
		   ||(input==';')
		   ||(input==',')
		   ||(input=='.');
}

//!BinaryTree方法的副本
//! \return 具有相应索引的节点地址
REBinaryClient<char> *getAddressFromIndex(REBinaryClient<char> *Entrance,long long inIndex)
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
	REBinaryClient<char> *leftResult=getAddressFromIndex(Entrance->getPLeftJunior(),inIndex);
	REBinaryClient<char> *rightResult=getAddressFromIndex(Entrance->getPRightJunior(),inIndex);
	if(leftResult!=nullptr)
		return leftResult;
	else if(rightResult!=nullptr)
		return rightResult;
	else
		return nullptr;

}


Chain<char> &RegularExpressionParser::getPostfixQueue()
{
	return postfixQueue;
}
REBinaryTree<char> &RegularExpressionParser::getParseTree()
{
	return parseTree;
}

//!用于InitializeFromString()
int charcmp(const char &a,const char &b)
{
	if(a<b)
		return -1;
	else
		return 1;
}

//! 根据输入字符串逐个拆分并调用transferToPostfix()
//! \param inputRE
void RegularExpressionParser::InitializeFromString(string inputRE)
{
	for(int i=0;i<inputRE.size();i++)
	{
		//if(inputRE[i]!=PARALLEL&&inputRE[i]!=CLOSURE&&inputRE[i]!=CLUSTER&&inputRE[i]!=LEFT_BRACE&&inputRE[i]!=RIGHT_BRACE&&inputRE[i]!=FINAL)
		if(isValidChar(inputRE[i]))
			validChar.AddFinal(inputRE[i]);
	}
	validChar.MakeUnique(charcmp);
	Chain<char> inputQueue;
	for(int i=0;i<inputRE.size();i++)
	{
		inputQueue.AddFinal(inputRE[i]);
	}
	transferToPostfix(inputQueue);
}

//!为了避免优先级的冲突，利用逆波兰式的无优先级特点
//! \param inputQueue 由调用者InitializeFromString()生成
void RegularExpressionParser::transferToPostfix(Chain<char> inputQueue)
{
	Chain<char> reverseStack;
	for(;!inputQueue.isEmpty();)
	{
		char element='0';
		inputQueue.Extract(0,element);
		if(isValidChar(element)||element==FINAL)
			postfixQueue.AddFinal(element);
		else if(element==CLOSURE)
		{
			postfixQueue.AddFinal(element);//单目运算符存疑
//			reverseStack.AddFinal(element);
		}
		else if(element==CLUSTER)
		{

			if((!reverseStack.isEmpty())&&reverseStack.getTail()->Data==CLOSURE)
			{
				char tmp=0;
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}
			if((!reverseStack.isEmpty())&&reverseStack.getTail()->Data==CLUSTER)
			{
				char tmp=0;
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}

			reverseStack.AddFinal(element);
		}
		else if(element==PARALLEL)
		{
			if((!reverseStack.isEmpty())&&reverseStack.getTail()->Data==CLOSURE)
			{
				char tmp=0;
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}
			if((!reverseStack.isEmpty())&&reverseStack.getTail()->Data==CLUSTER)
			{
				char tmp=0;
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}
			if((!reverseStack.isEmpty())&&reverseStack.getTail()->Data==PARALLEL)
			{
				char tmp=0;
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}

			reverseStack.AddFinal(element);
		}
		else if(element==LEFT_BRACE)
		{
			reverseStack.AddFinal(element);
		}
		else if(element==RIGHT_BRACE)
		{
			char tmp=0;
			for(;reverseStack.getTail()->Data!=LEFT_BRACE;)
			{
				reverseStack.ExtractFinal(tmp);
				postfixQueue.AddFinal(tmp);
			}
			reverseStack.DeleteFinal();
		}
	}
	for(;!reverseStack.isEmpty();)
	{
		char tmp=0;
		reverseStack.ExtractFinal(tmp);
		postfixQueue.AddFinal(tmp);
	}
}

//! 通过遍历二叉树给每个节点赋予索引值，通过PostTraverse()调用
//! \param in 当前访问节点地址
//! \param inRef 用于标识当前节点索引值的可更新变量指针(类型转换)
void setClientIndex(REBinaryClient<char> *in,void *inRef)
{
	if(in==nullptr)
		return;

	long long *realRef=(long long *)inRef;
	if(in->getData()!=EPISILON&&(isValidChar(in->getData())||in->getData()==FINAL))
	{
		in->getIndexR()=*realRef;
		(*realRef)++;
	}
	else
	{
		in->getIndexR()=-1;
	}
}

//!根据之前计算完的逆波兰式List生成二叉树，注意调用顺序
void RegularExpressionParser::MakeParseTree()
{
	Chain<char> postfixList(postfixQueue);
	Chain<REBinaryClient<char> *> parseTreeList;
	char outChar=0;
	for(;!postfixList.isEmpty();)
	{
		postfixList.Extract(0,outChar);
		switch(outChar)
		{
			case PARALLEL:
			case CLUSTER:
			{
				REBinaryClient<char> *subTree1=nullptr,*subTree2=nullptr;
				if(parseTreeList.getSize()<2)
				{
					cerr<<"Invalid input."<<endl;
					return;
				}
				parseTreeList.ExtractFinal(subTree2);
				parseTreeList.ExtractFinal(subTree1);
				REBinaryClient<char> *newSenior=new REBinaryClient<char>(outChar,nullptr,subTree1,subTree2);
				subTree1->getPSeniorR()=newSenior;
				subTree2->getPSeniorR()=newSenior;
				parseTreeList.AddFinal(newSenior);
				break;
			}
			case CLOSURE:
			{
				REBinaryClient<char> *subTree=nullptr;
				if(parseTreeList.getSize()<1)
				{
					cerr<<"Invalid input."<<endl;
					return;
				}
				parseTreeList.ExtractFinal(subTree);
				REBinaryClient<char> *newSenior=new REBinaryClient<char>(outChar,nullptr,subTree,nullptr);
				subTree->getPSeniorR()=newSenior;
				parseTreeList.AddFinal(newSenior);
				break;
			}
			default:
			{
				if(isValidChar(outChar)||outChar==FINAL)
				{
					REBinaryClient<char> *newClient=new REBinaryClient<char>(outChar,nullptr,nullptr,nullptr);
					parseTreeList.AddFinal(newClient);
				}
				else
				{
					cerr<<"Invalid input."<<endl;
					return;
				}
			}
		}
	}
	REBinaryClient<char> *newRoot=nullptr;
	parseTreeList.ExtractFinal(newRoot);
	parseTree=REBinaryTree<char>(newRoot);
	long long count=1;//从1开始编号
	parseTree.PostTraverse(parseTree.getRERoot(),setClientIndex,&count);
}


int pairCmp(const pair<pair<long long,char>,Sequence<long long>> &a,const pair<pair<long long,char>,Sequence<long long>> &b)
{
	long long cA=a.first.first;
	long long cB=b.first.first;
	if(cA>0&&cB>0)
		if(cA<cB)
			return -1;
		else
			return 1;
	else if(cA<0&&cB>0)
		return 1;
	else if(cA>0&&cB<0)
		return -1;
	else
		return 1;
}

//! 通过Traverse方法为二叉树节点设置first，last，nullable
//! \param inClient 当前访问节点
//! \param unused 不使用
void setREProperties_FLN(REBinaryClient<char> *inClient,void *unused)
{
	REBinaryClient<char> *realClient=inClient;
	if(realClient->getIndex()!=-1)//叶子节点
	{
		if(realClient->getData()==EPISILON)
		{
			realClient->getNullableR()=true;
		}
		else if(isValidChar(realClient->getData())||realClient->getData()==FINAL)
		{
			realClient->getNullableR()=false;
			realClient->getFirstposR().AddFinal(realClient->getIndex());
			realClient->getLastposR().AddFinal(realClient->getIndex());

		}
	}
	else if(realClient->getIndex()==-1)//非叶子节点
	{
		REBinaryClient<char> *REPLeftJunior=realClient->getPLeftJunior();
		REBinaryClient<char> *REPRightJunior=realClient->getPRightJunior();
		if(realClient->getData()==PARALLEL)
		{
			realClient->getNullableR()=(REPLeftJunior->getNullableR())||(REPRightJunior->getNullableR());
			realClient->getFirstposR().Merge(REPLeftJunior->getFirstposR());
			realClient->getFirstposR().Merge(REPRightJunior->getFirstposR());
			realClient->getLastposR().Merge(REPLeftJunior->getLastposR());
			realClient->getLastposR().Merge(REPRightJunior->getLastposR());
		}
		else if(realClient->getData()==CLUSTER)
		{
			realClient->getNullableR()=(REPLeftJunior->getNullableR())&&(REPRightJunior->getNullableR());
			if(REPLeftJunior->getNullableR())
			{
				realClient->getFirstposR().Merge(REPLeftJunior->getFirstposR());
				realClient->getFirstposR().Merge(REPRightJunior->getFirstposR());
			}
			else
			{
				realClient->getFirstposR().Merge(REPLeftJunior->getFirstposR());
			}

			if(REPRightJunior->getNullableR())
			{
				realClient->getLastposR().Merge(REPLeftJunior->getLastposR());
				realClient->getLastposR().Merge(REPRightJunior->getLastposR());
			}
			else
			{
				realClient->getLastposR().Merge(REPRightJunior->getLastposR());
			}
		}
		else if(realClient->getData()==CLOSURE)
		{
			realClient->getNullableR()=true;
			realClient->getFirstposR().Merge(REPLeftJunior->getFirstposR());
			realClient->getLastposR().Merge(REPLeftJunior->getLastposR());
		}
	}
}

//! 通过Traverse方法为二叉树节点设置follow
//! \param inClient inClient 当前访问节点
//! \param Root 根节点，便于查找节点
void setREProperties_Follow(REBinaryClient<char> *inClient,void *Root)
{
	REBinaryClient<char> *realClient=inClient;
	REBinaryClient<char> *realRoot=static_cast<REBinaryClient<char> *>(Root);
	if(realClient->getData()==CLUSTER)
	{
		Sequence<long long> rightFirstpos(realClient->getPRightJunior()->getFirstpos());
		Sequence<long long> leftLastpos(realClient->getPLeftJunior()->getLastpos());
		for(int i=0;i<leftLastpos.getUsage();i++)
		{
			getAddressFromIndex(realRoot,leftLastpos[i])->getFollowposR().Merge(rightFirstpos);
		}
	}
	else if(realClient->getData()==CLOSURE)
	{
		for(int i=0;i<realClient->getLastpos().getUsage();i++)
		{
			getAddressFromIndex(realRoot,realClient->getLastposR()[i])->getFollowposR().Merge(realClient->getFirstposR());
		}
	}
}


int indexSortCmp(const long long &a,const long long &b)
{
	if(a<b)
		return -1;
	else
		return 1;
}
//!follow列表排序
void indexSort(REBinaryClient<char> *inClient,void *cmp)
{
	int (*realCmp)(const long long &,const long long &)=(int (*)(const long long &,const long long &))(cmp);
	REBinaryClient<char> *realClient=inClient;
	realClient->getFollowposR().Sort(realCmp);
}
//!将follow逐个记录到List中
void recordFollowpos(REBinaryClient<char> *inClient,void *REParser)
{
	RegularExpressionParser *parser=(RegularExpressionParser *)REParser;
	if(inClient->getIndex()>0)
		parser->getFollowposListR().AddFinal(make_pair(make_pair(inClient->getIndex(),inClient->getData()),inClient->getFollowpos()));
}
//!总的计算follow的过程
void RegularExpressionParser::SetREClientProperties()
{
	parseTree.PostTraverse(parseTree.getRERoot(),setREProperties_FLN,nullptr);
	parseTree.PostTraverse(parseTree.getRERoot(),setREProperties_Follow,parseTree.getRERoot());
	parseTree.PostTraverse(parseTree.getRERoot(),indexSort,(void *)indexSortCmp);
	parseTree.PostTraverse(parseTree.getRERoot(),recordFollowpos,this);
	followposList.Sort(pairCmp);
#if DEBUG_DISPLAY_FOLLOWPOS==1
	cout<<endl<<"DEBUG: Followpos:"<<endl;
	for(int i=0;i<getFollowposListR().getUsage();i++)
	{
		auto out=getFollowposListR()[i].first;
		auto sec=getFollowposListR()[i].second;
		cout<<"	Index: "<<out.first<<"/Char: "<<out.second<<endl;
		for(int j=0;j<sec.getUsage();j++)
			cout<<"	"<<sec[j]<<"; ";
		cout<<endl;
	}
#endif
}

Sequence<pair<pair<long long,char>,Sequence<long long>>> &RegularExpressionParser::getFollowposListR()
{
	return followposList;
}
//!
//! \param inChar 执行变换的类型
//! \param indexList 可供选择的起始位置集合
//! \return 可以执行该变换的起始位置集合
Sequence<long long int> RegularExpressionParser::getFollowposSetRef(char inChar,Sequence<long long int> &indexList)
{
	Sequence<long long> result;
	for(int i=0;i<indexList.getUsage();i++)
	{

		if(followposList[indexList[i]-1].first.second==inChar)
			result.AddFinal(indexList[i]);
	}
	return result;
}
int sequenceCmp(const Sequence<long long> &a,const Sequence<long long> &b)
{
	if(const_cast<Sequence<long long> &>(a).operator<(const_cast<Sequence<long long> &>(b)))
		return -1;
	else
		return 1;

}

//! 生成状态转换List
void RegularExpressionParser::MakeStateTransferList()
{
//	firstposRoot=getAddressFromIndex(parseTree.getRERoot(),1)->getFirstpos();
	REBinaryClient<char> *finalSharpPosition=parseTree.getAddressFromData(parseTree.getRERoot(),FINAL);
	long long finalSharpIndex=(finalSharpPosition==nullptr?-1:finalSharpPosition->getIndex());
	REBinaryClient<char> *firstToAnalyze=parseTree.getRERoot();
	Sequence<long long> firstposRoot=firstToAnalyze->getFirstpos();

	stateList.AddFinal(firstposRoot);
	isFinalState.AddFinal(firstposRoot.hasData(finalSharpIndex));

	for(long long i=0;i<stateList.getUsage();i++)
	{
		//每个状态都有一个stateTransferList
		Sequence<pair<char,long long>> currentTransfer;
		stateTransferList.AddFinal(make_pair(i,currentTransfer));

		for(int j=0;j<validChar.getUsage();j++)
		{
			Sequence<long long> currentCharIndex=move(getFollowposSetRef(validChar[j],stateList[i]));


#if DEBUG_DISPLAY_CURRENTCHARINDEX==1
			cout<<endl;
			cout<<"DEBUG: "<<" CurrentCharIndex: /"<<validChar[j]<<endl;
			for(int l=0;l<currentCharIndex.getUsage();l++)
			{
				cout<<"	"<<currentCharIndex[l]<<";";

			}
			cout<<endl;
#endif
			Sequence<long long> currentState;
			for(int k=0;k<currentCharIndex.getUsage();k++)
			{
				currentState.Merge(followposList[currentCharIndex[k]-1].second);
			}
			currentState.MakeUnique(indexSortCmp);

			long long existSubscript=stateList.getSubscriptFromData(currentState);
#if DEBUG_DISPLAY_STATE==1
			cout<<endl;
			for(int l=0;l<stateList.getUsage();l++)
			{
				cout<<"DEBUG: State: "<<l<<":"<<endl;
				for(int m=0;m<stateList[l].getUsage();m++)
				{
					cout<<"	"<<stateList[l][m]<<";";
				}
				cout<<endl;
			}
#endif
			if(existSubscript==-1)
			{

#if DEBUG_DISPLAY_STATE==1
				cout<<"DEBUG: CurrentState: "<<endl;
				for(int l=0;l<currentState.getUsage();l++)
				{
					cout<<"	"<<currentState[l]<<";";

				}
				cout<<endl;
#endif
				if(!currentState.isEmpty())
				{
					stateList.AddFinal(currentState);
					isFinalState.AddFinal(currentState.hasData(finalSharpIndex));
					long long currentStateIndex=stateList.getUsage()-1;
					stateTransferList.getFinalDataPointer()->second.AddFinal(make_pair(validChar[j],currentStateIndex));
				}

			}
			else
			{
				//不增加新状态，仅添加转移记录
				stateTransferList.getFinalDataPointer()->second.AddFinal(make_pair(validChar[j],existSubscript));//变换类型，变换后的状态号
			}
		}
	}

#if DEBUG_DISPLAY_STATETRANSFERLIST==1
	cout<<endl<<"DISPLAY_STATETRANSFERLIST"<<endl;
	auto list=getStateTransferListR();
	for(int i=0;i<list.getUsage();i++)
	{
		cout<<"State "<<list[i].first<<":"<<endl;
		auto pairSeq=list[i].second;
		for(int j=0;j<pairSeq.getUsage();j++)
		{
			cout<<"	Conversion: "<<pairSeq[j].first;
			cout<<"	To State: "<<pairSeq[j].second<<endl;
		}
	}
#endif
}
Sequence<pair<long long int,Sequence<pair<char,long long int>>>> &RegularExpressionParser::getStateTransferListR()
{
	return stateTransferList;
}
Graph<long long int,char> &RegularExpressionParser::getFinalDFAR()
{
	return finalDFA;
}

//!由状态转换表生成最终的DFA，使用有向图表示
void RegularExpressionParser::MakeFinalDFA()
{
	finalDFA=Graph<long long,char>(stateTransferList.getUsage());
	for(int i=0;i<stateList.getUsage();i++)
	{
		if(isFinalState[i])
			finalDFA.getSpecialVertexListR().AddFinal(i);
		finalDFA.getVertexPointerFromPosition(i)->setIndex(i);
		finalDFA.getVertexPointerFromPosition(i)->setData(i);
	}
	for(int i=0;i<stateTransferList.getUsage();i++)
	{
		long long currentIndex=stateTransferList[i].first;
		Sequence<pair<char,long long>> conversionStateList=stateTransferList[i].second;
		for(int j=0;j<conversionStateList.getUsage();j++)
		{
			finalDFA.getEdgeFromPosition(currentIndex,conversionStateList[j].second).AddEdge(conversionStateList[j].first);
		}
	}
#if DEBUG_DISPLAY_DFA==1
	cout<<endl<<"DISPLAY_DFA"<<endl;
	auto DFA=getFinalDFAR();
	for(int i=0;i<DFA.getSpecialVertexListR().getUsage();i++)
	{
		cout<<"Final State: "<<DFA.getSpecialVertexListR()[i]<<endl;
	}
	for(int i=0;i<DFA.getSize();i++)
	{
		for(int j=0;j<DFA.getSize();j++)
		{
			auto outEdge=DFA.getEdgeFromPosition(i,j);
			for(int k=0;k<outEdge.getMultiplicity();k++)
				cout<<"From: "<<i<<" Conversion: "<<outEdge.getEdgeDataFromIndex(k)<<" To: "<<j<<endl;
		}
	}
#endif
}
//!完整的操作过程
void RegularExpressionParser::AllProcess(string inputRE)
{
	InitializeFromString(inputRE);
	MakeParseTree();
	SetREClientProperties();
	MakeStateTransferList();
	MakeFinalDFA();
}





