/*!
 * \file CompileOutputTextEdit.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef COMPILEOUTPUT_H
#define COMPILEOUTPUT_H

#include <QTextEdit>

/*!
 * @brief This class used to output compile errors.
 */
class CompileOutputTextEdit : public QTextEdit
{

public:

    explicit CompileOutputTextEdit(QWidget *parent = 0);

    /*!
     * This method scrolls widgets scrollbar to bottom.
     */
    void scrollToBottom();

    /*!
     * This method adds string to widget. String can be marked up by HTML tags.
     *
     * @param[in] newString Identifier name.
     * @return Index of identifier in table or -1 when identifier not exists.
     */
    void addHTMLString(const QString& newString);

};

#endif // COMPILEOUTPUT_H
