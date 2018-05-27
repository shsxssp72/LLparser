//
// Created by 星落_月残 on 2018/5/26.
//

#ifndef COMPILER_GUI_INITIALIZE_H
#define COMPILER_GUI_INITIALIZE_H

#include "TokenParser.h"
#include "RegularExpressionParser.h"
#include "LLParser.h"


TokenParser InitTokenParser();
string GenerateForSyntaxParser(TokenParser &tokenParser,Sequence<string> &input,Sequence<TokenInf> &tokenList);
pair<string,Sequence<string>> ReformatInput(const string &inputFileName);
LLParser InitLLParser();
Sequence<string> splitInput(string &input,Sequence<char> delimiterList);
Sequence<string> getErrorStmts(Sequence<long long> errorLocationList,const char &endMark);
#endif //COMPILER_GUI_INITIALIZE_H
