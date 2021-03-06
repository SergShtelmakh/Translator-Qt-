#include "Expression.h"
#include "ErrorGenerator.h"
#include "Token.h"

Expression::Expression(QList<Token> &tokenList)
{
    this->makePolishNotation(tokenList);
    this->makeThreeAddressCode();
}

Expression::Expression()
{}

void Expression::makePolishNotation(QList<Token> &tokenList)
{
    // Expression:
    // 2+6*87+(8-9)*(4-9/7)

    // PolishNotation:
    // 2 6 87 * + 8 9 - 4 9 7 / - * +

    while (!tokenList.isEmpty()) {
        Token currentToken = tokenList.takeFirst();
        switch (currentToken.category()) {
        case Token::IDENTIFIER_CATEGORY: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::CHAR_TOKEN_CATEGORY: {
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
        case Token::NUMBER_LITERAL_CATEGORY: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::STRING_LITERAL_CATEGORY: {
            m_polishNotation.push_back(currentToken);
            break;
        }
        case Token::KEYWORD_CATEGORY: {
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

void Expression::makeThreeAddressCode()
{

    // PolishNotation:
    // 2 6 87 * + 8 9 - 4 9 7 / - * +

    // ThreeAddressCode:
    // t0 := (*, 6, 87)
    // t1 := (+, 2, t0)
    // t2 := (-, 8, 9)
    // t3 := (/, 9, 7)
    // t4 := (-, 4, t3)
    // t5 := (*, t2, t4)
    // t6 := (+, t1, t5)

    while (!m_polishNotation.isEmpty()) {
        Token currentToken = m_polishNotation.takeFirst();
        if (!isOperation(currentToken)) {
            m_stack.push_front(currentToken);
        } else {
            Token secondToken = m_stack.takeFirst();
            Token firstToken = m_stack.takeFirst();
            Token newToken = Token(QString("t%1").arg(m_codeList.size()), Token::IDENTIFIER_CATEGORY);
            Expression::Type newType = resultType(currentToken, firstToken, secondToken);
            if (newType == Expression::NONE_TYPE) {
                m_error +=  ErrorGenerator::cantDoOperation(currentToken, firstToken, secondToken);
                return;
            }
            newToken.setType(newType);
            m_codeList.push_back(QString("%1 := (%2,%3,%4)")
                                 .arg(newToken.lexeme())
                                 .arg(currentToken.lexeme())
                                 .arg(firstToken.lexeme())
                                 .arg(secondToken.lexeme()));
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



