#include "ThreeAddressCodeGenerator.h"
#include "Expression.h"
#include "ErrorGenerator.h"

void ThreeAddressCodeGenerator::generate(const QList<Token> &tokenList)
{
    this->prepareToCodeGeneration(tokenList);
    QList<Token>::iterator currentToken = m_tokenList.begin();
    this->generateCode(currentToken);
}

void ThreeAddressCodeGenerator::prepareToCodeGeneration(const QList<Token> &tokenList)
{
    m_labelCount = 0;
    m_tokenList = tokenList;
    m_threeAddressCode.clear();
    m_error.clear();
    int index = 0;
    while (index < m_tokenList.length()) {
        if (m_tokenList[index].lexeme() == "DIM") {
            deleteIdentifierDeclaration(index);
        } else if (m_tokenList[index].category() == Token::SPACE_CATEGORY) {
            m_tokenList.removeAt(index);
        } else {
            index++;
        }
    }
}

void ThreeAddressCodeGenerator::deleteIdentifierDeclaration(int index)
{
    while (m_tokenList[index].category() != Token::LINE_FEED_CATEGORY) {
        m_tokenList.removeAt(index);
    }
    m_tokenList.removeAt(index);
}

void ThreeAddressCodeGenerator::generateCode(QList<Token>::iterator &currentToken)
{
    while (currentToken != m_tokenList.end()) {
        if (currentToken->category() == Token::IDENTIFIER_CATEGORY) {
            this->parseAssignmentStatement(currentToken);
        } else if (currentToken->lexeme() == "FOR") {
            this->parseBeginForStatement(currentToken);
        } else if (currentToken->lexeme() == "IF") {
            this->parseBeginIfStatement(currentToken);
        } else if (currentToken->lexeme() == "NEXT"){
            this->parseEndForStatement(currentToken);
        } else if (currentToken->lexeme() == "END"){
            this->parseEndIfStatement(currentToken);
        } else {
            ++currentToken;
        }
    }
}

void ThreeAddressCodeGenerator::parseAssignmentStatement(QList<Token>::iterator &currentToken)
{
    // currentToken
    //  |
    // <ID> = <EXP>
    Token idToken = *currentToken;
    Expression expression = getNextExpression(currentToken += 2);
    if (idToken.type() != expression.result().type()) {
        m_error += ErrorGenerator::incorrectTypeToAssignment(idToken);
        return;
    }
    this->addAssignmentToThreeAddressCode(idToken, expression);

    ++currentToken;
}

void ThreeAddressCodeGenerator::parseBeginForStatement(QList<Token>::iterator &currentToken)
{
    // currentToken
    //  |
    // FOR <ID> = <EXP> TO <EXP> (STEP <EXP>)?
    Token idToken = *(++currentToken);

    //    currentToken
    //       |
    // FOR <ID> = <EXP> TO <EXP> (STEP <EXP>)?
    Expression beginExpression = this->getNextExpression(currentToken +=2);

    //             currentToken
    //                  |
    // FOR <ID> = <EXP> TO <EXP> (STEP <EXP>)?
    Expression endExpression = this->getNextExpression(++currentToken);

    //                        currentToken
    //                             |
    // FOR <ID> = <EXP> TO <EXP> (STEP <EXP>)?
    Expression stepExpression;
    if ((++currentToken)->lexeme() == "STEP") {
        stepExpression = this->getNextExpression(++currentToken);
    }

    QString label = QString("A%1:").arg(m_labelCount++);
    m_forStatementStack.push_front(ForStatement(idToken, beginExpression, endExpression, label, stepExpression));
    this->addAssignmentToThreeAddressCode(idToken, beginExpression);
    this->addLabel(label);

    ++currentToken;
}

