#include "Block.h"

Block::Block() :
    m_type(MAIN),
    m_scopeBeginLineNumber(0),
    m_scopeEndLineNumber(INT_MAX)
{
    m_parent = NULL;
}

Block::Block(Block::BlockType type, int startLineNumber, Block *parent) :
    m_type(type),
    m_scopeBeginLineNumber(startLineNumber),
    m_parent(parent)
{}

Block::~Block()
{
    while (!m_identifiers.isEmpty()) {
        Identifier *id = m_identifiers.takeFirst();
        delete id;
    }
    while (!m_children.isEmpty()) {
        Block* block = m_children.takeFirst();
        delete block;
    }
}

void Block::addChildBlock(Block *child)
{
    m_children.push_back(child);
}

void Block::setScopeEndLineNumber(const int endLine)
{
    m_scopeEndLineNumber = endLine;
}

bool Block::isIdentifierDeclared(Identifier *identifier)
{
    foreach (Identifier *currentId, m_identifiers) {
        if (*identifier == *currentId) {
            return true;
        }
    }
    return false;
}

void Block::addIdentifier(Identifier *identifier)
{
    m_identifiers.push_back(identifier);
}

Block *Block::parent() const
{
    return m_parent;
}

int Block::scopeBeginLineNumber() const
{
    return m_scopeBeginLineNumber;
}

int Block::scopeEndLineNumber() const
{
    return m_scopeEndLineNumber;
}

QVector<Block *> Block::children() const
{
    return m_children;
}

QVector<Identifier*> Block::identifiers() const
{
    return m_identifiers;
}

Block::BlockType Block::type() const
{
    return m_type;
}

QString BlockTypeToString(Block::BlockType type)
{
    switch (type) {
    case Block::IF:
        return "IF";
    case Block::FOR:
        return "FOR";
    default:
        return QString();
    }
}
