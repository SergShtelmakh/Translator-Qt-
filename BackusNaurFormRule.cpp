#include "BackusNaurFormRule.h"

BackusNaurFormRule::BackusNaurFormRule(SyntacticSymbol leftPart, QList<SyntacticSymbol> rightPart) :
    m_leftPart(leftPart),
    m_rightPart(rightPart)
{}

SyntacticSymbol BackusNaurFormRule::leftPart() const
{
    return m_leftPart;
}

void BackusNaurFormRule::setLeftPart(const SyntacticSymbol &leftPart)
{
    m_leftPart = leftPart;
}

QList<SyntacticSymbol> BackusNaurFormRule::rightPart() const
{
    return m_rightPart;
}

void BackusNaurFormRule::setRightPart(const QList<SyntacticSymbol> &rightPart)
{
    m_rightPart = rightPart;
}



