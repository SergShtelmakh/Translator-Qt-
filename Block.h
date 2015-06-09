#ifndef BLOCK_H
#define BLOCK_H

#include <QVector>
#include "Identifier.h"

class Block
{

public:

    enum BlockType {
        FOR,
        IF,
        MAIN
    };

    Block();
    Block(BlockType type, int scopeBeginLineNumber, Block *parent);
    ~Block();
    void addChildBlock(Block *child);
    void setScopeEndLineNumber(const int endLine);
    bool isIdentifierDeclared(Identifier *identifier);
    void addIdentifier(Identifier *identifier);

    Block *parent() const;
    int scopeBeginLineNumber() const;
    int scopeEndLineNumber() const;
    QVector<Block *> children() const;
    QVector<Identifier *> identifiers() const;
    BlockType type() const;

private:

    BlockType m_type;
    int m_scopeBeginLineNumber;
    int m_scopeEndLineNumber;
    QVector<Block *> m_children;
    Block *m_parent;
    QVector<Identifier *> m_identifiers;

};

QString BlockTypeToString(Block::BlockType type);

#endif // BLOCK_H
