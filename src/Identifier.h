#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QList>
#include <QPoint>
#include <QString>

class Identifier
{

public:

    Identifier(const Identifier &other);
    Identifier(QString, QPoint);

    QList<QPoint> getPositionsList() const;
    QString getName() const;

    QPoint getFirstPosition();
    void addPosition(QPoint); 

    bool operator==(const Identifier& id1)const
    {
        return name == id1.getName();
    }

private:

    QString name;
    QList <QPoint> positionsList;

};

QString IdentifierPositionsCountToString(const Identifier &identifier);
QString IdentifierPositionsToString(const Identifier &identifier);

#endif // IDENTIFIER_H
