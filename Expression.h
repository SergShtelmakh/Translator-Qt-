#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QList>
#include <QString>

class Token;
class Expression
{
public:

    enum Type {
        BOOLEAN,
        DOUBLE,
        INTEGER,
        STRING,
        NONE
    };

    Expression(QList<Token> &tokenList);
    Expression();
    ~Expression();
    Token result() const;

    QString codeList() const;

    QString error() const;

private:

    void makePolishNotation(QList<Token> &tokenList);
    void makeCode();

    Type m_type;
    QList<Token> m_polishNotation;
    QList<QString> m_codeList;

    QList<Token> m_stack;
    QString m_error;
};

#endif // EXPRESSION_H
