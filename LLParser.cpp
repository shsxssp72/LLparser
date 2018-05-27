//
// Created by 星落_月残 on 2018/5/20.
//

#include <c++/7.3.0/sstream>
#include <c++/7.3.0/fstream>
#include "LLParser.h"


#define DEBUG_DISPLAY_TERMINAL_LIST 0
#define DEBUG_DISPLAY_NONTERMINAL_LIST 0
#define DEBUG_DISPLAY_PRODUCTION_LIST 0
#define DEBUG_DISPLAY_FIRST_LIST 0
#define DEBUG_DISPLAY_FOLLOW_LIST 0
#define DEBUG_DISPLAY_PARSING_TABLE 0
#define DEBUG_DISPLAY_PARSING_PROCEDURE 0

using namespace std;


LLParser::LLParser()
		:endMark(';')
{

}
void LLParser::setTerminal(std::string inputTerminalString)
{
	string terminal;
	for(int i=0;i<inputTerminalString.size();i++)//可能由多个字符组成
	{
		if(inputTerminalString[i]!=PARALLEL)
			terminal.push_back(inputTerminalString[i]);
		else
		{
			terminalList.AddFinal(terminal);
#if DEBUG_DISPLAY_TERMINAL_LIST==1
			cout<<"DEBUG: Terminal: "<<terminal<<endl;
#endif
			terminal.clear();
			continue;
		}
	}
	if(!terminal.empty())
	{
		terminalList.AddFinal(terminal);
#if DEBUG_DISPLAY_TERMINAL_LIST==1
		cout<<"DEBUG: Terminal: "<<terminal<<endl;
#endif
	}

}

void LLParser::setNonTerminal(std::string inputNonTerminalString)
{
	string nonTerminal;
	for(int i=0;i<inputNonTerminalString.size();i++)
	{
		if(inputNonTerminalString[i]!=PARALLEL)
			nonTerminal.push_back(inputNonTerminalString[i]);
		else
		{
			nonTerminalList.AddFinal(nonTerminal);
#if DEBUG_DISPLAY_NONTERMINAL_LIST==1
			cout<<"DEBUG: NonTerminal: "<<nonTerminal<<endl;
#endif
			nonTerminal.clear();
			continue;
		}
	}
	if(!nonTerminal.empty())
	{
		nonTerminalList.AddFinal(nonTerminal);
#if DEBUG_DISPLAY_NONTERMINAL_LIST==1
		cout<<"DEBUG: NonTerminal: "<<nonTerminal<<endl;
#endif
	}
}
//!标识一句语句结束的符号，如分号
void LLParser::setEndMark(char inEndMark)
{
	endMark=inEndMark;
}
void LLParser::AddProduction(std::string inputProduction)
{
	string left;
	Sequence<string> right;
	bool flag=true;
	int i=0;
	for(;i<inputProduction.size();i++)
	{
		if(inputProduction[i]==DERIVATION)
		{
			i++;
			break;
		}
		left.push_back(inputProduction[i]);


	}
	string subRight;
	for(;i<inputProduction.size();i++)
	{
		if(inputProduction[i]!=CLUSTER)
			subRight.push_back(inputProduction[i]);
		else
		{
			right.AddFinal(subRight);
			subRight.clear();
			continue;
		}
	}
	if(!subRight.empty())
		right.AddFinal(subRight);

	productionList.AddFinal(make_pair(left,right));
#if DEBUG_DISPLAY_PRODUCTION_LIST==1
	cout<<"DEBUG: Production: "<<productionList.getFinalDataPointer()->first<<"=";
	for(long long j=0;j<productionList.getFinalDataPointer()->second.getUsage();j++)
		cout<<productionList.getFinalDataPointer()->second[j];
	cout<<endl;
#endif
}

int stringCmp(const string &a,const string &b)
{
	for(int i=0;i<a.size()&&i<b.size();i++)
	{
		if(a[i]<b[i])
			return -1;
		else if(a[i]>b[i])
			return 1;
	}
	if(a.size()<b.size())
		return -1;
	else
		return 1;
}

