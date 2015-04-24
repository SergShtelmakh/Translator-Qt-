/*!
 * \file Production.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "SyntacticSymbol.h"
#include <QList>

/*!
 * @brief This class used to represent right part of product rules.
 */
class Production
{

public:

    Production(){}
    Production(int number, const QList <SyntacticSymbol> &syntacticSymbolList);

    int number() const;
    void setNumber(int number);

    QList<SyntacticSymbol> syntacticSymbolList() const;
    void setSyntacticSymbolList(const QList<SyntacticSymbol> &syntacticSymbolList);

    /*!
     * This method returns first syntactic symbol from product.
     *
     * @return Syntactic symbol.
     */
    SyntacticSymbol getFirstSymbol() const;

private:

    int m_number;                                   //!< Product number.
    QList <SyntacticSymbol> m_syntacticSymbolList;  //!< List of syntactic symbols in product.

};

#endif // PRODUCTION_H
