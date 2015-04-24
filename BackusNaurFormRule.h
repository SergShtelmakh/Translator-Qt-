/*!
 * \file BackusNaurFormRule.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef BACKUSNAURFORMRULE_H
#define BACKUSNAURFORMRULE_H

#include "SyntacticSymbol.h"
#include <QList>

class BackusNaurFormRule
{

public:

    BackusNaurFormRule(){}
    BackusNaurFormRule(SyntacticSymbol leftPart, QList <SyntacticSymbol> rightPart);

    SyntacticSymbol leftPart() const;
    void setLeftPart(const SyntacticSymbol &leftPart);

    QList<SyntacticSymbol> rightPart() const;
    void setRightPart(const QList<SyntacticSymbol> &rightPart);

private:

    SyntacticSymbol m_leftPart;
    QList <SyntacticSymbol> m_rightPart;

};

#endif // BACKUSNAURFORMRULE_H
