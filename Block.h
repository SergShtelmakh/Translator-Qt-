/*!
 * \file Block.h
 * \date 2015/06/13
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <QVector>
#include "Identifier.h"

/*!
 * @brief This class used to make hierarhic structure of progrm.
 */

class Block
{

public:

    enum BlockType {
        FOR_BLOCK,
        IF_BLOCK,
        MAIN_BLOCK
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
    QVector<Block *> childrenBlocks() const;
    QVector<Identifier *> identifiers() const;
    BlockType type() const;

private:

    BlockType m_type;
    int m_scopeBeginLineNumber;
    int m_scopeEndLineNumber;
    QVector<Block *> m_childrenBlocks;
    Block *m_parent;
    QVector<Identifier *> m_identifiers;

};

QString BlockTypeToString(Block::BlockType type);

#endif // BLOCK_H
