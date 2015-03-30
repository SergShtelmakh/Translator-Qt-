#ifndef ERRORGENERATOR_H
#define ERRORGENERATOR_H
#include <QString>
#include <QList>
#include "Token.h"
#include "SyntacticSymbol.h"

class ErrorGenerator
{

public:

    static QString lexicalError(Token incorrectToken);
    static QString syntacticError(QList<Token> tokenToParseList, QList<SyntacticSymbol> syntacticSymbolList);

};

#endif // ERRORGENERATOR_H
