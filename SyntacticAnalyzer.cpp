#include "SyntacticAnalyzer.h"
#include "ErrorGenerator.h"


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
    prepareToAnalysis(tokenList);

    while (!m_tokenToParseList.isEmpty()) {
        Token firstTokenToParse = m_tokenToParseList.first();
        SyntacticSymbol firstSymbolToParse = m_symbolToParseList.first();
        if (firstSymbolToParse == firstTokenToParse) {
            m_tokenToParseList.takeFirst();
            m_symbolToParseList.takeFirst();
        } else {
            Production production = findCongruentRule(firstSymbolToParse, firstTokenToParse);
            if (production.syntacticSymbolList().isEmpty()&&(!isLambdaRuleExists(firstSymbolToParse))) {
                addError(ErrorGenerator::syntacticError(m_tokenToParseList,m_symbolToParseList));
                m_tokenToParseList.takeFirst();
                continue;
            }
            m_symbolToParseList.takeFirst();
            m_symbolToParseList = production.syntacticSymbolList() + m_symbolToParseList;
            useRule(production.number() + 1, firstSymbolToParse, production.syntacticSymbolList());
        }
    }
    while (!m_symbolToParseList.isEmpty()) {
        SyntacticSymbol symbol = m_symbolToParseList.first();
        if (!isLambdaRuleExists(symbol)) {
            addError(ErrorGenerator::syntacticError(m_tokenToParseList,m_symbolToParseList));
            return;
        }
        m_symbolToParseList.takeFirst();
    }
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
    return Production();
}

bool SyntacticAnalyzer::isLambdaRuleExists(SyntacticSymbol firstSymbol)
{
    foreach (Production production, m_productRules.values(firstSymbol)) {
        if (production.syntacticSymbolList().isEmpty())
            return true;
    }
    return false;
}

void SyntacticAnalyzer::prepareToAnalysis(QList<Token> tokenList)
{
    m_tokenToParseList = tokenList;
    m_symbolToParseList.clear();
    m_errorText.clear();
    m_symbolToParseList << SyntacticSymbol ("S",SyntacticSymbol::startSymbol);
    m_usedRuleList.clear();
}

void SyntacticAnalyzer::addError(QString errorText)
{
   m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + errorText + "\n";
}

void SyntacticAnalyzer::useRule(int number, SyntacticSymbol leftPart, QList<SyntacticSymbol> rightPart)
{
    m_usedRuleList << QString("№%1\t"
                              + MakeString(leftPart)
                              + " ::= "
                              + MakeString(rightPart)).arg(number);
}

QStringList SyntacticAnalyzer::usedRuleList() const
{
    return m_usedRuleList;
}

QString SyntacticAnalyzer::errorText() const
{
    return m_errorText;
}

