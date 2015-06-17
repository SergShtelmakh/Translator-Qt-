/*!
 * \file Token.h
 * \date 2015/06/13
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <QPoint>
#include <QString>
#include <QHash>
#include "Expression.h"

/*!
 * @brief This class used to store informations about tokens.
 */
class Identifier;
class Token
{

public:

    /*!
     * @brief This enumerate include tokens category.
     */
    enum TokenCategory {
        NONE_CATEGORY           = 0,    //!< Incorrect token.
        CHAR_TOKEN_CATEGORY     = 1,    //!< Character token (e.g. +, -, *, / ...).
        IDENTIFIER_CATEGORY     = 2,    //!< Identifier (e.g. var1, var2 ...).
        KEYWORD_CATEGORY        = 3,    //!< Keyword (e.g. int, bool, double ...).
        LINE_FEED_CATEGORY      = 4,    //!< Line feed.
        NUMBER_LITERAL_CATEGORY = 5,    //!< Number literal (e.g. 5.54, 48.21E-45 ...).
        SPACE_CATEGORY          = 6,    //!< Space token.
        STRING_LITERAL_CATEGORY = 7     //!< String litaral (e.g. "literal1", "literal2" ...).
    };

    Token(){}
    Token(const Token &other);
    Token(const QString &lexeme, TokenCategory category, const QString &errorInformation = "", const QPoint &position = QPoint(), Expression::Type type = Expression::NONE_TYPE);

    Token& operator=(const Token& newToken);

    QString lexeme() const;
    TokenCategory category() const;
    QString errorInformation() const;

    void setPosition(const QPoint &value);
    QPoint position() const;

    Identifier *identifier() const;
    void setIdentifier(Identifier *identifier);

    Expression::Type type() const;
    void setType(const Expression::Type &type);

    QString getErrorInformationWithLexeme() const;

    /*!
     * This method used to check is token correct.
     *
     * @return If token is correct returns true other returns false.
     */
    bool isCorrect() const;

    static TokenCategory stringToTokenCategory(const QString &string);

private:

    QString m_lexeme;               //!< Tokens lexeme.
    TokenCategory m_tokenCategory;  //!< Tokens category.
    QString m_errorInformation;     //!< Tokens error information.
    QPoint m_position;              //!< Tokens position.
    Identifier *m_identifier;       //!< Pointer on identifer.
    Expression::Type m_type;        //!< Tokens type (if token is result of some expression)

    static QHash <QString, TokenCategory> m_convertingStringToTokenCategoryHash;

};

QString MakeStringRepresentation(const Token &token);
int GetTokenLineNumber(const Token &token);
int GetOperationPriority(const Token &token);
bool isOperation(const Token &token);
bool isLogicalOperation(const Token &token);
Expression::Type resultType(const Token &operation, const Token &first, const Token &second);

#endif // TOKEN_H
