//
// Created by 星落_月残 on 2018/5/18.
//

#include "TokenParser.h"


TokenInf::TokenInf()
		:tokenName(""),tokenType(""),data(0)
{
}
TokenInf::TokenInf(const std::string &tokenName,const std::string &tokenType,long long int data)
		:tokenName(tokenName),tokenType(tokenType),data(data)
{
}
TokenInf::TokenInf(const TokenInf &another)
		:tokenName(another.tokenName),tokenType(another.tokenType),data(another.data)
{
}
const std::string &TokenInf::getTokenName() const
{
	return tokenName;
}
const std::string &TokenInf::getTokenType() const
{
	return tokenType;
}
long long int TokenInf::getData() const
{
	return data;
}
TokenParser &TokenParser::operator=(const TokenParser &another)
{
	TokenParser parser(another);
	Swap(this->DFATypeList,parser.DFATypeList);
	return *this;
}

//! 判定输入是否满足DFA
//! \param input 输入字符串
//! \param DFA 用于检验的DFA
//! \return 是否通过
bool TokenParser::isMatch(std::string input,Graph<long long,char> &DFA)
{
	//TODO 增加终止状态
	long long currentIndex=0;
	bool flag=false;
	for(int i=0;i<input.size();i++)//循环完整个输入
	{
		for(long long j=0;j<DFA.getSize();j++)
		{
			if(DFA.hasEgdeData(currentIndex,j,input[i]))
			{
				currentIndex=j;
				flag=true;
				break;
			}
		}
		if(flag)
			flag=false;
		else
			return false;
	}
	//此处判断currentIndex是否为终止状态
	return DFA.getSpecialVertexListR().hasData(currentIndex);

}
//! 逐个DFA检查是否接受
//! \param input 输入字符串
//! \return 检查后的Token信息
TokenInf TokenParser::parse(std::string input)
{
	std::string typeName,tokenName;
	for(long long i=0;i<DFATypeList.getSize();i++)
	{
		if(isMatch(input,DFATypeList[i].second))
		{
			typeName=DFATypeList[i].first;
			tokenName=input;
			break;
		}
	}
	if(typeName.empty())
	{
		typeName="ERROR";
		tokenName=input;
	}
	return TokenInf(tokenName,typeName,0);

}
Sequence<std::pair<std::string,Graph<long long int,char>>> &TokenParser::getDFATypeListR()
{
	return DFATypeList;
}
