//
// Created by 星落_月残 on 2018/5/26.
//

#include "Initialize.h"

//!根据要求生成相应的DFA，并准备用于检验
TokenParser InitTokenParser()
{
	string keywords=string("")+LEFT_BRACE+
					LEFT_BRACE+"i"+CLUSTER+"n"+CLUSTER+"t"+RIGHT_BRACE+
					PARALLEL+LEFT_BRACE+"r"+CLUSTER+"e"+CLUSTER+"a"+CLUSTER+"l"+RIGHT_BRACE+
					PARALLEL+LEFT_BRACE+"i"+CLUSTER+"f"+RIGHT_BRACE+
					PARALLEL+LEFT_BRACE+"t"+CLUSTER+"h"+CLUSTER+"e"+CLUSTER+"n"+RIGHT_BRACE+
					PARALLEL+LEFT_BRACE+"e"+CLUSTER+"l"+CLUSTER+"s"+CLUSTER+"e"+RIGHT_BRACE+
					PARALLEL+LEFT_BRACE+"w"+CLUSTER+"h"+CLUSTER+"i"+CLUSTER+"l"+CLUSTER+"e"+RIGHT_BRACE+
					RIGHT_BRACE+
					CLUSTER+FINAL;

	string identifiers=LEFT_BRACE+RE_LowerCharSet+PARALLEL+RE_UpperCharSet+RIGHT_BRACE+CLUSTER+
					   LEFT_BRACE+RE_LowerCharSet+PARALLEL+RE_UpperCharSet+PARALLEL+RE_NumSet+RIGHT_BRACE+CLOSURE+
					   CLUSTER+FINAL;

	string operators=string("")+LEFT_BRACE+"+"+PARALLEL+"-"+PARALLEL+"/"+PARALLEL+"*"+PARALLEL+"="+PARALLEL+
					 LEFT_BRACE+"="+CLUSTER+"="+RIGHT_BRACE+PARALLEL+
					 LEFT_BRACE+"<"+CLUSTER+"="+RIGHT_BRACE+PARALLEL+
					 LEFT_BRACE+">"+CLUSTER+"="+RIGHT_BRACE+PARALLEL+
					 LEFT_BRACE+"!"+CLUSTER+"="+RIGHT_BRACE+PARALLEL+
					 "<"+PARALLEL+">"+RIGHT_BRACE+
					 CLUSTER+FINAL;

	string delimiters=string("")+LEFT_BRACE+"("+PARALLEL+")"+PARALLEL+
					  "{"+PARALLEL+"}"+PARALLEL+";"+PARALLEL+","+RIGHT_BRACE+
					  CLUSTER+FINAL;

	const string DIGIT_PLUS=LEFT_BRACE+RE_NumSet+CLUSTER+RE_NumSet+CLOSURE+RIGHT_BRACE;

	string intnumber=DIGIT_PLUS+CLUSTER+FINAL;
//
	const string EXPONENT=string("")+LEFT_BRACE+LEFT_BRACE+"E"+PARALLEL+"e"+RIGHT_BRACE+CLUSTER+LEFT_BRACE+"+"+PARALLEL+"-"+
						  PARALLEL+EPISILON+RIGHT_BRACE+CLUSTER+DIGIT_PLUS+RIGHT_BRACE;

	string exponent=EXPONENT+CLUSTER+FINAL;

	const string FRACTION=string("")+LEFT_BRACE+"."+CLUSTER+DIGIT_PLUS+RIGHT_BRACE;

	string fraction=FRACTION+CLUSTER+FINAL;

	string realnumber=string("")+LEFT_BRACE+LEFT_BRACE+DIGIT_PLUS+CLUSTER+EXPONENT+RIGHT_BRACE+PARALLEL+
					  LEFT_BRACE+DIGIT_PLUS+CLUSTER+FRACTION+CLUSTER+LEFT_BRACE+EXPONENT+PARALLEL+EPISILON+RIGHT_BRACE+RIGHT_BRACE+RIGHT_BRACE+
					  CLUSTER+FINAL;

	RegularExpressionParser parser1;
	parser1.AllProcess(keywords);
	RegularExpressionParser parser2;
	parser2.AllProcess(identifiers);
	RegularExpressionParser parser3;
	parser3.AllProcess(operators);
	RegularExpressionParser parser4;
	parser4.AllProcess(delimiters);
	RegularExpressionParser parser5;
	parser5.AllProcess(intnumber);
	RegularExpressionParser parser6;
	parser6.AllProcess(exponent);
	RegularExpressionParser parser7;
	parser7.AllProcess(fraction);
	RegularExpressionParser parser8;
	parser8.AllProcess(realnumber);


	TokenParser tokenParser;
	tokenParser.getDFATypeListR().AddFinal(make_pair("keywords",parser1.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("exponent",parser6.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("identifiers",parser2.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("operators",parser3.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("delimiters",parser4.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("intnumber",parser5.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("fraction",parser7.getFinalDFAR()));
	tokenParser.getDFATypeListR().AddFinal(make_pair("realnumber",parser8.getFinalDFAR()));

	return tokenParser;
}

//! 通过已经生成的DFA将输入的字符转换为LLParser可以接受的格式
//! \param tokenParser 生成的DFA
//! \param input 分拆过的输入
//! \param tokenList 待写入的符号表
//! \return 适应于LLParser的字符串
string GenerateForSyntaxParser(TokenParser &tokenParser,Sequence<string> &input,Sequence<TokenInf> &tokenList)
{
	string result;
	for(long long i=0;i<input.getUsage();i++)
	{
		string lexicalUnit=input[i];
		TokenInf tokenResult=tokenParser.parse(lexicalUnit);

		tokenList.AddFinal(tokenResult);
//		cout<<endl<<lexicalUnit<<"/"<<tokenResult.getTokenType()<<endl;
		if(tokenResult.getTokenType().empty());
		else if(tokenResult.getTokenType()=="intnumber"||tokenResult.getTokenType()=="realnumber")
		{
			if(result.empty())
			{
				result+="NUM";
			}
			else
			{
				result+=string()+CLUSTER+"NUM";
			}
		}
		else if(tokenResult.getTokenType()=="identifiers")
		{
			if(result.empty())
			{
				result+="ID";
			}
			else
			{
				result+=string()+CLUSTER+"ID";
			}
		}
		else if(tokenResult.getTokenType()=="ERROR");
		else
		{
			if(result.empty())
			{
				result+=tokenResult.getTokenName();
			}
			else
			{
				result+=string()+CLUSTER+tokenResult.getTokenName();
			}
		}
	}
	return result;
}

//! 分拆输入
//! \param inputFileName 输入文件名
//! \return 原始文件内容 以及 分拆后的内容
pair<string,Sequence<string>> ReformatInput(const string &inputFileName)
{
	Sequence<char> delimiterList;
	delimiterList.AddFinal(' ');
	delimiterList.AddFinal('\t');
	ifstream fin(inputFileName);
	string orginal;
	string inputLine;
	Sequence<string> result;

	for(;!fin.eof();)
	{
		getline(fin,inputLine);
		orginal+=inputLine;
		Sequence<string> lineResult=splitInput(inputLine,delimiterList);
		result.Merge(lineResult);
	}
	fin.close();
	return make_pair(orginal,result);
}
//! 根据要求生成相应的LLParser
LLParser InitLLParser()
{
	LLParser SyntaxParser;
	string terminal=string()+"if"+PARALLEL+"("+PARALLEL+")"+PARALLEL+"then"+PARALLEL+"else"+PARALLEL+"while"+PARALLEL+"ID"
					+PARALLEL+"int"+PARALLEL+"real"+PARALLEL+"="+PARALLEL+"<"+PARALLEL+">"+PARALLEL+"<="+PARALLEL+">="+PARALLEL+"=="
					+PARALLEL+"+"+PARALLEL+"-"+PARALLEL+"NUM"+PARALLEL+";"+PARALLEL+","+PARALLEL+"{"+PARALLEL+"}"+PARALLEL+"*"+PARALLEL+"/";
	string nonTerminal=string()+"program"+PARALLEL+"compoundstmt"+PARALLEL+"stmt"+PARALLEL+"decl"+PARALLEL+"ifstmt"+PARALLEL+
					   "whilestmt"+PARALLEL+"assgstmt"+PARALLEL+"stmts"+PARALLEL+"boolexpr"+PARALLEL+"arithexpr"+PARALLEL+
					   "type"+PARALLEL+"list"+PARALLEL+"list1"+PARALLEL+"multexpr"+PARALLEL+"arithexprprime"+PARALLEL+
					   "simpleexpr"+PARALLEL+"multexprprime"+PARALLEL+"boolop";

	SyntaxParser.setNonTerminal(nonTerminal);
	SyntaxParser.setTerminal(terminal);

	string production1=string()+"program"+DERIVATION+"compoundstmt";
	string production2=string()+"stmt"+DERIVATION+"decl";
	string production3=string()+"stmt"+DERIVATION+"ifstmt";
	string production4=string()+"stmt"+DERIVATION+"whilestmt";
	string production5=string()+"stmt"+DERIVATION+"assgstmt";
	string production6=string()+"stmt"+DERIVATION+"compoundstmt";
	string production7=string()+"compoundstmt"+DERIVATION+"{"+CLUSTER+"stmts"+CLUSTER+"}";
	string production8=string()+"stmts"+DERIVATION+"stmt"+CLUSTER+"stmts";
	string production9=string()+"stmts"+DERIVATION+EPISILON;
	string production10=string()+"ifstmt"+DERIVATION+"if"+CLUSTER+"("+CLUSTER+"boolexpr"+CLUSTER+")"+CLUSTER+"then"+CLUSTER+"stmt"+CLUSTER+"else"+CLUSTER+"stmt";
	string production11=string()+"whilestmt"+DERIVATION+"while"+CLUSTER+"("+CLUSTER+"boolexpr"+CLUSTER+")"+CLUSTER+"stmt";
	string production12=string()+"assgstmt"+DERIVATION+"ID"+CLUSTER+"="+CLUSTER+"arithexpr"+CLUSTER+";";
	string production13=string()+"decl"+DERIVATION+"type"+CLUSTER+"list"+CLUSTER+";";
	string production14=string()+"type"+DERIVATION+"int";
	string production15=string()+"type"+DERIVATION+"real";
	string production16=string()+"list"+DERIVATION+"ID"+CLUSTER+"list1";
	string production17=string()+"list1"+DERIVATION+","+CLUSTER+"list";
	string production18=string()+"list1"+DERIVATION+EPISILON;
	string production19=string()+"boolexpr"+DERIVATION+"arithexpr"+CLUSTER+"boolop"+CLUSTER+"arithexpr";
	string production20=string()+"boolop"+DERIVATION+"<";
	string production21=string()+"boolop"+DERIVATION+">";
	string production22=string()+"boolop"+DERIVATION+"<=";
	string production23=string()+"boolop"+DERIVATION+">=";
	string production24=string()+"boolop"+DERIVATION+"==";
	string production25=string()+"arithexpr"+DERIVATION+"multexpr"+CLUSTER+"arithexprprime";
	string production26=string()+"arithexprprime"+DERIVATION+"+"+CLUSTER+"multexpr"+CLUSTER+"arithexprprime";
	string production27=string()+"arithexprprime"+DERIVATION+"-"+CLUSTER+"multexpr"+CLUSTER+"arithexprprime";
	string production28=string()+"arithexprprime"+DERIVATION+EPISILON;
	string production29=string()+"multexpr"+DERIVATION+"simpleexpr"+CLUSTER+"multexprprime";
	string production30=string()+"multexprprime"+DERIVATION+"*"+CLUSTER+"simpleexpr"+CLUSTER+"multexprprime";
	string production31=string()+"multexprprime"+DERIVATION+"/"+CLUSTER+"simpleexpr"+CLUSTER+"multexprprime";
	string production32=string()+"multexprprime"+DERIVATION+EPISILON;
	string production33=string()+"simpleexpr"+DERIVATION+"ID";
	string production34=string()+"simpleexpr"+DERIVATION+"NUM";
	string production35=string()+"simpleexpr"+DERIVATION+"("+CLUSTER+"arithexpr"+CLUSTER+")";

	SyntaxParser.AddProduction(production1);
	SyntaxParser.AddProduction(production2);
	SyntaxParser.AddProduction(production3);
	SyntaxParser.AddProduction(production4);
	SyntaxParser.AddProduction(production5);
	SyntaxParser.AddProduction(production6);
	SyntaxParser.AddProduction(production7);
	SyntaxParser.AddProduction(production8);
	SyntaxParser.AddProduction(production9);
	SyntaxParser.AddProduction(production10);
	SyntaxParser.AddProduction(production11);
	SyntaxParser.AddProduction(production12);
	SyntaxParser.AddProduction(production13);
	SyntaxParser.AddProduction(production14);
	SyntaxParser.AddProduction(production15);
	SyntaxParser.AddProduction(production16);
	SyntaxParser.AddProduction(production17);
	SyntaxParser.AddProduction(production18);
	SyntaxParser.AddProduction(production19);
	SyntaxParser.AddProduction(production20);
	SyntaxParser.AddProduction(production21);
	SyntaxParser.AddProduction(production22);
	SyntaxParser.AddProduction(production23);
	SyntaxParser.AddProduction(production24);
	SyntaxParser.AddProduction(production25);
	SyntaxParser.AddProduction(production26);
	SyntaxParser.AddProduction(production27);
	SyntaxParser.AddProduction(production28);
	SyntaxParser.AddProduction(production29);
	SyntaxParser.AddProduction(production30);
	SyntaxParser.AddProduction(production31);
	SyntaxParser.AddProduction(production32);
	SyntaxParser.AddProduction(production33);
	SyntaxParser.AddProduction(production34);
	SyntaxParser.AddProduction(production35);

	SyntaxParser.setEndMark(';');
	SyntaxParser.getCouplingKeywordsR().AddFinal("else");
	SyntaxParser.MakeParsingTable();
	return SyntaxParser;
}

//!用于拆分输入
Sequence<string> splitInput(string &input,Sequence<char> delimiterList)
{
	Sequence<string> result;
	string element;
	for(long long i=0;i<input.size();i++)
	{
		if(!delimiterList.hasData(input[i]))
		{
			element.push_back(input[i]);
		}
		else
		{
			if(!element.empty())
				result.AddFinal(element);
			element.clear();
		}
	}
	if(!element.empty())
		result.AddFinal(element);
	return result;
}




