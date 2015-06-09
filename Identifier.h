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
#include "Expression.h"

/*!
 * @brief This class used to store informations about identifiers.
 */


class Identifier
{

public:



    Identifier(const Identifier &other);
    Identifier(){}
    Identifier(const QString &lexeme, Expression::Type type, int scopeBeginLineNumber, int scopeEndLineNumber);

    QString lexeme() const;
    bool operator==(const Identifier &otherIdentifier)const;

    Expression::Type type() const;
    int scopeBeginLineNumber() const;
    int scopeEndLineNumber() const;

    int code() const;
    void setCode(int code);

private:

    QString m_lexeme;                     //!< Identifiers name.
    Expression::Type m_type;
    int m_scopeBeginLineNumber;
    int m_scopeEndLineNumber;
    int m_code;
};

Expression::Type StringToType(QString lexeme);
QString TypeToString(Expression::Type type);


#endif // IDENTIFIER_H
