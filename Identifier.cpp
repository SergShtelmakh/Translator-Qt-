#include "Identifier.h"

Identifier::Identifier(const Identifier &newIdentifier) :
    m_name(newIdentifier.name()),
    m_positionsList(newIdentifier.positionsList())
{}

Identifier::Identifier(const QString &name, const QPoint &position) :
    m_name(name),
    m_positionsList()
{
    m_positionsList.append(position);
}

QPoint Identifier::getFirstPosition()
{
    return m_positionsList.first();
}

void Identifier::addPosition(const QPoint &position)
{
    m_positionsList.append(position);
}

bool Identifier::operator==(const Identifier &otherIdentifier) const
{
    return m_name == otherIdentifier.name();
}

QList<QPoint> Identifier::positionsList() const
{
    return m_positionsList;
}

QString Identifier::name() const
{
    return m_name;
}

QString IdentifierPositionsToString(const Identifier &identifier)
{
    QString allPositionString;
    foreach (QPoint position, identifier.positionsList())
        allPositionString += QString("(%1:%2) ").arg(position.x()).arg(position.y());

    return allPositionString;
}
