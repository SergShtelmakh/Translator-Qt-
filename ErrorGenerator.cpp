#include "ErrorGenerator.h"

QString ErrorGenerator::lexicalError(const Token &incorrectToken)
{
    return QString("(%1:%2)\t ").arg(incorrectToken.position().x()).arg(incorrectToken.position().y()) + incorrectToken.getAllErrorInformation();
}

QString ErrorGenerator::syntacticError(const QList<Token> &tokenToParseList,const QList<SyntacticSymbol> &syntacticSymbolList)
{
    if (tokenToParseList.isEmpty()) {
        if (syntacticSymbolList.isEmpty()) {
            return QString();
        } else {
            return "\tCharacter is missing, can't parse " + MakeString(syntacticSymbolList.first());
        }
    } else {
        QString tokenPosition = QString("(%1:%2)\t ").arg(tokenToParseList.first().position().x()).arg(tokenToParseList.first().position().y());
        if (syntacticSymbolList.isEmpty()) {
            return tokenPosition + " Can't find rule to parse from " + MakeString(tokenToParseList.first());
        } else {
            if (tokenToParseList.first().tokenCategory() == Token::categoryLineFeed) {
                return tokenPosition + " Missing " + MakeString(syntacticSymbolList.first());
            } else {
                return tokenPosition + " Can't find rule to parse " + MakeString(syntacticSymbolList.first()) + " from " + MakeString(tokenToParseList.first());
            }
        }
    }
    return QString();
}
