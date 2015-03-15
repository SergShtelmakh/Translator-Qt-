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

    QString getLexeme() const;
    TokenCategory getTokenCategory() const;
    void setPosition(const QPoint &value);
    QPoint getPosition() const;

    int getBeginIndexInLine()const;
    int getEndIndexInLine()const;
    QString getTokenRepresentation();
    QString getErrorInformation() const;

    bool isCorrect() const;

private:

    QString lexeme;
    TokenCategory tokenCategory;
    QPoint position;
    QString errorInformation;

};

#endif // TOKEN_H
