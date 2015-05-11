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
    Block(BlockType type, int startLineNumber, Block *parent);
    ~Block();
    void addChildBlock(Block *child);
    void setEndLine(int endLine);
    bool isIdentifierDeclared(const Identifier &identifier);
    void addIdentifier(const Identifier &identifier);

    Block *parent() const;
    int startLineNumber() const;
    int endLineNumber() const;
    QVector<Block *> children() const;

private:

    BlockType m_type;
    int m_startLineNumber;
    int m_endLineNumber;
    QVector<Block *> m_children;
    Block *m_parent;
    QVector<Identifier> m_identifiers;

};

#endif // BLOCK_H
