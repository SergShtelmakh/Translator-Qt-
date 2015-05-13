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

    QString errorText() const;
    Block *mainBlock() const;

private:

    void makeBlocks (const QList<Token> &tokenList);
    void findIdentifiersDeclaration (const QList<Token> &tokenList);
    void checkIdentifiersScope(const QList<Token> &tokenList);
    Block *getBlockByLineNumber(const int lineNumber);
    bool isIdentifierDeclarate(const Token &identifier);
    void addError(QString errorText);
    void prepareToAnalysis(const QList<Token> &tokenList);

    Block *m_mainBlock;
    QString m_errorText;

};

bool AtRange(int beginRange, int endRange, int value);

#endif // SEMANTICANALYZER_H