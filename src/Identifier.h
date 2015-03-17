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

    QList<QPoint> positionsList() const;
    QString name() const;

    QPoint getFirstPosition();
    void addPosition(QPoint); 

    bool operator==(const Identifier &id1)const;

private:

    QString m_name;
    QList <QPoint> m_positionsList;

};

QString IdentifierPositionsCountToString(const Identifier &identifier);
QString IdentifierPositionsToString(const Identifier &identifier);

#endif // IDENTIFIER_H
