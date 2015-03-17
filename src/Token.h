#ifndef TOKEN_H
#define TOKEN_H

#include <QPoint>
#include <QString>

class Token
{

public:

    enum TokenCategory{
        categoryBracket,
        categoryComma,
        categoryComparator,
        categoryIdentifier,
        categoryKeyWord,
        categoryLineFeed,
        categoryNone,
        categoryNumberLiteral,
        categoryOperation,
        categorySpace,
        categoryStringLiteral
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

    int getBeginIndexInLine()const;
    int getEndIndexInLine()const;
    QString getTokenRepresentation();


    bool isCorrect() const;

private:

    QString m_lexeme;
    TokenCategory m_tokenCategory;
    QPoint m_position;
    QString m_errorInformation;

};

#endif // TOKEN_H