Sequence<string> &LLParser::getFirst(std::string inputSymbol)
{
	Sequence<string> firstList;//结果存放
	for(long long i=0;i<productionFirstList.getUsage();i++)
	{
		if(productionFirstList[i].first==inputSymbol)
		{
#if DEBUG_DISPLAY_FIRST_LIST==1
			cout<<"DEBUG: First: "<<inputSymbol<<endl;
			for(long long j=0;j<productionFirstList[j].second.getUsage();j++)
				cout<<"	"<<productionFirstList[i].second[j]<<",";
			cout<<endl;
#endif
			return productionFirstList[i].second;//防止重复添加
		}
	}


	if(terminalList.hasData(inputSymbol)||inputSymbol==string()+EPISILON)//输入为终止符的情况
	{
		firstList.AddFinal(inputSymbol);
		productionFirstList.AddFinal(make_pair(inputSymbol,firstList));
#if DEBUG_DISPLAY_FIRST_LIST==1
		cout<<"DEBUG: First: "<<inputSymbol<<endl;
		for(long long j=0;j<productionFirstList.getFinalDataPointer()->second.getUsage();j++)
			cout<<"	"<<productionFirstList.getFinalDataPointer()->second[j]<<",";
		cout<<endl;
#endif
		return productionFirstList.getFinalDataPointer()->second;
	}
	else//非终止符
	{
		Sequence<Sequence<string>> derivationResult;//从生成式列表中获得由inputSymbol开头的推导结果
		for(long long i=0;i<productionList.getUsage();i++)
		{
			if(productionList[i].first==inputSymbol)
				derivationResult.AddFinal(productionList[i].second);
		}

		bool episilonFlag=true;

		for(long long i=0;i<derivationResult.getUsage();i++)
		{
			episilonFlag=true;
			//先将生成式逐个"语法符号"拆分，逐个分析是否可为空
			for(long long j=0;j<derivationResult[i].getUsage();j++)
			{
				//判断每个语法元素是否分析过，并按规则Merge进入firstList
				Sequence<string> subResult=getFirst(derivationResult[i][j]);//递归
				if(!subResult.hasData(string("")+EPISILON))
				{
					episilonFlag=false;
					firstList.Merge(subResult);
					break;
				}
				else//没有空字符继续分析
				{
					subResult.Delete(subResult.getSubscriptFromData(string("")+EPISILON));
					firstList.Merge(subResult);
				}
			}
			if(episilonFlag)
			{
				firstList.AddFinal(string()+EPISILON);
			}
		}


		firstList.MakeUnique(stringCmp);
		productionFirstList.AddFinal(make_pair(inputSymbol,firstList));
#if DEBUG_DISPLAY_FIRST_LIST==1
		cout<<"DEBUG: First: "<<inputSymbol<<endl;
		for(long long j=0;j<productionFirstList.getFinalDataPointer()->second.getUsage();j++)
			cout<<"	"<<productionFirstList.getFinalDataPointer()->second[j]<<",";
		cout<<endl;
#endif
		return productionFirstList.getFinalDataPointer()->second;
	}
}

void LLParser::MakeFirstList()
{
	for(long long i=0;i<productionList.getUsage();i++)
	{
		getFirst(productionList[i].first);
	};
#if 0
	//DEBUG
	cout<<"DEBUG: From MakeFirstList() Started."<<endl;
	for(long long i=0;i<productionFirstList.getUsage();i++)
	{
		cout<<"DEBUG: First: "<<productionFirstList[i].first<<endl;
		for(long long j=0;j<productionFirstList[i].second.getUsage();j++)
			cout<<" "<<productionFirstList[i].second[j]<<",";
		cout<<endl;
	}
	cout<<"DEBUG: From MakeFirstList() Finished."<<endl;
#endif
}

void LLParser::MakeFollowList()
{
	for(long long i=0;i<nonTerminalList.getUsage();i++)
		getFollow(nonTerminalList[i],string());
#if 0
	//DEBUG
	cout<<"DEBUG: From MakeFollowList() Started."<<endl;
	for(long long i=0;i<productionFollowList.getUsage();i++)
	{
		cout<<"DEBUG: Follow: "<<productionFollowList[i].first<<endl;
		for(long long j=0;j<productionFollowList[i].second.getUsage();j++)
			cout<<" "<<productionFollowList[i].second[j]<<",";
		cout<<endl;
	}
	cout<<"DEBUG: From MakeFollowList() Finished."<<endl;
#endif
}

