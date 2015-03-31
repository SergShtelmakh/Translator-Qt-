#ifndef SYNTACTICANALYZER_H
#define SYNTACTICANALYZER_H

#include <QHash>
#include "SyntacticSymbol.h"
#include "Token.h"
#include "Production.h"
#include <QStringList>

class SyntacticAnalyzer
{

public:

    SyntacticAnalyzer();
    ~SyntacticAnalyzer();

    void addProductRule(SyntacticSymbol firstSymbol, QList <SyntacticSymbol> productionList);
    void analyze(QList<Token> tokenList);
    QString errorText() const;

    QStringList usedRuleList() const;

private:

    Production findCongruentRule(SyntacticSymbol firstSymbol, Token firstProductSymbol);
    Production findLambdaRule(SyntacticSymbol firstSymbol);
    bool isLambdaRuleExists(SyntacticSymbol firstSymbol);

    void prepareToAnalysis(QList<Token> tokenList);
    void addError(QString errorText);
    void useRule(int number, SyntacticSymbol leftPart, QList<SyntacticSymbol> rightPart);
    QMultiHash < SyntacticSymbol, Production > m_productRules;
    QList <SyntacticSymbol> m_symbolToParseList;
    QList <Token> m_tokenToParseList;
    QString m_errorText;
    QStringList m_usedRuleList;

};

#endif // SYNTACTICANALYZER_H
