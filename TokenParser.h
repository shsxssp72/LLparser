//
// Created by 星落_月残 on 2018/5/18.
//

#ifndef COMPILER_TOKENPARSER_H
#define COMPILER_TOKENPARSER_H

#include "MultipurposeSequence.h"
#include "BasicGraph.h"


class TokenInf
{
public:
	TokenInf();
	TokenInf(const std::string &tokenName,const std::string &tokenType,long long int data);
	TokenInf(const TokenInf &another);
	const std::string &getTokenName() const;
	const std::string &getTokenType() const;
	long long int getData() const;

private:
	std::string tokenName;
	std::string tokenType;
	long long data;
};

class TokenParser
{
public:
	TokenParser()=default;
	TokenParser(const TokenParser &another)=default;
	TokenParser &operator=(const TokenParser &another);
	Sequence<std::pair<std::string,Graph<long long int,char>>> &getDFATypeListR();
	TokenInf parse(std::string input);
private:
	bool isMatch(std::string input,Graph<long long,char> &DFA);
	Sequence<std::pair<std::string,Graph<long long,char>>> DFATypeList;//Token类型名，DFA

};


#endif //COMPILER_TOKENPARSER_H
