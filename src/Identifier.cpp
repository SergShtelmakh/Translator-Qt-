#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier)
{
    name = newIdentifier.getName();
    positionsList = newIdentifier.getPositionsList();
}

Identifier::Identifier(QString newName, QPoint newPosition)
{
    name = newName;
    positionsList.append(newPosition);
}

QPoint Identifier::getFirstPosition()
{
    return positionsList.first();
}

void Identifier::addPosition(QPoint newPosition)
{
    positionsList.append(newPosition);
}

QList<QPoint> Identifier::getPositionsList() const
{
    return positionsList;
}

QString Identifier::getName() const
{
    return name;
}

QString IdentifierPositionsCountToString(const Identifier &identifier)
{
    return QString("%1").arg(identifier.getPositionsList().count());
}

QString IdentifierPositionsToString(const Identifier &identifier)
{
    QString allPositionString;
    foreach (QPoint position, identifier.getPositionsList())
        allPositionString += QString("(%1:%2) ").arg(position.x()).arg(position.y());

    return allPositionString;
}
