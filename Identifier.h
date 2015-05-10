/*!
 * \file Identifier.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QList>
#include <QString>

/*!
 * @brief This class used to store informations about identifiers.
 */
class Identifier
{

public:

    enum IdentifierType {
        BOOLEAN,
        DOUBLE,
        INTEGER,
        STRING,
        NONE
    };

    Identifier(const Identifier &other);
    Identifier(const QString &lexeme, IdentifierType type, int scopeBeginLineNumber, int scopeEndLineNumber);

    QString lexeme() const;
    bool operator==(const Identifier &otherIdentifier)const;

    IdentifierType type() const;
    int scopeBeginLineNumber() const;
    int scopeEndLineNumber() const;

private:

    QString m_lexeme;                     //!< Identifiers name.
    IdentifierType m_type;
    int m_scopeBeginLineNumber;
    int m_scopeEndLineNumber;

};

Identifier::IdentifierType MakeIdentifierType(QString lexeme);

#endif // IDENTIFIER_H
