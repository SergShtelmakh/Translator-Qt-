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
    m_errorText.clear();
    m_symbolToParseList << SyntacticSymbol ("S",SyntacticSymbol::startSymbol);

    qDebug() << "New analysis:";
    while (!m_tokenToParseList.isEmpty()) {
        Token firstTokenToParse = m_tokenToParseList.first();
        SyntacticSymbol firstSymbolToParse = m_symbolToParseList.first();
        if (firstSymbolToParse == firstTokenToParse) {
            m_tokenToParseList.takeFirst();
            m_symbolToParseList.takeFirst();
        } else {
            Production production = findCongruentRule(firstSymbolToParse, firstTokenToParse);
            if (production.syntacticSymbolList().isEmpty()&&(!isFinalRuleExist(firstSymbolToParse))) {
//                m_errorText += "Can't find rule \n";
                qDebug() << "Can't find rule";
//                m_errorText << "Can't find rule ";
                return;
            }
            m_symbolToParseList.takeFirst();
            m_symbolToParseList = production.syntacticSymbolList() + m_symbolToParseList;
//            m_errorText += QString("Using %1 rule").arg(production.number() + 1);
            qDebug() << "Using" << production.number() + 1 << "rule";
        }
    }
    while (!m_symbolToParseList.isEmpty()) {
        SyntacticSymbol symbol = m_symbolToParseList.takeFirst();
        if (!isFinalRuleExist(symbol)) {
//            m_errorText += "Character is missing";
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
QString SyntacticAnalyzer::errorText() const
{
    return m_errorText;
}

