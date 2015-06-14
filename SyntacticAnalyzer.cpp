#include "SyntacticAnalyzer.h"
#include "ErrorGenerator.h"

void SyntacticAnalyzer::addProductRule(const SyntacticSymbol &firstSymbol, const QList<SyntacticSymbol> &productionList)
{
    int ruleNumber = m_productRules.size();
    m_productRules.insert(firstSymbol, Production(ruleNumber,productionList));
}

void SyntacticAnalyzer::analyze(const QList<Token> &tokenList)
{
    this->prepareToAnalysis(tokenList);
    // LL(1) analysis
    while (!m_tokenToParseList.isEmpty()&&!m_symbolToParseList.isEmpty()) {
        Token firstTokenToParse = m_tokenToParseList.first();
        SyntacticSymbol firstSymbolToParse = m_symbolToParseList.first();
        if (firstSymbolToParse == firstTokenToParse) {
            m_tokenToParseList.takeFirst();
            m_symbolToParseList.takeFirst();
        } else {
            Production production = this->findCongruentRule(firstSymbolToParse, firstTokenToParse);
            if (production.syntacticSymbolList().isEmpty() && (!isLambdaRuleExists(firstSymbolToParse))) {
                this->addError(ErrorGenerator::syntacticError(m_tokenToParseList, m_symbolToParseList));
                m_tokenToParseList.takeFirst();
                return;
            }
            m_symbolToParseList.takeFirst();
            m_symbolToParseList = production.syntacticSymbolList() + m_symbolToParseList;
            this->useRule(production.number() + 1, firstSymbolToParse, production.syntacticSymbolList());
        }
    }

    while (!m_symbolToParseList.isEmpty()) {
        SyntacticSymbol symbol = m_symbolToParseList.takeFirst();
        while (symbol.category() == Token::LINE_FEED_CATEGORY) {
            symbol = m_symbolToParseList.takeFirst();
        }
        if (!isLambdaRuleExists(symbol)) {
            this->addError(ErrorGenerator::syntacticError(m_tokenToParseList, m_symbolToParseList));
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
    return this->findLambdaRule(firstSymbol);
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
    m_symbolToParseList << SyntacticSymbol ("S", SyntacticSymbol::START_SYMBOL);
    m_usedRuleList.clear();
}

void SyntacticAnalyzer::addError(QString errorText)
{
   m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + errorText + "\n";
}

void SyntacticAnalyzer::useRule(int number, const SyntacticSymbol &leftPart, const QList<SyntacticSymbol> &rightPart)
{
    m_usedRuleList << QString("№%1\t" + MakeStringRepresentation(leftPart) + " ::= " + MakeStringRepresentation(rightPart)).arg(number);
}

QStringList SyntacticAnalyzer::usedRuleList() const
{
    return m_usedRuleList;
}

QString SyntacticAnalyzer::errorText() const
{
    return m_errorText;
}

