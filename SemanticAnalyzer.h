#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "Block.h"
#include "Token.h"
#include "Identifier.h"
#include <QList>

class SemanticAnalyzer
{

public:

    SemanticAnalyzer();
    ~SemanticAnalyzer();
    void analyze (const QList<Token> &tokenList);

private:

    void makeBlocks (const QList<Token> &tokenList);
    void findIdentifiersDeclaration (const QList<Token> &tokenList);
    Block* getBlockByLineNumber(const int lineNumber);

    Block* m_mainBlock;
    QList<Identifier> m_identifiersList;

};

#endif // SEMANTICANALYZER_H
