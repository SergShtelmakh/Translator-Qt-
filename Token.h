#ifndef TOKEN_H
#define TOKEN_H

#include <QPoint>
#include <QString>
#include <QHash>

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
        categoryNone            = 0,    //!< Incorrect token.
        categoryCharToken       = 1,    //!< Character token (e.g. +, -, *, / ...).
        categoryIdentifier      = 2,    //!< Identifier (e.g. var1, var2 ...).
        categoryKeyword         = 3,    //!< Keyword (e.g. int, bool, double ...).
        categoryLineFeed        = 4,    //!< Line feed.
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

    QString getAllErrorInformation() const;

    QString getTokenRepresentation();

    /*!
     * This method used to check is token correct.
     *
     * @return If token is correct returns true other returns false.
     */
    bool isCorrect() const;

    static TokenCategory stringToTokenCategory(QString string);

private:

    QString m_lexeme;               //!< Tokens lexeme.
    TokenCategory m_tokenCategory;  //!< Tokens category.
    QPoint m_position;              //!< Tokens position.
    QString m_errorInformation;     //!< Tokens error information.

    static QHash <QString, TokenCategory> m_convertingStringToTokenCategoryHash;

};

QString MakeString(Token token);

#endif // TOKEN_H
