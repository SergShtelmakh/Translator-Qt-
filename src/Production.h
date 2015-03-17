#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "SyntacticSymbol.h"
#include <QList>

class Production
{

public:

    Production();
    Production(int number, QList <SyntacticSymbol> syntacticSymbolList);
    ~Production();

    int number() const;
    void setNumber(int number);

    QList<SyntacticSymbol> syntacticSymbolList() const;
    void setSyntacticSymbolList(const QList<SyntacticSymbol> &syntacticSymbolList);

    SyntacticSymbol getFirstSymbol();
private:

    int m_number;
    QList <SyntacticSymbol> m_syntacticSymbolList;
};

#endif // PRODUCTION_H
