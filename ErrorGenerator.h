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

};

#endif // ERRORGENERATOR_H
