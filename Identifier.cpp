#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier) :
    m_lexeme(newIdentifier.lexeme()),
    m_type(newIdentifier.type()),
    m_scopeBeginLineNumber(newIdentifier.scopeBeginLineNumber()),
    m_scopeEndLineNumber(newIdentifier.scopeEndLineNumber())
{}

Identifier::Identifier(const QString &lexeme, Identifier::IdentifierType type, int scopeBeginLineNumber, int scopeEndLineNumber) :
    m_lexeme(lexeme),
    m_type(type),
    m_scopeBeginLineNumber(scopeBeginLineNumber),
    m_scopeEndLineNumber(scopeEndLineNumber)
{}

bool Identifier::operator==(const Identifier &otherIdentifier) const
{
    return m_lexeme == otherIdentifier.lexeme();
}

Identifier::IdentifierType Identifier::type() const
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

QString Identifier::lexeme() const
{
    return m_lexeme;
}

Identifier::IdentifierType MakeIdentifierType(QString lexeme)
{
    if (lexeme == "INTEGER")
        return Identifier::INTEGER;
    if (lexeme == "BOOLEAN")
        return Identifier::BOOLEAN;
    if (lexeme == "DOUBLE")
        return Identifier::DOUBLE;
    if (lexeme == "STRING")
        return Identifier::STRING;
    return Identifier::NONE;
}


QString IdentifierTypeToString(Identifier::IdentifierType type)
{
    switch (type) {
    case Identifier::BOOLEAN:
        return "BOOLEAN";
    case Identifier::INTEGER:
        return "INTEGER";
    case Identifier::DOUBLE:
        return "DOUBLE";
    case Identifier::STRING:
        return "STRING";
    default:
        return QString();
    }
}
