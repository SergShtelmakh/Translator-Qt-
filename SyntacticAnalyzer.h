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

    void addProductRule(const SyntacticSymbol &firstSymbol, const QList <SyntacticSymbol> &productionList);
    void analyze(const QList<Token> &tokenList);
    QString errorText() const;

    QStringList usedRuleList() const;

private:

    Production findCongruentRule(const SyntacticSymbol &firstSymbol, const Token &firstProductSymbol) const;
    Production findLambdaRule(const SyntacticSymbol &firstSymbol) const;
    bool isLambdaRuleExists(const SyntacticSymbol &firstSymbol);

    void prepareToAnalysis(const QList<Token> &tokenList);
    void addError(const QString &errorText);
    void useRule(int number, const SyntacticSymbol &leftPart, const QList<SyntacticSymbol> &rightPart);
    QMultiHash < SyntacticSymbol, Production > m_productRules;
    QList <SyntacticSymbol> m_symbolToParseList;
    QList <Token> m_tokenToParseList;
    QString m_errorText;
    QStringList m_usedRuleList;

};

#endif // SYNTACTICANALYZER_H
