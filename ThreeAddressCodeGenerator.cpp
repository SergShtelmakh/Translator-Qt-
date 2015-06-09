#include "ThreeAddressCodeGenerator.h"
#include "Expression.h"

ThreeAddressCodeGenerator::ThreeAddressCodeGenerator()
{
}

ThreeAddressCodeGenerator::~ThreeAddressCodeGenerator()
{

}

void ThreeAddressCodeGenerator::generate(const QList<Token> &tokenList)
{
    prepare(tokenList);
    QList<Token>::iterator currentToken = m_tokenList.begin();
    parseStatement(currentToken);
}

void ThreeAddressCodeGenerator::prepare(const QList<Token> &tokenList)
{
    m_labelCount = 0;
    m_tokenList = tokenList;
    m_threeAddressCode.clear();
    m_error.clear();
    int index = 0;
    while (index < m_tokenList.length()) {
        if (m_tokenList[index].lexeme() == "DIM") {
            deleteIdentifierDeclaration(index);
        } else if (m_tokenList[index].category() == Token::categorySpace) {
            m_tokenList.removeAt(index);
        } else {
            index++;
        }
    }
}

void ThreeAddressCodeGenerator::deleteIdentifierDeclaration(int index)
{
    while (m_tokenList[index].category() != Token::categoryLineFeed) {
        m_tokenList.removeAt(index);
    }
    m_tokenList.removeAt(index);
}

void ThreeAddressCodeGenerator::parseStatement(QList<Token>::iterator &currentToken)
{
    while (currentToken != m_tokenList.end()) {
    if (currentToken->category() == Token::categoryIdentifier) {
        parseAssignmentStatement(currentToken);
    } else if (currentToken->lexeme() == "FOR") {
        parseForStatement(currentToken);
    } else if (currentToken->lexeme() == "IF") {
        parseIfStatement(currentToken);
    } else if (currentToken->lexeme() == "NEXT"){
        ForStatement currentFor = m_forStatementStack.takeFirst();
        if ((++currentToken)->lexeme() != currentFor.m_id.lexeme()) {
            m_error += QString("Incorrect id at (%1,%2)\n").arg(currentToken->position().x()).arg(currentToken->position().y());
        }
        ++currentToken;
        if (currentFor.m_stepExpression.codeList().isEmpty()) {
            m_threeAddressCode += currentFor.m_id.lexeme() + " := (+," + currentFor.m_id.lexeme() + ",1)\n";
        } else {
            m_threeAddressCode += currentFor.m_stepExpression.codeList();
            m_threeAddressCode += currentFor.m_id.lexeme() + " := (+," + currentFor.m_id.lexeme() + "," + currentFor.m_stepExpression.result().lexeme() + ")\n";
        }
        m_threeAddressCode += currentFor.m_endExpression.codeList();
        m_threeAddressCode += "(<," + currentFor.m_id.lexeme() + "," + currentFor.m_endExpression.result().lexeme() + ")\n";
        m_threeAddressCode += "IF true goto " + currentFor.m_label + "\n";
    } else if (currentToken->lexeme() == "END"){
        IfStatement currentIf = m_ifStatementStack.takeFirst();
        m_threeAddressCode += currentIf.m_label + "\n";
        currentToken +=2;
    } else {
        ++currentToken;
    }
    }
}

void ThreeAddressCodeGenerator::parseAssignmentStatement(QList<Token>::iterator &currentToken)
{
    Token idToken = *currentToken;
    currentToken += 2;
    Expression expression = parseExpression(currentToken);
    m_threeAddressCode += expression.codeList();
    if (idToken.type() != expression.result().type()) {
        m_error += QString("Incorrect type to assignment at (%1,%2)\n").arg(idToken.position().x()).arg(idToken.position().y());
        return;
    }
    m_threeAddressCode += idToken.lexeme() + " := " + expression.result().lexeme() + "\n";
}

void ThreeAddressCodeGenerator::parseForStatement(QList<Token>::iterator &currentToken)
{
    Token idToken = *(++currentToken);
    Expression beginExpression = parseExpression(++currentToken);
    Expression endExpression = parseExpression(++currentToken);

    Expression stepExpression;
    if ((++currentToken)->lexeme() == "STEP") {
        stepExpression = parseExpression(++currentToken);
        ++currentToken;
    }
    QString label = QString("A%1:").arg(m_labelCount++);
    m_forStatementStack.push_front(ForStatement(idToken, beginExpression, endExpression, label, stepExpression));
    m_threeAddressCode += beginExpression.codeList();
    m_threeAddressCode += idToken.lexeme() + " := " + beginExpression.result().lexeme() + "\n";
    m_threeAddressCode += label + "\n";
}

void ThreeAddressCodeGenerator::parseIfStatement(QList<Token>::iterator &currentToken)
{
    Expression ifExpression = parseExpression(++currentToken);
    currentToken += 2;
    QString label = QString("A%1:").arg(m_labelCount++);
    m_ifStatementStack.push_front(IfStatement(ifExpression, label));
    m_threeAddressCode += ifExpression.codeList();
    m_threeAddressCode += "If false goto " + label + "\n";

}

Expression ThreeAddressCodeGenerator::parseExpression(QList<Token>::iterator &currentToken)
{
    QList<Token> currentList;
    bool keyWordButNotTrueFalse = false;
    while ((currentToken != m_tokenList.end())&&(currentToken->category() != Token::categoryLineFeed)&&(!keyWordButNotTrueFalse)) {
        currentList.push_back(*currentToken);
        ++currentToken;
        keyWordButNotTrueFalse = (currentToken->category() == Token::categoryKeyword)&&(currentToken->lexeme() != "TRUE")&&(currentToken->lexeme() != "FALSE");
    }
    Expression exp = Expression(currentList);
    m_error += exp.error();
    return exp;
}
QString ThreeAddressCodeGenerator::error() const
{
    return m_error;
}

QString ThreeAddressCodeGenerator::threeAddressCode() const
{
    return m_threeAddressCode;
}

