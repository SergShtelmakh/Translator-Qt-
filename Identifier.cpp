#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier) :
    m_lexeme(newIdentifier.lexeme()),
    m_type(newIdentifier.type()),
    m_scopeBeginLineNumber(newIdentifier.scopeBeginLineNumber()),
    m_scopeEndLineNumber(newIdentifier.scopeEndLineNumber()),
    m_code(newIdentifier.code())
{}

Identifier::Identifier(const QString &lexeme, Expression::Type type, int scopeBeginLineNumber, int scopeEndLineNumber) :
    m_lexeme(lexeme),
    m_type(type),
    m_scopeBeginLineNumber(scopeBeginLineNumber),
    m_scopeEndLineNumber(scopeEndLineNumber)
{}

bool Identifier::operator==(const Identifier &otherIdentifier) const
{
    return m_lexeme == otherIdentifier.lexeme();
}

Expression::Type Identifier::type() const
{
    return m_type;
}

int Identifier::scopeBeginLineNumber() const
{
    return m_scopeBeginLineNumber;
}

int Identifier::scopeEndLineNumber() const
{
    return m_scopeEndLineNumber;
}
int Identifier::code() const
{
    return m_code;
}

void Identifier::setCode(int code)
{
    m_code = code;
}


QString Identifier::lexeme() const
{
    return m_lexeme;
}

Expression::Type StringToType(QString lexeme)
{
    if (lexeme == "INTEGER")
        return Expression::INTEGER;
    if (lexeme == "BOOLEAN")
        return Expression::BOOLEAN;
    if (lexeme == "DOUBLE")
        return Expression::DOUBLE;
    if (lexeme == "STRING")
        return Expression::STRING;
    return Expression::NONE;
}


QString TypeToString(Expression::Type type)
{
    switch (type) {
    case Expression::BOOLEAN:
        return "BOOLEAN";
    case Expression::INTEGER:
        return "INTEGER";
    case Expression::DOUBLE:
        return "DOUBLE";
    case Expression::STRING:
        return "STRING";
    default:
        return QString();
    }
}
