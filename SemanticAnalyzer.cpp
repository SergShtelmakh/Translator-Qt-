#include "SemanticAnalyzer.h"
#include <iterator>
#include <QStringList>
#include "ErrorGenerator.h"

SemanticAnalyzer::SemanticAnalyzer() : m_mainBlock(NULL)
{}

SemanticAnalyzer::~SemanticAnalyzer()
{
    delete m_mainBlock;
}

void SemanticAnalyzer::analyze(const QList<Token> &tokenList)
{
    prepareToAnalysis();
    makeBlocks(tokenList);
    findIdentifiersDeclaration(tokenList);
    checkIdentifiersScope(tokenList);
    if (!m_errorText.isEmpty()) {
        return;
    }
}

void SemanticAnalyzer::makeBlocks(const QList<Token> &tokenList)
{
    Block *currentBlock = m_mainBlock;
    QList<Token>::const_iterator tokenIterator = tokenList.begin();
    while(tokenIterator != tokenList.end()) {
        Token currentToken = *tokenIterator;
        if (currentToken.lexeme() == "FOR") {
            Block *newBlock = new Block(Block::FOR, currentToken.position().y(), currentBlock);
            currentBlock->addChildBlock(newBlock);
            currentBlock = newBlock;
        } else if (currentToken.lexeme() == "IF"){
            Block *newBlock = new Block(Block::IF, currentToken.position().y(), currentBlock);
            currentBlock->addChildBlock(newBlock);
            currentBlock = newBlock;
        } else if ((currentToken.lexeme() == "END")||(currentToken.lexeme() == "NEXT")){
            currentBlock->setEndLine(currentToken.position().y());
            currentBlock = currentBlock->parent();
            // jump through <END><space><IF> or <NEXT><space><ID>
            tokenIterator += 2;
        }
        tokenIterator++;
    }
}

void SemanticAnalyzer::findIdentifiersDeclaration(const QList<Token> &tokenList)
{
    QList<Token>::const_iterator tokenIterator = tokenList.begin();
    while(tokenIterator != tokenList.end()) {
        Token currentToken = *tokenIterator;
        if (currentToken.lexeme() == "DIM") {
            int tokenLineNumber = currentToken.position().y();
            Block *currentBlock = getBlockByLineNumber(tokenLineNumber);
            const Token identifierToken = *(tokenIterator + 2);
            const Token identifierTypeToken = *(tokenIterator + 6);
            Identifier *newId = new Identifier(identifierToken.lexeme(),
                                          MakeIdentifierType(identifierTypeToken.lexeme()),
                                          tokenLineNumber,
                                          currentBlock->endLineNumber());
            if (currentBlock->isIdentifierDeclared(*newId)) {
                addError(ErrorGenerator::redeclarationOfIdentifier(identifierToken));
            } else {
                currentBlock->addIdentifier(*newId);
                m_identifiersList.append(newId);
            }
        }
        tokenIterator++;
    }
}

void SemanticAnalyzer::checkIdentifiersScope(const QList<Token> &tokenList)
{
    QList<Token>::const_iterator tokenIterator = tokenList.begin();
    while(tokenIterator != tokenList.end()) {
        Token currentToken = *tokenIterator;
        if (currentToken.tokenCategory() == Token::categoryIdentifier) {
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
            if ((currentBlock->startLineNumber() <= lineNumber)&&(currentBlock->endLineNumber() >= lineNumber))
                foundChildBlock = currentChildBlock;
        }
        if (!foundChildBlock) {
            return currentBlock;
        } else {
            currentBlock = foundChildBlock;
        }
    }
}

bool SemanticAnalyzer::isIdentifierDeclarate(Token identifier)
{
    foreach (Identifier *currentIdentifier, m_identifiersList) {
        if (currentIdentifier->lexeme() == identifier.lexeme()) {
            if((currentIdentifier->scopeBeginLineNumber() <= identifier.position().y())&&(currentIdentifier->scopeEndLineNumber() >= identifier.position().y())) {
                return true;
            }
        }
    }
    return false;
}

void SemanticAnalyzer::addError(QString errorText)
{
    m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + errorText + "\n";
}

void SemanticAnalyzer::prepareToAnalysis()
{
    if (m_mainBlock)
        delete m_mainBlock;

    m_mainBlock = new Block();
    m_errorText.clear();
}

Block *SemanticAnalyzer::mainBlock() const
{
    return m_mainBlock;
}

QString SemanticAnalyzer::errorText() const
{
    return m_errorText;
}



