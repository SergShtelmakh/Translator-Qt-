#include "ProgramBlockTreeWidget.h"
#include <QTreeWidgetItem>
#include <QDebug>

ProgramBlockTreeWidget::ProgramBlockTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    setColumnCount(5);
    QStringList headerLabels;
    headerLabels << "Type" << "Name" << "Code" << "Data type" << "Scope begin" << "Scope end";
    setHeaderLabels(headerLabels);
}

ProgramBlockTreeWidget::~ProgramBlockTreeWidget()
{

}

void ProgramBlockTreeWidget::setData(Block *mainBlock)
{
    if (!mainBlock) {
        return;
    }
    this->clear();
    QStringList rootItemStringList;
    rootItemStringList << "Block";
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(rootItemStringList);
    QVector<Identifier *> identifierVector = mainBlock->identifiers();
    QVector<Block *> childrenVector = mainBlock->children();
    while ((!childrenVector.isEmpty())||(!identifierVector.isEmpty())) {
        addNode(rootItem, identifierVector, childrenVector);
    }
    addTopLevelItem(rootItem);
    this->expandAll();
}

void ProgramBlockTreeWidget::addNode(QTreeWidgetItem *currentNode, QVector<Identifier *> &identifiers, QVector<Block *> &childBlocks)
{
    NodeType nextNodeType = getNextNodeType(identifiers, childBlocks);
    QStringList nodeStringList;

    switch (nextNodeType) {
    case BLOCK: {
        Block *currentChild = childBlocks.takeFirst();
        nodeStringList << "Block"
                       << BlockTypeToString(currentChild->type())
                       << ""
                       << ""
                       << QString("%1").arg(currentChild->scopeBeginLineNumber())
                       << QString("%1").arg(currentChild->scopeEndLineNumber());
        QTreeWidgetItem *currentChildItem = new QTreeWidgetItem(nodeStringList);
        currentNode->addChild(currentChildItem);
        QVector<Identifier *> identifierVector = currentChild->identifiers();
        QVector<Block *> childrenVector = currentChild->children();
        while ((!childrenVector.isEmpty())||(!identifierVector.isEmpty())) {
            addNode(currentChildItem, identifierVector, childrenVector);
        }
        break;
    }
    case IDENTIFIER: {
        Identifier *currentIdentifier = identifiers.takeFirst();
        nodeStringList << "Identifier"
                       << currentIdentifier->lexeme()
                       << QString("#%1").arg(currentIdentifier->code())
                       << TypeToString(currentIdentifier->type())
                       << QString("%1").arg(currentIdentifier->scopeBeginLineNumber())
                       << QString("%1").arg(currentIdentifier->scopeEndLineNumber());
        currentNode->addChild(new QTreeWidgetItem(nodeStringList));
        break;
    }
    default:
        break;
    }
}

ProgramBlockTreeWidget::NodeType ProgramBlockTreeWidget::getNextNodeType(QVector<Identifier *> &identifiers, QVector<Block *> &childBlocks)
{
    if (identifiers.isEmpty()) {
        return BLOCK;
    } else if (childBlocks.isEmpty()) {
        return IDENTIFIER;
    } else {
        Block *currentChild = childBlocks.first();
        Identifier *currentIdentifier = identifiers.first();
        if (currentChild->scopeBeginLineNumber() < currentIdentifier->scopeBeginLineNumber()) {
            return BLOCK;
        } else {
            return IDENTIFIER;
        }
    }
}