void ThreeAddressCodeGenerator::parseEndForStatement(QList<Token>::iterator &currentToken)
{
    ForStatement currentFor = m_forStatementStack.takeFirst();
    // currentToken
    //  |
    // NEXT <ID>
    if ((++currentToken)->lexeme() != currentFor.m_id.lexeme()) {
        m_error += ErrorGenerator::incorrectIdentifier(*currentToken);
    }

    if (currentFor.m_stepExpression.codeList().isEmpty()) {
        this->addTriade(currentFor.m_id.lexeme(), "+", currentFor.m_id.lexeme(), "1");
    } else {
        this->addTriade(currentFor.m_id.lexeme(), "+", currentFor.m_id.lexeme(), currentFor.m_stepExpression);
    }
    this->addTriade("<",  currentFor.m_id.lexeme(), currentFor.m_endExpression);
    this->addGoto("true", currentFor.m_label);

    ++currentToken;
}

void ThreeAddressCodeGenerator::parseBeginIfStatement(QList<Token>::iterator &currentToken)
{
    // currentToken
    // |
    // IF <EXP> THEN
    Expression ifExpression = this->getNextExpression(++currentToken);

    QString label = QString("A%1:").arg(m_labelCount++);
    m_ifStatementStack.push_front(IfStatement(ifExpression, label));
    m_threeAddressCode += ifExpression.codeList();
    this->addGoto("false", label);

    currentToken += 2;
}

void ThreeAddressCodeGenerator::parseEndIfStatement(QList<Token>::iterator &currentToken)
{
    // currentToken
    //  |
    // END IF
    IfStatement currentIf = m_ifStatementStack.takeFirst();
    this->addLabel(currentIf.m_label);

    currentToken +=2;
}

void ThreeAddressCodeGenerator::addAssignmentToThreeAddressCode(const Token &token, const Expression &expression)
{
    m_threeAddressCode += expression.codeList();
    m_threeAddressCode += token.lexeme() + " := " + expression.result().lexeme() + "\n";
}

void ThreeAddressCodeGenerator::addTriade(const QString &result, const QString &first, const QString &second, const QString &third)
{
    m_threeAddressCode += result + " := (" + first + "," + second + "," + third + ")\n";
}

void ThreeAddressCodeGenerator::addTriade(const QString &first, const QString &second, const QString &third)
{
    m_threeAddressCode += "(" + first + "," + second + "," + third + ")\n";
}

void ThreeAddressCodeGenerator::addTriade(const QString &result, const QString &first, const QString &second, const Expression &third)
{
    m_threeAddressCode += third.codeList();
    this->addTriade(result, first, second, third.result().lexeme());
}

void ThreeAddressCodeGenerator::addTriade(const QString &first, const QString &second, const Expression &third)
{
    m_threeAddressCode += third.codeList();
    this->addTriade(first, second, third.result().lexeme());
}

void ThreeAddressCodeGenerator::addGoto(const QString &reason, const QString &label)
{
    m_threeAddressCode += "If " + reason + " goto " + label + "\n";
}

void ThreeAddressCodeGenerator::addLabel(const QString &label)
{
    m_threeAddressCode += label + "\n";
}

Expression ThreeAddressCodeGenerator::getNextExpression(QList<Token>::iterator &currentToken)
{
    QList<Token> currentList;
    bool keyWordButNotTrueFalse = false;
    while ((currentToken != m_tokenList.end())
           &&(currentToken->category() != Token::LINE_FEED_CATEGORY)
           &&(!keyWordButNotTrueFalse)) {
        currentList.push_back(*currentToken);
        ++currentToken;
        keyWordButNotTrueFalse = (currentToken->category() == Token::KEYWORD_CATEGORY)
                                &&(currentToken->lexeme() != "TRUE")
                                &&(currentToken->lexeme() != "FALSE");
    }

    Expression exp = Expression(currentList);
    m_error += exp.error();
    return exp;
}

QString ThreeAddressCodeGenerator::errorText() const
{
    return m_error;
}

QString ThreeAddressCodeGenerator::threeAddressCode() const
{
    return m_threeAddressCode;
}

