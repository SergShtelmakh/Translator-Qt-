#include "SyntacticAnalyzer.h"
#include "ErrorGenerator.h"

void SyntacticAnalyzer::addProductRule(const SyntacticSymbol &firstSymbol, const QList<SyntacticSymbol> &productionList)
{
    int ruleNumber = m_productRules.size();
    m_productRules.insert(firstSymbol, Production(ruleNumber,productionList));
}

void SyntacticAnalyzer::analyze(const QList<Token> &tokenList)
{
    prepareToAnalysis(tokenList);

    while (!m_tokenToParseList.isEmpty()&&!m_symbolToParseList.isEmpty()) {
        Token firstTokenToParse = m_tokenToParseList.first();
        SyntacticSymbol firstSymbolToParse = m_symbolToParseList.first();
        if (firstSymbolToParse == firstTokenToParse) {
            m_tokenToParseList.takeFirst();
            m_symbolToParseList.takeFirst();
        } else {
            Production production = findCongruentRule(firstSymbolToParse, firstTokenToParse);
            if (production.syntacticSymbolList().isEmpty()&&(!isLambdaRuleExists(firstSymbolToParse))) {
                QString error = ErrorGenerator::syntacticError(m_tokenToParseList,m_symbolToParseList);
                addError(error);
                m_tokenToParseList.takeFirst();
                return;
            }
            m_symbolToParseList.takeFirst();
            m_symbolToParseList = production.syntacticSymbolList() + m_symbolToParseList;
            useRule(production.number() + 1, firstSymbolToParse, production.syntacticSymbolList());
        }
    }

    while (!m_symbolToParseList.isEmpty()) {
        SyntacticSymbol symbol = m_symbolToParseList.takeFirst();
        while (symbol.category() == Token::categoryLineFeed) {
            symbol = m_symbolToParseList.takeFirst();
        }
        if (!isLambdaRuleExists(symbol)) {
            QString error = ErrorGenerator::syntacticError(m_tokenToParseList,m_symbolToParseList);
            addError(error);
            return;
        }
    }
}

Production SyntacticAnalyzer::findCongruentRule(const SyntacticSymbol &firstSymbol, const Token &firstProductSymbol) const
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (!production.syntacticSymbolList().isEmpty()) {
            if (production.getFirstSymbol() == firstProductSymbol) {
                return production;
            }
        }
    }
    return findLambdaRule(firstSymbol);
}

Production SyntacticAnalyzer::findLambdaRule(const SyntacticSymbol &firstSymbol) const
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (production.syntacticSymbolList().isEmpty())
            return production;
    }
    return Production();
}

bool SyntacticAnalyzer::isLambdaRuleExists(const SyntacticSymbol &firstSymbol)
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (production.syntacticSymbolList().isEmpty())
            return true;
    }
    return false;
}

void SyntacticAnalyzer::prepareToAnalysis(const QList<Token> &tokenList)
{
    m_tokenToParseList = tokenList;
    m_symbolToParseList.clear();
    m_errorText.clear();
    m_symbolToParseList << SyntacticSymbol ("S",SyntacticSymbol::startSymbol);
    m_usedRuleList.clear();
}

void SyntacticAnalyzer::addError(const QString &errorText)
{
   m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + errorText + "\n";
}

void SyntacticAnalyzer::useRule(int number, const SyntacticSymbol &leftPart, const QList<SyntacticSymbol> &rightPart)
{
    m_usedRuleList << QString("№%1\t" + MakeString(leftPart) + " ::= " + MakeString(rightPart)).arg(number);
}

QStringList SyntacticAnalyzer::usedRuleList() const
{
    return m_usedRuleList;
}

QString SyntacticAnalyzer::errorText() const
{
    return m_errorText;
}

