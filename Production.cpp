#include "Production.h"

Production::Production(int number, QList<SyntacticSymbol> syntacticSymbolList) :
    m_number(number),
    m_syntacticSymbolList(syntacticSymbolList)
{}

int Production::number() const
{
    return m_number;
}

void Production::setNumber(int number)
{
    m_number = number;
}

QList<SyntacticSymbol> Production::syntacticSymbolList() const
{
    return m_syntacticSymbolList;
}

void Production::setSyntacticSymbolList(const QList<SyntacticSymbol> &syntacticSymbolList)
{
    m_syntacticSymbolList = syntacticSymbolList;
}

SyntacticSymbol Production::getFirstSymbol()
{
    return m_syntacticSymbolList.first();
}



