/*!
 * \file Identifier.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QList>
#include <QPoint>
#include <QString>

/*!
 * @brief This class used to store informations about identifiers.
 */
class Identifier
{

public:

    Identifier(const Identifier &other);
    Identifier(const QString &name,const QPoint &position);

    QList <QPoint> positionsList() const;
    QString name() const;

    /*!
     * This method returns first position of identifier in source code.
     *
     * @return First positions in source code.
     */
    QPoint getFirstPosition();

    /*!
     * This method add new position of identifier in source code.
     *
     * @param[in] posistion New position.
     */
    void addPosition(const QPoint &position);

    bool operator==(const Identifier &otherIdentifier)const;

private:

    QString m_name;                     //!< Identifiers name.
    QList <QPoint> m_positionsList;     //!< List of identifiers positions.

};

/*!
 * This function gets all identifiers positions and returns that as string.
 *
 * @param[in] identifier Identifier.
 * @return Identifiers position list as string.
 */
QString IdentifierPositionsToString(const Identifier &identifier);

#endif // IDENTIFIER_H
