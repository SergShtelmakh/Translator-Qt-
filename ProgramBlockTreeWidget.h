#ifndef PROGRAMBLOCKTREEWIDGET_H
#define PROGRAMBLOCKTREEWIDGET_H

#include <QTreeWidget>
#include "Block.h"

class ProgramBlockTreeWidget : public QTreeWidget
{

public:

    explicit ProgramBlockTreeWidget(QWidget *parent = 0);
    ~ProgramBlockTreeWidget();
    void setData(Block* mainBlock);

private:
    enum NodeType {
        BLOCK,
        IDENTIFIER
    };

    void addNode(QTreeWidgetItem *currentNode, QVector<Identifier *> &identifiers, QVector<Block *> &childBlocks);
    NodeType getNextNodeType(QVector<Identifier *> &identifiers, QVector<Block *> &childBlocks);
};

#endif // PROGRAMBLOCKTREEWIDGET_H