Sequence<std::string> &LLParser::getFollow(std::string inputSymbol,std::string parentInput)
{
	Sequence<string> followList;//结果存放
	for(long long i=0;i<productionFollowList.getUsage();i++)
	{
		if(productionFollowList[i].first==inputSymbol)
		{
#if DEBUG_DISPLAY_FOLLOW_LIST==1
			cout<<"DEBUG: Follow: "<<inputSymbol<<endl;
			for(long long j=0;j<productionFollowList.getFinalDataPointer()->second.getUsage();j++)
				cout<<"	"<<productionFollowList.getFinalDataPointer()->second[j]<<",";
			cout<<endl;
#endif
			return productionFollowList[i].second;//防止重复添加
		}
	}


	if(inputSymbol==productionList[0].first)//起始符
	{
		followList.AddFinal(string()+TERMINATOR);
	}

	for(long long i=0;i<productionList.getUsage();i++)//扫描所有生成式
	{
		for(long long j=0;j<productionList[i].second.getUsage();j++)
		{
			if(productionList[i].second[j]!=inputSymbol)
				continue;
			else
			{
				bool episilonFlag=true;
				if((j<productionList[i].second.getUsage()-1)
				   &&(productionList[i].second[j+1]!=string()+EPISILON))//如果后面有语法符号
				{
					Sequence<string> tmpResult=getFirst(productionList[i].second[j+1]);
					tmpResult.Delete(tmpResult.getSubscriptFromData(string("")+EPISILON));//清除空字符
					followList.Merge(tmpResult);
					followList.MakeUnique(stringCmp);
				}
				//逐个判定接续的语法符号是否为空
				for(long long k=j+1;k<productionList[i].second.getUsage();k++)
				{
					if(!nullable(productionList[i].second[k]))
						episilonFlag=false;
				}
				//后面为空或空字符
				if(episilonFlag)
				{
					if(productionList[i].first==inputSymbol)
						continue;
					if(productionList[i].first==parentInput)
						continue;
					followList.Merge(getFollow(productionList[i].first,inputSymbol));
					followList.MakeUnique(stringCmp);
				}
			}
		}
	}
	productionFollowList.AddFinal(make_pair(inputSymbol,followList));
#if DEBUG_DISPLAY_FOLLOW_LIST==1
	cout<<"DEBUG: Follow: "<<inputSymbol<<endl;
	for(long long j=0;j<productionFollowList.getFinalDataPointer()->second.getUsage();j++)
		cout<<"	"<<productionFollowList.getFinalDataPointer()->second[j]<<",";
	cout<<endl;
#endif
	return productionFollowList.getFinalDataPointer()->second;


}
Sequence<std::string> &LLParser::getCouplingKeywordsR()
{
	return couplingKeywords;
}
bool LLParser::nullable(const string &nonTerminal)
{
	return getFirst(nonTerminal).hasData(string()+EPISILON);
}
Sequence<long long int> &LLParser::getErrorLocationListR()
{
	return errorLocationList;
}
//!LL构表过程
void LLParser::MakeParsingTable()
{
	parsingTable=Table<string,long long>(nonTerminalList.getUsage(),terminalList.getUsage()+1);//初始化表的大小
	for(long long i=0;i<terminalList.getUsage();i++)
	{
		parsingTable.setColAttr(i,terminalList[i]);
	}
	parsingTable.setColAttr(parsingTable.getCol()-1,string()+TERMINATOR);
	for(long long i=0;i<nonTerminalList.getUsage();i++)
	{
		parsingTable.setRowAttr(i,nonTerminalList[i]);
	}
	for(long long i=0;i<parsingTable.getRow();i++)
	{
		for(long long j=0;j<parsingTable.getCol();j++)
		{
			parsingTable.setDataById(i,j,-1);
		}
	}

	for(long long i=0;i<productionList.getUsage();i++)
	{
		Sequence<string> firstAlpha=getFirst(productionList[i].second[0]);
		for(long long j=0;j<firstAlpha.getUsage();j++)//Assert此处所有内容均为终止符或空字符
		{
			if(firstAlpha[j]!=string()+EPISILON)
			{
				parsingTable.setDataByAttr(productionList[i].first,firstAlpha[j],i);
			}
			else
			{
				Sequence<string> followA=getFollow(productionList[i].first,string());
				for(long long k=0;k<followA.getUsage();k++)//包括Terminator
				{
					parsingTable.setDataByAttr(productionList[i].first,followA[k],i);
				}
			}
		}
	}
#if DEBUG_DISPLAY_PARSING_TABLE==1
	cout<<"DEBUG: ParsingTable: "<<endl;
	for(long long i=0;i<parsingTable.getCol();i++)
		cout<<"  "<<*parsingTable.getColAttr(i);
	cout<<endl;
	for(long long i=0;i<parsingTable.getRow();i++)
	{
		cout<<*parsingTable.getRowAttr(i)<<" ";
		for(long long j=0;j<parsingTable.getCol();j++)
			cout<<parsingTable.getDataByIndex(i,j)<<" ";
		cout<<endl;
	}
#endif
}
//!清空Parser
void LLParser::Clear()
{
	couplingKeywords.Clear();
	terminalList.Clear();
	nonTerminalList.Clear();
	productionList.Clear();
	productionFirstList.Clear();
	productionFollowList.Clear();
	parsingTable.Clear();
	errorLocationList.Clear();
}
Table<string,long long int> &LLParser::getParsingTableR()
{
	return parsingTable;
}

