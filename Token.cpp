#include "Token.h"

QHash<QString,Token::TokenCategory> Token::m_convertingStringToTokenCategoryHash;

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

bool Token::isCorrect() const
{
    return m_tokenCategory != Token::categoryNone;
}

Token::TokenCategory Token::stringToTokenCategory(QString string)
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

QString MakeString(Token token)
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
        return "<linefeed>\n";
    default:
        return "<" + token.lexeme() + ">";
    }
}
