#include "Token.h"

Token::Token(const Token &newToken)
{
    m_lexeme = newToken.lexeme();
    m_tokenCategory = newToken.tokenCategory();
    m_position = newToken.position();
    m_errorInformation = newToken.errorInformation();
}

Token::Token(QString newLexeme, Token::TokenCategory newTokenCategory, QPoint newPosition, QString newErrorInformation)
{
    m_lexeme = newLexeme;
    m_tokenCategory = newTokenCategory;
    m_position = newPosition;
    m_errorInformation = newErrorInformation;
}

Token::Token(QString newLexeme, Token::TokenCategory newTokenCategory, QString newErrorInformation)
{
    m_lexeme = newLexeme;
    m_tokenCategory = newTokenCategory;
    m_errorInformation = newErrorInformation;
}

Token &Token::operator=(const Token &newToken)
{
    if (this == &newToken)
        return *this;

    m_lexeme = newToken.lexeme();
    m_tokenCategory = newToken.tokenCategory();
    m_position = newToken.position();
    return *this;
}

QString Token::lexeme() const
{
    return m_lexeme;
}

Token::TokenCategory Token::tokenCategory() const
{
    return m_tokenCategory;
}

QPoint Token::position() const
{
    return m_position;
}

void Token::setPosition(const QPoint &value)
{
    m_position = value;
}

int Token::getBeginIndexInLine() const
{
    return m_position.y();
}

int Token::getEndIndexInLine() const
{
    return m_position.y() + m_lexeme.length();
}

QString Token::getTokenRepresentation()
{
    if (!this->isCorrect())
        return "";

    switch (m_tokenCategory) {
    case Token::categoryIdentifier:
        return "<id, \"" + m_lexeme + "\">";
    case Token::categoryNumberLiteral:
        return "<numb, \"" + m_lexeme + "\">";
    case Token::categoryStringLiteral:
        return "<literal, " + m_lexeme + ">";
    case Token::categorySpace:
        return "";
    case Token::categoryLineFeed:
        return "<linefeed>\n";
    default:
        return "<" + m_lexeme + ">";
    }
}

bool Token::isCorrect() const
{
    return m_tokenCategory != Token::categoryNone;
}

QString Token::errorInformation() const
{
    return m_errorInformation + " " + m_lexeme;
}





