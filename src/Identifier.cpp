#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier)
{
    m_name = newIdentifier.name();
    m_positionsList = newIdentifier.positionsList();
}

Identifier::Identifier(QString newName, QPoint newPosition)
{
    m_name = newName;
    m_positionsList.append(newPosition);
}

QPoint Identifier::getFirstPosition()
{
    return m_positionsList.first();
}

void Identifier::addPosition(QPoint newPosition)
{
    m_positionsList.append(newPosition);
}

bool Identifier::operator==(const Identifier &id1) const
{
    return m_name == id1.name();
}

QList<QPoint> Identifier::positionsList() const
{
    return m_positionsList;
}

QString Identifier::name() const
{
    return m_name;
}

QString IdentifierPositionsCountToString(const Identifier &identifier)
{
    return QString("%1").arg(identifier.positionsList().count());
}

QString IdentifierPositionsToString(const Identifier &identifier)
{
    QString allPositionString;
    foreach (QPoint position, identifier.positionsList())
        allPositionString += QString("(%1:%2) ").arg(position.x()).arg(position.y());

    return allPositionString;
}
