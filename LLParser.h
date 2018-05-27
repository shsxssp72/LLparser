//
// Created by 星落_月残 on 2018/5/20.
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

#ifndef COMPILER_LLPARSER_H
#define COMPILER_LLPARSER_H

#include "MultipurposeSequence.h"
#include "BasicTable.h"


class LLParser
{
public:
	LLParser();
	void Clear();
	Table<std::string,long long int> &getParsingTableR();
	void setTerminal(std::string inputTerminalString);//以PARALLEL隔开,一次性输入
	void setNonTerminal(std::string inputNonTerminalString);//以PARALLEL隔开,一次性输入
	void setEndMark(char inEndMark);
	void AddProduction(std::string inputProduction);
	void MakeFirstList();//不一定需要显式调用，可以按需调用setFirst
	void MakeFollowList();
	Sequence<std::string> &getFirst(std::string inputSymbol);
	Sequence<std::string> &getFollow(std::string inputSymbol,std::string parentInput);
	Sequence<std::string> &getCouplingKeywordsR();
	Sequence<long long int> &getErrorLocationListR();
	void MakeParsingTable();
	std::string parse(std::string &inputStmt);//要求传入参数使用CLUSTER隔开
	Sequence<std::string> getErrorStmts(std::string inputFileName);
private:
	bool nullable(const std::string &nonTerminal);
	char endMark;
	Sequence<std::string> couplingKeywords;//防止耦合度过高不可判定的语句，便于出错处理,但需要自行分析
	Sequence<std::string> terminalList;
	Sequence<std::string> nonTerminalList;
	Sequence<std::pair<std::string,Sequence<std::string>>> productionList;//箭头左边，箭头右边，注，拆分所有的PARALLEL(即列表内的项全为CLUSTER)，另外，第一个为起始符号
	Sequence<std::pair<std::string,Sequence<std::string>>> productionFirstList;//任意符号，终止符列表
	Sequence<std::pair<std::string,Sequence<std::string>>> productionFollowList;//非终止符，终止符列表
	Table<std::string,long long> parsingTable;//（端点类型，边类型）非终止/终止符，生成式在productionList中序号
	Sequence<long long> errorLocationList;
};

/*
 * 初始步骤：
 * 获得语法表达式
 * 计算First和Follow
 * 根据First和Follow建表
 */

#endif //COMPILER_LLPARSER_H
