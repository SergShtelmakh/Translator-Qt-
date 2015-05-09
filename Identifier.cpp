#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier) :
    m_name(newIdentifier.name()),
    m_type(newIdentifier.type()),
    m_scopeBeginLineNumber(newIdentifier.scopeBeginLineNumber()),
    m_scopeEndLineNumber(newIdentifier.scopeEndLineNumber())
{}

Identifier::Identifier(const QString &name, Identifier::IdentifierType type, int scopeBeginLineNumber, int scopeEndLineNumber) :
    m_name(name),
    m_type(type),
    m_scopeBeginLineNumber(scopeBeginLineNumber),
    m_scopeEndLineNumber(scopeEndLineNumber)
{}

bool Identifier::operator==(const Identifier &otherIdentifier) const
{
    return m_name == otherIdentifier.name();
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

QString Identifier::name() const
{
    return m_name;
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
