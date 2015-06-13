/*!
 * \file Identifier.h
 * \date 2015/06/13
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QList>
#include <QString>
#include "Expression.h"

/*!
 * @brief This class used to store informations about identifiers.
 */

class Identifier
{

public:

    Identifier(){}
    Identifier(const Identifier &other);
    Identifier(const QString &lexeme, Expression::Type type, int scopeBeginLineNumber, int scopeEndLineNumber);

    QString lexeme() const;
    bool operator==(const Identifier &otherIdentifier)const;

    Expression::Type type() const;
    int scopeBeginLineNumber() const;
    int scopeEndLineNumber() const;

    int code() const;
    void setCode(int code);

private:

    QString m_lexeme;               //!< Identifiers name.
    Expression::Type m_type;        //!< Identifiers type.
    int m_scopeBeginLineNumber;     //!< Identifiers scope begin line number.
    int m_scopeEndLineNumber;       //!< Identifiers scope end line number.
    int m_code;                     //!< Identifiers code (in memory).
};

Expression::Type StringToType(QString lexeme);
QString TypeToString(Expression::Type type);


#endif // IDENTIFIER_H
