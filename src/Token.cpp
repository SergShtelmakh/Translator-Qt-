#include "Token.h"
#include "GlobalObjects.h"

Token::Token(const Token &newToken)
{
    lexeme = newToken.getLexeme();
    tokenCategory = newToken.getTokenCategory();
    position = newToken.getPosition();
    errorInformation = newToken.getErrorInformation();
}

Token::Token(QString newLexeme, Token::TokenCategory newTokenCategory, QPoint newPosition, QString newErrorInformation)
{
    lexeme = newLexeme;
    tokenCategory = newTokenCategory;
    position = newPosition;
    errorInformation = newErrorInformation;
}

Token::Token(QString newLexeme, Token::TokenCategory newTokenCategory, QString newErrorInformation)
{
    lexeme = newLexeme;
    tokenCategory = newTokenCategory;
    errorInformation = newErrorInformation;
}

Token &Token::operator=(const Token &newToken)
{
    if(this == &newToken){
        return *this;
    }
    lexeme = newToken.getLexeme();
    tokenCategory = newToken.getTokenCategory();
    position = newToken.getPosition();
    return *this;
}

QString Token::getLexeme() const
{
    return lexeme;
}

Token::TokenCategory Token::getTokenCategory() const
{
    return tokenCategory;
}

QPoint Token::getPosition() const
{
    return position;
}

void Token::setPosition(const QPoint &value)
{
    position = value;
}

int Token::getBeginIndexInLine() const
{
    return position.y();
}

int Token::getEndIndexInLine() const
{
    return position.y() + lexeme.length();
}

QString Token::getTokenRepresentation()
{
    if(!this->isCorrect()){
        return "";
    }
    switch (tokenCategory) {
    case Token::categoryIdentifier:
        return "<id, \"" + lexeme + "\">";
    case Token::categoryNumberLiteral:
        return "<numb, \"" + lexeme + "\">";
    case Token::categoryStringLiteral:
        return "<literal, " + lexeme + ">";
    case Token::categorySpace:
        return "";
    case Token::categoryLineFeed:
        return "\n";
    default:
        return "<" + lexeme + ">";
    }
}

bool Token::isCorrect() const
{
    return tokenCategory != Token::categoryNone;
}

QString Token::getErrorInformation() const
{
    return errorInformation + " " + lexeme;
}





