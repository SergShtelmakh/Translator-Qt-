#include "Token.h"
#include "Identifier.h"

QHash<QString,Token::TokenCategory> Token::m_convertingStringToTokenCategoryHash;

Token::Token(const Token &otherToken) :
    m_lexeme(otherToken.lexeme()),
    m_tokenCategory(otherToken.category()),
    m_errorInformation(otherToken.errorInformation()),
    m_position(otherToken.position()),
    m_identifier(otherToken.identifier()),
    m_type(otherToken.type())
{}

Token::Token(const QString &lexeme,
             Token::TokenCategory tokenCategory,
             const QString &errorInformation,
             const QPoint &position,
             Expression::Type type) :
    m_lexeme(lexeme),
    m_tokenCategory(tokenCategory),
    m_errorInformation(errorInformation),
    m_position(position),
    m_identifier(NULL),
    m_type(type)
{}

Token &Token::operator=(const Token &otherToken)
{
    if (this == &otherToken)
        return *this;

    m_lexeme = otherToken.lexeme();
    m_tokenCategory = otherToken.category();
    m_errorInformation = otherToken.errorInformation();
    m_position = otherToken.position();
    m_type = otherToken.type();
    return *this;
}

QString Token::lexeme() const
{
    return m_lexeme;
}

Token::TokenCategory Token::category() const
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
    return m_tokenCategory != Token::NONE_CATEGORY;
}

Token::TokenCategory Token::stringToTokenCategory(const QString &string)
{
    if (m_convertingStringToTokenCategoryHash.isEmpty()) {
        m_convertingStringToTokenCategoryHash.insert("categoryCharToken",       Token::CHAR_TOKEN_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryIdentifier",      Token::IDENTIFIER_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryKeyword",         Token::KEYWORD_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryLineFeed",        Token::LINE_FEED_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryNone",            Token::NONE_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryNumberLiteral",   Token::NUMBER_LITERAL_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categorySpace",           Token::SPACE_CATEGORY);
        m_convertingStringToTokenCategoryHash.insert("categoryStringLiteral",   Token::STRING_LITERAL_CATEGORY);
    }

    return m_convertingStringToTokenCategoryHash.value(string);
}

Identifier *Token::identifier() const
{
    return m_identifier;
}

void Token::setIdentifier(Identifier *identifier)
{
    m_identifier = identifier;
    m_type = identifier->type();
}

Expression::Type Token::type() const
{
    return m_type;
}

void Token::setType(const Expression::Type &type)
{
    m_type = type;
}

QString Token::errorInformation() const
{
    return m_errorInformation;
}

QString Token::getErrorInformationWithLexeme() const
{
    return m_lexeme + ": " + m_errorInformation;
}

QString MakeStringRepresentation(const Token &token)
{
    if (!token.isCorrect())
        return "";

    switch (token.category()) {
    case Token::IDENTIFIER_CATEGORY:
        return "<id, \"" + token.lexeme() + "\">";
    case Token::NUMBER_LITERAL_CATEGORY:
        return "<numb, \"" + token.lexeme() + "\">";
    case Token::STRING_LITERAL_CATEGORY:
        return "<literal, " + token.lexeme() + ">";
    case Token::SPACE_CATEGORY:
        return "";
    case Token::LINE_FEED_CATEGORY:
        return "<linefeed>";
    default:
        return "<" + token.lexeme() + ">";
    }
}

int GetTokenLineNumber(const Token &token)
{
    return token.position().y();
}

int GetOperationPriority(const Token &token)
{
    if (token.lexeme() == "(" || token.lexeme() == ")") {
        return 0;
    } else if (token.lexeme() == "=="
               || token.lexeme() == "<"
               || token.lexeme() == ">"
               || token.lexeme() == "<="
               || token.lexeme() == ">=") {
        return 1;
    } else if (token.lexeme() == "+" || token.lexeme() == "-") {
        return 2;
    } else if (token.lexeme() == "*" || token.lexeme() == "/") {
        return 3;
    } else {
        return -1;
    }
}

bool isOperation(const Token &token)
{
    return (GetOperationPriority(token) > 0);
}

bool isLogicalOperation(const Token &token)
{
    return (GetOperationPriority(token) == 1);
}

Expression::Type resultType(const Token &operation, const Token &first, const Token &second)
{
    if ((first.type() == Expression::BOOLEAN_TYPE) || (second.type() == Expression::BOOLEAN_TYPE))
        return Expression::NONE_TYPE;

    switch (first.type()) {
    case Expression::DOUBLE_TYPE: {
        if (second.type() == Expression::STRING_TYPE) {
            return Expression::NONE_TYPE;
        } else if (isLogicalOperation(operation)) {
            return Expression::BOOLEAN_TYPE;
        } else {
            return Expression::DOUBLE_TYPE;
        }
    }
    case Expression::INTEGER_TYPE: {
        if (second.type() == Expression::STRING_TYPE) {
            return Expression::NONE_TYPE;
        } else if (isLogicalOperation(operation)) {
            return Expression::BOOLEAN_TYPE;
        } else if (second.type() == Expression::DOUBLE_TYPE) {
            return Expression::DOUBLE_TYPE;
        } else {
            return Expression::INTEGER_TYPE;
        }
    }
    case Expression::STRING_TYPE: {
        if (second.type() != Expression::STRING_TYPE) {
            return Expression::NONE_TYPE;
        } else if (isLogicalOperation(operation)) {
            return Expression::BOOLEAN_TYPE;
        } else {
            return Expression::STRING_TYPE;
        }
    }
    default:
        return Expression::NONE_TYPE;
    }
}
