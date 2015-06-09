#include "SemanticAnalyzer.h"
#include <iterator>
#include <QStringList>
#include "ErrorGenerator.h"

SemanticAnalyzer::SemanticAnalyzer() : m_mainBlock(NULL), m_identifierCount(0)
{}

SemanticAnalyzer::~SemanticAnalyzer()
{
    delete m_mainBlock;
}

void SemanticAnalyzer::analyze(QList<Token> &tokenList)
{
    if (tokenList.isEmpty()) {
        return;
    }
    this->prepareToAnalysis(tokenList);
    this->makeBlocks(tokenList);
    this->findIdentifiersDeclaration(tokenList);
    this->checkIdentifiersScope(tokenList);
}

void SemanticAnalyzer::makeBlocks(const QList<Token> &tokenList)
{
    Block *currentBlock = m_mainBlock;
    QList<Token>::const_iterator tokenIterator = tokenList.begin();
    while(tokenIterator != tokenList.end()) {
        Token currentToken = *tokenIterator;
        if (currentToken.lexeme() == "FOR") {
            Block *newChildBlock = new Block(Block::FOR, GetTokenLineNumber(currentToken), currentBlock);
            currentBlock->addChildBlock(newChildBlock);
            currentBlock = newChildBlock;
        } else if (currentToken.lexeme() == "IF"){
            Block *newChildBlock = new Block(Block::IF, GetTokenLineNumber(currentToken), currentBlock);
            currentBlock->addChildBlock(newChildBlock);
            currentBlock = newChildBlock;
        } else if ((currentToken.lexeme() == "END")||(currentToken.lexeme() == "NEXT")){
            currentBlock->setScopeEndLineNumber(GetTokenLineNumber(currentToken));
            currentBlock = currentBlock->parent();
            // jump through <END><space><IF> or <NEXT><space><ID>
            tokenIterator += 2;
        }
        tokenIterator++;
    }
}

void SemanticAnalyzer::findIdentifiersDeclaration(QList<Token> &tokenList)
{
    int index = 0;
    while(index < tokenList.size()) {
        Token currentToken = tokenList[index];
        if (currentToken.lexeme() == "DIM") {
            Block *currentBlock = getBlockByLineNumber(GetTokenLineNumber(currentToken));
            Token *identifierToken = &tokenList[index+2];
            const Token identifierTypeToken = tokenList[index+6];
            Identifier *newId = new Identifier(identifierToken->lexeme(),
                                          StringToType(identifierTypeToken.lexeme()),
                                          GetTokenLineNumber(currentToken),
                                          currentBlock->scopeEndLineNumber());

            if (currentBlock->isIdentifierDeclared(newId)) {
                addError(ErrorGenerator::redeclarationOfIdentifier(*identifierToken));
            } else {
                newId->setCode(m_identifierCount++);
                currentBlock->addIdentifier(newId);
                identifierToken->setIdentifier(newId);
            }
        } else {
            if (currentToken.category() == Token::categoryIdentifier) {
                tokenList[index].setType(getIdentifierByToken(currentToken)->type());
            }
        }
        ++index ;
    }
}

void SemanticAnalyzer::checkIdentifiersScope(const QList<Token> &tokenList)
{
    QList<Token>::const_iterator tokenIterator = tokenList.begin();
    while(tokenIterator != tokenList.end()) {
        Token currentToken = *tokenIterator;
        if (currentToken.category() == Token::categoryIdentifier) {
            if (!isIdentifierDeclarate(currentToken)) {
                addError(ErrorGenerator::undeclaratedIdentifierError(currentToken));
            }
        }
        tokenIterator++;
    }
}

Block *SemanticAnalyzer::getBlockByLineNumber(const int lineNumber)
{
    Block *currentBlock = m_mainBlock;
    for (;;) {
        Block *foundChildBlock = NULL;
        foreach (Block *currentChildBlock, currentBlock->children()) {
            if (AtRange(currentChildBlock->scopeBeginLineNumber(),
                        currentChildBlock->scopeEndLineNumber(),
                        lineNumber)) {
                foundChildBlock = currentChildBlock;
            }
        }
        if (!foundChildBlock) {
            return currentBlock;
        } else {
            currentBlock = foundChildBlock;
        }
    }
}

bool SemanticAnalyzer::isIdentifierDeclarate(const Token &identifierToken)
{
    Block* currentBlock = getBlockByLineNumber(GetTokenLineNumber(identifierToken));
    while (currentBlock) {
        foreach (Identifier *currentIdentifier, currentBlock->identifiers()) {
            if (currentIdentifier->lexeme() == identifierToken.lexeme()) {
                if (AtRange(currentIdentifier->scopeBeginLineNumber(),
                            currentIdentifier->scopeEndLineNumber(),
                            GetTokenLineNumber(identifierToken))) {
                    return true;
                }
            }
        }
        currentBlock = currentBlock->parent();
    }
    return false;
}

void SemanticAnalyzer::addError(QString errorText)
{
    m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + errorText + "\n";
}

void SemanticAnalyzer::prepareToAnalysis(const QList<Token> &tokenList)
{
    if (m_mainBlock)
        delete m_mainBlock;

    m_mainBlock = new Block();
    m_mainBlock->setScopeEndLineNumber(tokenList.last().position().y());
    m_errorText.clear();
    m_identifierCount = 0;
}

Identifier *SemanticAnalyzer::getIdentifierByToken(const Token &identifierToken)
{
    Block* currentBlock = getBlockByLineNumber(GetTokenLineNumber(identifierToken));
    while (currentBlock) {
        foreach (Identifier *currentIdentifier, currentBlock->identifiers()) {
            if (currentIdentifier->lexeme() == identifierToken.lexeme()) {
                if (AtRange(currentIdentifier->scopeBeginLineNumber(),
                            currentIdentifier->scopeEndLineNumber(),
                            GetTokenLineNumber(identifierToken))) {
                    return currentIdentifier;
                }
            }
        }
        currentBlock = currentBlock->parent();
    }
    return NULL;
}

Block *SemanticAnalyzer::mainBlock() const
{
    return m_mainBlock;
}

QString SemanticAnalyzer::errorText() const
{
    return m_errorText;
}

bool AtRange(int beginRange, int endRange, int value)
{
    return (beginRange <= value)&&(endRange >= value);
}

