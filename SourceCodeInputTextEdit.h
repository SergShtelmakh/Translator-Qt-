#ifndef SMARTTEXTEDIT_H
#define SMARTTEXTEDIT_H

#include <QTextEdit>

/*!
 * @brief This class used to represent source code.
 */
class SourceCodeInputTextEdit : public QTextEdit
{
    Q_OBJECT

public:

    explicit SourceCodeInputTextEdit(QWidget *parent = 0);
    ~SourceCodeInputTextEdit();

    /*!
     * This method returns current position of cursor.
     *
     * @return Current position of cursor.
     */
    QPoint getCursorCoordinate() const;

signals:

    /*!
     * This signal emits update of status bar.
     *
     * @param[in] currentLine Current line.
     * @param[in] currentCharacter Current character.
     */
    void updateStatusBarSignal(int currentLine,int currentCharacter);

    /*!
     * This signal emits update current widget.
     */
    void updateTextByTimerSignal();

public slots:


    void onTextChangedSlot();

    void onCursorPositionChangedSlot();

    /*!
     * This slot used to update widget by timer.
     */
    void onTextUpdateTimerTimeoutSlot();

private:

    QTimer *m_textUpdateTimer; //!< Timer for updating widget.

};

#endif // SMARTTEXTEDIT_H
