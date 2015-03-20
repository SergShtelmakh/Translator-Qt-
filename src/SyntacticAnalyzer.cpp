#include "SyntacticAnalyzer.h"
#include <QDebug>

SyntacticAnalyzer::SyntacticAnalyzer()
{

}

SyntacticAnalyzer::~SyntacticAnalyzer()
{

}

void SyntacticAnalyzer::addProductRule(SyntacticSymbol firstSymbol, QList<SyntacticSymbol> productionList)
{
    int ruleNumber = m_productRules.size();
    m_productRules.insert(firstSymbol, Production(ruleNumber,productionList));
}

void SyntacticAnalyzer::analyze(QList<Token> tokenList)
{
    m_tokenToParseList = tokenList;
    m_symbolToParseList.clear();
    m_symbolToParseList << SyntacticSymbol ("",SyntacticSymbol::startSymbol);

    qDebug() << "New analysis:";
    while (!m_tokenToParseList.isEmpty()) {
        Token firstTokenToParse = m_tokenToParseList.first();
        SyntacticSymbol firstSymbolToParse = m_symbolToParseList.first();
        if (firstSymbolToParse == firstTokenToParse) {
            m_tokenToParseList.takeFirst();
            m_symbolToParseList.takeFirst();
        } else {
            Production production = findCongruentRule(firstSymbolToParse, firstTokenToParse);
            if (production.syntacticSymbolList().isEmpty()) {
                qDebug() << "Can't find rule ";
                return;
            }
            m_symbolToParseList.takeFirst();
            m_symbolToParseList = production.syntacticSymbolList() + m_symbolToParseList;
            qDebug() << "Using " << production.number() << " rule";
        }
    }
    while (!m_symbolToParseList.isEmpty()) {
        SyntacticSymbol symbol = m_symbolToParseList.takeFirst();
        if (!isFinalRuleExist(symbol)) {
            qDebug() << "Character is missing";
            return;
        }
    }
}

SyntacticSymbol SyntacticAnalyzer::getNextSymbolToParse()
{
    if (m_symbolToParseList.isEmpty())
        return SyntacticSymbol("",SyntacticSymbol::startSymbol);

    SyntacticSymbol nextSymbol = m_symbolToParseList.takeFirst();
    nextSymbol = m_symbolToParseList.takeFirst();
    return nextSymbol;
}

Production SyntacticAnalyzer::findCongruentRule(SyntacticSymbol firstSymbol, Token firstProductSymbol)
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (!production.syntacticSymbolList().isEmpty()) {
            if (production.getFirstSymbol() == firstProductSymbol) {
                return production;
            }
        }
    }
    Production emptyProduct;
    return emptyProduct;
}

bool SyntacticAnalyzer::isFinalRuleExist(SyntacticSymbol firstSymbol)
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (production.syntacticSymbolList().isEmpty())
            return true;
    }
    return false;
}