//!\注意 此处语法出错处理使用的算法是，每当有一个语句结束符即分号被接受，记录此时的栈状态
//!		 出错时，将栈回退至上一个分号，先测试当前位置，若不成功则将待分析的位置移至出错行的分号后
//!		 为避免当发生if-else语句else分句的分号缺失错误时，之后所有输入全部报错
//!		 这里的解决方案为指定耦合度高的特征关键字，当准备更新栈的备份时，如果栈内有关键字，则跳过这次更新
//!		 缺陷为当左大括号后第一条语句出错时，之后所有输入全部报错
//! \param inputStmt 经过格式化后的完整的程序输入
//!\return Parse的过程字符串
string LLParser::parse(string &inputStmt)//也许可以用返回值代表是否成功
{
	long long endMarkAcceptCount=0;
	stringstream ss;
	auto rdBackup=cout.rdbuf();
	cout.rdbuf(ss.rdbuf());
	//将string转换成列表
	Sequence<string> toParse;
	string element;
	for(long long i=0;i<inputStmt.size();i++)
	{
		if(inputStmt[i]!=CLUSTER)
		{
			element.push_back(inputStmt[i]);
		}
		else
		{
			toParse.AddFinal(element);
			element.clear();
		}
	}
	if(!element.empty())
	{
		toParse.AddFinal(element);
	}
	toParse.AddFinal(string()+TERMINATOR);//增加结束标记

	Sequence<string> parseStack;
	parseStack.AddFinal(string()+TERMINATOR);
	parseStack.AddFinal(string()+productionList[0].first);//初始化栈为 $S
	Sequence<string> revertStack=parseStack;
	bool isNoError=true;

	for(long long i=0;i<toParse.getUsage();)
	{
		if(*parseStack.getFinalDataPointer()==string()+TERMINATOR&&toParse[i]==string()+TERMINATOR)
		{
			if(isNoError)
			{
				cout<<"Accepted."<<endl;
				cout.rdbuf(rdBackup);
				return ss.str();
			}
			else
			{
				cout<<"Error occurs"<<endl;
				cout.rdbuf(rdBackup);
				return ss.str();
			}
		}

		if(terminalList.hasData(*parseStack.getFinalDataPointer()))//栈顶为终止符
		{
#if DEBUG_DISPLAY_PARSING_PROCEDURE==1
			cout<<endl<<"Stack Top : "<<*parseStack.getFinalDataPointer()<<endl;
			cout<<"Met : "<<toParse[i]<<endl;
#endif
			if(toParse[i]==*parseStack.getFinalDataPointer())
			{
				cout<<"Shifted : "<<toParse[i]<<endl;
				parseStack.DeleteFinal();
				if(toParse[i]==string()+endMark)
				{
					bool flag=true;
					endMarkAcceptCount++;
					for(long long j=0;j<couplingKeywords.getUsage();j++)
					{
						if(parseStack.hasData(couplingKeywords[i]))
							flag=false;
					}
					if(flag)
					{
						revertStack=parseStack;
					}
				}
				i++;
				continue;
			}
			else
			{
				cout<<"Error -No matching production."<<endl;
				isNoError=false;
				//在i下移前用先前的栈测试当前位置
				if(!errorLocationList.hasData(endMarkAcceptCount))
				{
					parseStack=revertStack;
					errorLocationList.AddFinal(endMarkAcceptCount);
					continue;
				}


				for(long long j=i;j<toParse.getUsage();j++)
				{
					if(toParse[j]==string()+endMark)
					{
						if(j<toParse.getUsage()-1)
						{
							endMarkAcceptCount++;
							parseStack=revertStack;
							i=j+1;
							break;
						}
						else
						{
							cout.rdbuf(rdBackup);
							return ss.str();
						}
					}
					if(j==toParse.getUsage()-1)
					{
						cout.rdbuf(rdBackup);
						return ss.str();
					}
				}
				parseStack=revertStack;
				continue;
			}
		}
		else if(nonTerminalList.hasData(*parseStack.getFinalDataPointer()))//栈顶为非终止符
		{
#if DEBUG_DISPLAY_PARSING_PROCEDURE==1
			cout<<endl<<"Stack Top : "<<*parseStack.getFinalDataPointer()<<endl;
			cout<<"Met : "<<toParse[i]<<endl;
#endif
			long long productionIndex=0;//寻找匹配的生成式
			productionIndex=parsingTable.getDataByAttr(*parseStack.getFinalDataPointer(),toParse[i]);
			if(productionIndex<0)
			{
				cout<<"Error -No matching production."<<endl;
				isNoError=false;
				//在i下移前用先前的栈测试当前位置
				if(!errorLocationList.hasData(endMarkAcceptCount))
				{
					parseStack=revertStack;
					errorLocationList.AddFinal(endMarkAcceptCount);
					continue;
				}


				for(long long j=i;j<toParse.getUsage();j++)
				{
					if(toParse[j]==string()+endMark)
					{
						if(j<toParse.getUsage()-1)
						{
							endMarkAcceptCount++;
							parseStack=revertStack;
							i=j+1;
							break;
						}
						else
						{
							cout.rdbuf(rdBackup);
							return ss.str();
						}
					}
					if(j==toParse.getUsage()-1)
					{
						cout.rdbuf(rdBackup);
						return ss.str();
					}
				}
				parseStack=revertStack;
				continue;
			}

			cout<<"Derivation :"<<*parseStack.getFinalDataPointer()<<" -> ";
			for(long long j=0;j<productionList[productionIndex].second.getUsage();j++)
			{
				if(productionList[productionIndex].second[j]!=string()+EPISILON)
					cout<<productionList[productionIndex].second[j]<<" ";
				else
					cout<<"EPISILON";
			}
			cout<<endl;

			//栈顶元素替换
			parseStack.DeleteFinal();
			for(long long j=productionList[productionIndex].second.getUsage()-1;j>=0;j--)
			{
				if(productionList[productionIndex].second[j]!=string()+EPISILON)
					parseStack.AddFinal(productionList[productionIndex].second[j]);
			}
		}
		else//栈中元素不是语法符号
		{
#if DEBUG_DISPLAY_PARSING_PROCEDURE==1
			cout<<endl<<"Stack Top : "<<*parseStack.getFinalDataPointer()<<endl;
			cout<<"Met : "<<toParse[i]<<endl;
#endif
			cout<<"Inner Error -Not a valid symbol."<<endl;
			isNoError=false;
			errorLocationList.AddFinal(endMarkAcceptCount);
			for(long long j=i+1;j<toParse.getUsage();j++)
			{
				if(toParse[j]==string()+endMark)
				{
					if(j<toParse.getUsage()-1)
					{
						endMarkAcceptCount++;
						parseStack=revertStack;
						i=j+1;
						break;
					}
					else
					{
						cout.rdbuf(rdBackup);
						return ss.str();
					}
				}
				if(j==toParse.getUsage()-1)
				{
					cout.rdbuf(rdBackup);
					return ss.str();
				}
			}
			parseStack=revertStack;
			continue;
		}
	}
	cout.rdbuf(rdBackup);
	return ss.str();
}

int indexCmp(const long long &a,const long long &b)
{
	if(a<b)
		return -1;
	else
		return 1;
}
//! 通过errorLocationList找出所有的出错语句
//! \param originalText 原始输入(不需要转换)
//! \return 出错语句集合
Sequence<string> LLParser::getErrorStmts(string originalText)
{
	Sequence<string> result;

	errorLocationList.MakeUnique(indexCmp);

	//此处errorLocationList从小到大排序
	long long loc=0;
	long long endMarkCount=0;
	string element;

	for(long long i=0;i<errorLocationList.getUsage();i++)
	{
		for(;loc<originalText.size();loc++)
		{
			if(originalText[loc]==endMark)
				endMarkCount++;
			if(errorLocationList[i]==endMarkCount)
			{
				for(long long j=loc+1;j<originalText.size();j++)
				{
					if(originalText[j]!=endMark)
					{
						element.push_back(originalText[j]);
					}
					else
					{
						result.AddFinal(element+";");
						element.clear();
						loc=j-1;
						break;
					}
				}
				if(!element.empty())
					result.AddFinal(element);
				break;//使得i跳到下一个查询位置
			}
		}
	}
	return result;
}

