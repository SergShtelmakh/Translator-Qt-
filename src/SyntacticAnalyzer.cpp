#include "SyntacticAnalyzer.h"
#include <QDebug>

SyntacticAnalyzer::SyntacticAnalyzer()
{
    SyntacticSymbol startSymbol = SyntacticSymbol("",SyntacticSymbol::startSymbol);
    SyntacticSymbol nonterminalSymbol1 = SyntacticSymbol("val",SyntacticSymbol::nonterminalSymbol);
    SyntacticSymbol nonterminalSymbol2 = SyntacticSymbol("val2",SyntacticSymbol::nonterminalSymbol);

    SyntacticSymbol dataSymbol = SyntacticSymbol("DATA",SyntacticSymbol::terminalSymbol,Token::categoryKeyword);
    SyntacticSymbol idSymbol = SyntacticSymbol("id",SyntacticSymbol::terminalSymbol,Token::categoryIdentifier);
    SyntacticSymbol commaSymbol = SyntacticSymbol(",",SyntacticSymbol::terminalSymbol,Token::categoryCharToken);
    SyntacticSymbol lineFeedSymbol = SyntacticSymbol("\n",SyntacticSymbol::terminalSymbol,Token::categoryLineFeed);

    QList < SyntacticSymbol > ruleList1;
    ruleList1 << dataSymbol << nonterminalSymbol1;
    addProductRule(1,startSymbol,ruleList1);

    QList < SyntacticSymbol > ruleList2;
    ruleList2 << idSymbol << nonterminalSymbol2;
    addProductRule(2,nonterminalSymbol1,ruleList2);

    QList < SyntacticSymbol > ruleList3;
    ruleList3 << commaSymbol << nonterminalSymbol1;
    addProductRule(3,nonterminalSymbol2,ruleList3);

    QList < SyntacticSymbol > ruleList4;
    ruleList4 << lineFeedSymbol;
    addProductRule(4,nonterminalSymbol2,ruleList4);

    QList < SyntacticSymbol > ruleList5;
    addProductRule(5,nonterminalSymbol2,ruleList5);

}

SyntacticAnalyzer::~SyntacticAnalyzer()
{

}

void SyntacticAnalyzer::addProductRule(int number, SyntacticSymbol firstSymbol, QList<SyntacticSymbol> productionList)
{
    m_productRules.insert(firstSymbol, Production(number,productionList));
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
