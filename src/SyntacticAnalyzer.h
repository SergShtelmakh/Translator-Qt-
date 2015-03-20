#ifndef SYNTACTICANALYZER_H
#define SYNTACTICANALYZER_H

#include <QHash>
#include "SyntacticSymbol.h"
#include "Token.h"
#include "Production.h"

class SyntacticAnalyzer
{

public:

    SyntacticAnalyzer();
    ~SyntacticAnalyzer();

    void addProductRule(SyntacticSymbol firstSymbol, QList <SyntacticSymbol> productionList);
    void analyze(QList<Token> tokenList);
    SyntacticSymbol getNextSymbolToParse();

private:

    Production findCongruentRule(SyntacticSymbol firstSymbol, Token firstProductSymbol);
    bool isFinalRuleExist(SyntacticSymbol firstSymbol);

    QMultiHash < SyntacticSymbol, Production > m_productRules;
    QList <SyntacticSymbol> m_symbolToParseList;
    QList <Token> m_tokenToParseList;

};

#endif // SYNTACTICANALYZER_H
