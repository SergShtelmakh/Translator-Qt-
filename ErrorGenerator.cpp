#include "ErrorGenerator.h"
#include "Token.h"
#include "SyntacticSymbol.h"

QString ErrorGenerator::lexicalError(const Token &incorrectToken)
{
    return QString("(%1:%2)\t ").arg(incorrectToken.position().x()).arg(incorrectToken.position().y()) + incorrectToken.getErrorInformationWithLexeme();
}

QString ErrorGenerator::syntacticError(const QList<Token> &tokenToParseList,const QList<SyntacticSymbol> &syntacticSymbolList)
{
    if (tokenToParseList.isEmpty()) {
        if (syntacticSymbolList.isEmpty()) {
            return QString();
        } else {
            return "\tCharacter is missing, can't parse " + MakeStringRepresentation(syntacticSymbolList.first());
        }
    } else {
        QString tokenPosition = QString("(%1:%2)\t ")
                                .arg(tokenToParseList.first().position().x())
                                .arg(tokenToParseList.first().position().y());
        if (syntacticSymbolList.isEmpty()) {
            return tokenPosition + " Can't find rule to parse from " + MakeStringRepresentation(tokenToParseList.first());
        } else {
            if (tokenToParseList.first().category() == Token::LINE_FEED_CATEGORY) {
                return tokenPosition + " Missing " + MakeStringRepresentation(syntacticSymbolList.first());
            } else {
                return tokenPosition
                        + " Can't find rule to parse "
                        + MakeStringRepresentation(syntacticSymbolList.first())
                        + " from "
                        + MakeStringRepresentation(tokenToParseList.first());
            }
        }
    }
    return QString();
}

QString ErrorGenerator::undeclaratedIdentifierError(const Token &identifierToken)
{
    return QString("Undeclarated identifier " + identifierToken.lexeme() + " at (%1:%2)")
            .arg(identifierToken.position().x())
            .arg(identifierToken.position().y());
}

QString ErrorGenerator::redeclarationOfIdentifier(const Token &identifierToken)
{
    return QString("Redeclaration of identifier " + identifierToken.lexeme() + " at (%1:%2)")
            .arg(identifierToken.position().x())
            .arg(identifierToken.position().y());
}

QString ErrorGenerator::incorrectTypeToAssignment(const Token &idToken)
{
    return QString("Incorrect type to assignment at (%1,%2)\n")
            .arg(idToken.position().x())
            .arg(idToken.position().y());
}

QString ErrorGenerator::incorrectIdentifier(const Token &idToken)
{
    return QString("Incorrect id at (%1,%2)\n")
            .arg(idToken.position().x())
            .arg(idToken.position().y());
}

QString ErrorGenerator::cantDoOperation(const Token &operation, const Token &token1, const Token &token2)
{
    return "Can't do operation "
            + operation.lexeme()
            + " with "
            + token1.lexeme() + " "
            + token2.lexeme() + "\n";
}
