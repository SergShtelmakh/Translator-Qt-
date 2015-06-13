#ifndef THREEADDRESSCODEGENERATOR_H
#define THREEADDRESSCODEGENERATOR_H

#include <QList>
#include "Token.h"
#include "Expression.h"

struct ForStatement
{
    Token m_id;
    Expression m_beginExpression;
    Expression m_endExpression;
    Expression m_stepExpression;
    QString m_label;

    ForStatement(const Token &id,
                 const Expression &beginExpression,
                 const Expression &endExpression,
                 const QString &label, const
                 Expression &stepExpression = Expression())
    {
        m_id = id;
        m_beginExpression = beginExpression;
        m_endExpression = endExpression;
        m_label = label;
        m_stepExpression = stepExpression;
    }
};

struct IfStatement
{
    Expression m_ifExpression;
    QString m_label;

    IfStatement(const Expression &ifExpression, const QString &label)
    {
        m_ifExpression = ifExpression;
        m_label = label;
    }
};


class ThreeAddressCodeGenerator
{
public:

    void generate(const QList<Token> &tokenList);

    QString threeAddressCode() const;

    QString error() const;

private:

    void prepareToCodeGeneration(const QList<Token> &tokenList);
    void deleteIdentifierDeclaration(int index);
    void generateCode(QList<Token>::iterator &currentToken);
    void parseAssignmentStatement(QList<Token>::iterator &currentToken);
    void parseBeginForStatement(QList<Token>::iterator &currentToken);
    void parseEndForStatement(QList<Token>::iterator &currentToken);
    void parseBeginIfStatement(QList<Token>::iterator &currentToken);
    void parseEndIfStatement(QList<Token>::iterator &currentToken);

    void addAssignmentToThreeAddressCode(const Token &token, const Expression &expression);
    void addTriade(const QString &result, const QString &first, const QString &second, const QString &third);
    void addTriade(const QString &first, const QString &second, const QString &third);
    void addTriade(const QString &result, const QString &first, const QString &second, const Expression &third);
    void addTriade(const QString &first, const QString &second, const Expression &third);
    void addGoto(const QString &reason, const QString &label);
    void addLabel(const QString &label);

    Expression getNextExpression(QList<Token>::iterator &currentToken);

    QList<Token> m_tokenList;
    QString m_threeAddressCode;
    QList<ForStatement> m_forStatementStack;
    QList<IfStatement> m_ifStatementStack;
    int m_labelCount;

    QString m_error;

};

#endif // THREEADDRESSCODEGENERATOR_H
