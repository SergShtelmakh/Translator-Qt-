#include "Block.h"

Block::Block() :
    m_type(MAIN),
    m_startLineNumber(0),
    m_endLineNumber(INT_MAX)
{
    m_parent = NULL;
}

Block::Block(Block::BlockType type, int startLineNumber, Block *parent) :
    m_type(type),
    m_startLineNumber(startLineNumber),
    m_parent(parent)
{}

Block::~Block()
{
    while (!m_children.isEmpty()) {
        Block* block = m_children.takeFirst();
        delete block;
    }
}

void Block::addChildBlock(Block *child)
{
    m_children.push_back(child);
}

void Block::setEndLine(int endLine)
{
    m_endLineNumber = endLine;
}

bool Block::isIdentifierDeclared(const Identifier &identifier)
{
    if (m_identifiers.contains(identifier))
        return true;
    return false;
}

void Block::addIdentifier(const Identifier &identifier)
{
    m_identifiers.push_back(identifier);
}

Block *Block::parent() const
{
    return m_parent;
}

int Block::startLineNumber() const
{
    return m_startLineNumber;
}

int Block::endLineNumber() const
{
    return m_endLineNumber;
}

QVector<Block *> Block::children() const
{
    return m_children;
}



