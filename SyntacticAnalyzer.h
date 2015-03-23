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
    QString errorText() const;

private:

    Production findCongruentRule(SyntacticSymbol firstSymbol, Token firstProductSymbol);
    bool isLambdaRuleExists(SyntacticSymbol firstSymbol);

    void prepareToAnalysis(QList<Token> tokenList);
    QMultiHash < SyntacticSymbol, Production > m_productRules;
    QList <SyntacticSymbol> m_symbolToParseList;
    QList <Token> m_tokenToParseList;
    QString m_errorText;

};

#endif // SYNTACTICANALYZER_H
