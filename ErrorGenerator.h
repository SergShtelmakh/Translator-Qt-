/*!
 * \file ErrorGenerator.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef ERRORGENERATOR_H
#define ERRORGENERATOR_H

#include <QString>
#include <QList>

class Token;
class SyntacticSymbol;

class ErrorGenerator
{

public:

    static QString lexicalError(const Token &incorrectToken);
    static QString syntacticError(const QList<Token> &tokenToParseList,const QList<SyntacticSymbol> &syntacticSymbolList);
    static QString undeclaratedIdentifierError(const Token &incorrectToken);
    static QString redeclarationOfIdentifier(const Token &identifierToken);

};

#endif // ERRORGENERATOR_H
