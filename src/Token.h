#ifndef TOKEN_H
#define TOKEN_H

#include <QPoint>
#include <QString>

/*!
 * @brief This class used to store informations about tokens.
 */
class Token
{

public:

    /*!
     * @brief This enumerate include tokens category.
     */
    enum TokenCategory{
        categoryCharToken       = 1,    //!< Character token (e.g. +, -, *, / ...).
        categoryIdentifier      = 2,    //!< Identifier (e.g. var1, var2 ...).
        categoryKeyword         = 3,    //!< Keyword (e.g. int, bool, double ...).
        categoryLineFeed        = 4,    //!< Line feed.
        categoryNone            = 0,    //!< Incorrect token.
        categoryNumberLiteral   = 5,    //!< Number literal (e.g. 5.54, 48.21E-45 ...).
        categorySpace           = 6,    //!< Space token.
        categoryStringLiteral   = 7     //!< String litaral (e.g. "literal1", "literal2" ...).
    };

    Token(){}
    Token(const Token &other);
    Token (QString, TokenCategory, QPoint, QString);
    Token (QString, TokenCategory, QString = "");

    Token& operator=(const Token& newToken);

    QString lexeme() const;

    TokenCategory tokenCategory() const;

    QString errorInformation() const;

    void setPosition(const QPoint &value);
    QPoint position() const;

    /*!
     * This method returns token representation (e.g. <+>, <id, "var1">, <number, 5.5> ...)
     *
     * @return Token representation.
     */
    QString getTokenRepresentation();

    /*!
     * This method used to check is token correct.
     *
     * @return If token is correct returns true other returns false.
     */
    bool isCorrect() const;

private:

    QString m_lexeme;               //!< Tokens lexeme.
    TokenCategory m_tokenCategory;  //!< Tokens category.
    QPoint m_position;              //!< Tokens position.
    QString m_errorInformation;     //!< Tokens error information.

};

#endif // TOKEN_H
