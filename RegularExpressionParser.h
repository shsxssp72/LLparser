//
// Created by 星落_月残 on 2018/5/16.
//

#ifndef COMPILER_REGULAREXPRESSIONPARSER_H
#define COMPILER_REGULAREXPRESSIONPARSER_H

#include <string>
#include "MultipurposeChain.h"
#include "REBinaryTree.h"
#include "BasicGraph.h"


using namespace std;

class RegularExpressionParser
{
public:
	RegularExpressionParser()=default;
	RegularExpressionParser(const RegularExpressionParser &another)=delete;
	RegularExpressionParser &operator=(const RegularExpressionParser &another) = delete;
	Chain<char> &getPostfixQueue();
	REBinaryTree<char> &getParseTree();
	Sequence<pair<pair<long long,char>,Sequence<long long>>> &getFollowposListR();
	Sequence<pair<long long int,Sequence<pair<char,long long int>>>> &getStateTransferListR();
	Graph<long long int,char> &getFinalDFAR();
	void InitializeFromString(string inputRE);
	void MakeParseTree();
	void SetREClientProperties();
	void MakeStateTransferList();
	void MakeFinalDFA();
	void AllProcess(string inputRE);
private:
	void transferToPostfix(Chain<char> inputQueue);
	Sequence<long long int> getFollowposSetRef(char inChar,Sequence<long long int> &indexList);
	Chain<char> postfixQueue;
	REBinaryTree<char> parseTree;
	Sequence<char> validChar;
	Sequence<pair<pair<long long,char>,Sequence<long long>>> followposList;//节点序号，对应字符，followpos列表
	Sequence<Sequence<long long>> stateList;
	Sequence<bool> isFinalState;//序号与stateList一一对应
	Sequence<pair<long long,Sequence<pair<char,long long>>>> stateTransferList;//对应stateList里的序号，变换类型，变换后的状态号
	Graph<long long,char> finalDFA;
};

const string RE_LowerCharSet=string()+LEFT_BRACE+"a"+PARALLEL+"b"+PARALLEL+"c"+PARALLEL+"d"+PARALLEL+"e"+PARALLEL+
							 "f"+PARALLEL+"g"+PARALLEL+"h"+PARALLEL+"i"+PARALLEL+"j"+PARALLEL+"k"+PARALLEL+"l"
							 +PARALLEL+"m"+PARALLEL+"n"+PARALLEL+"o"+PARALLEL+"p"+PARALLEL+"q"+PARALLEL+"r"+PARALLEL
							 +"s"+PARALLEL+"t"+PARALLEL+"u"+PARALLEL+"v"+PARALLEL+"w"+PARALLEL+"x"+PARALLEL+"y"
							 +PARALLEL+"z"+RIGHT_BRACE;
const string RE_UpperCharSet=string()+LEFT_BRACE+"A"+PARALLEL+"B"+PARALLEL+"C"+PARALLEL+"D"+PARALLEL+"E"+PARALLEL+
							 "F"+PARALLEL+"G"+PARALLEL+"H"+PARALLEL+"I"+PARALLEL+"J"+PARALLEL+"K"+PARALLEL+"L"
							 +PARALLEL+"M"+PARALLEL+"N"+PARALLEL+"O"+PARALLEL+"P"+PARALLEL+"Q"+PARALLEL+"R"+PARALLEL
							 +"S"+PARALLEL+"T"+PARALLEL+"U"+PARALLEL+"V"+PARALLEL+"W"+PARALLEL+"X"+PARALLEL+"Y"
							 +PARALLEL+"Z"+RIGHT_BRACE;
const string RE_NumSet=string()+LEFT_BRACE+"0"+PARALLEL+"1"+PARALLEL+"2"+PARALLEL+"3"+PARALLEL+"4"+PARALLEL+"5"
					   +PARALLEL+"6"+PARALLEL+"7"+PARALLEL+"8"+PARALLEL+"9"+RIGHT_BRACE;

#endif //COMPILER_REGULAREXPRESSIONPARSER_H
