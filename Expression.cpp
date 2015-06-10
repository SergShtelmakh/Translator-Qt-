#include "Expression.h"
#include "Token.h"

Expression::Expression(QList<Token> &tokenList)
{
    makePolishNotation(tokenList);
    makeCode();
}

void Expression::makePolishNotation(QList<Token> &tokenList)
{
    while (!tokenList.isEmpty()) {
        Token currentToken = tokenList.takeFirst();
        switch (currentToken.category()) {
        case Token::categoryIdentifier: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::categoryCharToken: {
            if (currentToken.lexeme() == "(") {
                m_stack.push_front(currentToken);
            } else if (currentToken.lexeme() == ")") {
                while (m_stack.first().lexeme() != "(") {
                    m_polishNotation.push_back(m_stack.takeFirst());
                    if (m_stack.isEmpty()) {
                        return;
                    }
                }
                m_stack.takeFirst();
            } else if (isOperation(currentToken)) {
                if (!m_stack.isEmpty()) {
                    if (GetOperationPriority(currentToken) <= GetOperationPriority(m_stack.first())) {
                        m_polishNotation.push_back(m_stack.takeFirst());
                    }
                }
                m_stack.push_front(currentToken);
            }
            break;
        }
        case Token::categoryNumberLiteral: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::categoryStringLiteral: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::categoryKeyword: {
            if (currentToken.lexeme() == "TRUE" || currentToken.lexeme() == "FALSE")
                m_polishNotation.push_back(currentToken);
            break;
        }
        default:
            break;
        }

    }
    while (!m_stack.isEmpty()) {
        m_polishNotation.push_back(m_stack.takeFirst());
    }
}

void Expression::makeCode()
{
    while (!m_polishNotation.isEmpty()) {
        Token currentToken = m_polishNotation.takeFirst();
        if (!isOperation(currentToken)) {
            m_stack.push_front(currentToken);
        } else {
                Token secondToken = m_stack.takeFirst();
                Token firstToken = m_stack.takeFirst();
                Token newToken = Token(QString("t%1").arg(m_codeList.size()),Token::categoryIdentifier);
                Expression::Type newType = resultType(currentToken,firstToken,secondToken);
                if (newType == Expression::NONE) {
                    m_error += "can't do operation " + currentToken.lexeme() + " with " + firstToken.lexeme() + " " + secondToken.lexeme() + "\n";
                    return;
                }
                newToken.setType(newType);
                m_codeList.push_back(QString("%1 := (%2,%3,%4)").arg(newToken.lexeme()).arg(currentToken.lexeme()).arg(firstToken.lexeme()).arg(secondToken.lexeme()));
                m_stack.push_front(newToken);
        }
    }
}

QString Expression::error() const
{
    return m_error;
}

QString Expression::codeList() const
{
    QString result;
    foreach (QString str, m_codeList) {
        result += str + "\n";
    }
    return result;
}

Token Expression::result() const
{
    return m_stack.first();
}



