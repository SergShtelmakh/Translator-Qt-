#include "Token.h"

QHash<QString,Token::TokenCategory> Token::m_convertingStringToTokenCategoryHash;

Token::Token(const Token &otherToken) :
    m_lexeme(otherToken.lexeme()),
    m_tokenCategory(otherToken.tokenCategory()),
    m_errorInformation(otherToken.errorInformation()),
    m_position(otherToken.position())
{}

Token::Token(const QString &lexeme, Token::TokenCategory tokenCategory, const QString &errorInformation, const QPoint &position) :
    m_lexeme(lexeme),
    m_tokenCategory(tokenCategory),
    m_errorInformation(errorInformation),
    m_position(position)
{}

Token &Token::operator=(const Token &otherToken)
{
    if (this == &otherToken)
        return *this;

    m_lexeme = otherToken.lexeme();
    m_tokenCategory = otherToken.tokenCategory();
    m_position = otherToken.position();
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

bool Token::isCorrect() const
{
    return m_tokenCategory != Token::categoryNone;
}

Token::TokenCategory Token::stringToTokenCategory(const QString &string)
{
    if (m_convertingStringToTokenCategoryHash.isEmpty()) {
        m_convertingStringToTokenCategoryHash.insert("categoryCharToken",Token::categoryCharToken);
        m_convertingStringToTokenCategoryHash.insert("categoryIdentifier",Token::categoryIdentifier);
        m_convertingStringToTokenCategoryHash.insert("categoryKeyword",Token::categoryKeyword);
        m_convertingStringToTokenCategoryHash.insert("categoryLineFeed",Token::categoryLineFeed);
        m_convertingStringToTokenCategoryHash.insert("categoryNone",Token::categoryNone);
        m_convertingStringToTokenCategoryHash.insert("categoryNumberLiteral",Token::categoryNumberLiteral);
        m_convertingStringToTokenCategoryHash.insert("categorySpace",Token::categorySpace);
        m_convertingStringToTokenCategoryHash.insert("categoryStringLiteral",Token::categoryStringLiteral);
    }

    return m_convertingStringToTokenCategoryHash.value(string);
}

QString Token::errorInformation() const
{
    return m_errorInformation ;
}

QString Token::getAllErrorInformation() const
{
    return m_lexeme + ": " + m_errorInformation;
}

QString MakeString(const Token &token)
{
    if (!token.isCorrect())
        return "";

    switch (token.tokenCategory()) {
    case Token::categoryIdentifier:
        return "<id, \"" + token.lexeme() + "\">";
    case Token::categoryNumberLiteral:
        return "<numb, \"" + token.lexeme() + "\">";
    case Token::categoryStringLiteral:
        return "<literal, " + token.lexeme() + ">";
    case Token::categorySpace:
        return "";
    case Token::categoryLineFeed:
        return "<linefeed>";
    default:
        return "<" + token.lexeme() + ">";
    }
}
