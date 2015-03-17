#ifndef SMARTTEXTEDIT_H
#define SMARTTEXTEDIT_H

#include <QTextEdit>

class SourceCodeInputTextEdit : public QTextEdit
{
    Q_OBJECT

public:

    explicit SourceCodeInputTextEdit(QWidget *parent = 0);
    ~SourceCodeInputTextEdit();

    QPoint getCursorCoordinate();

signals:

    void updateStatusBarSignal(int,int);
    void updateTextByTimerSignal();

public slots:

    void onTextChangedSlot();
    void onCursorPositionChangedSlot();
    void onTextUpdateTimerTimeoutSlot();

private:

    QTimer *m_textUpdateTimer;

};

#endif // SMARTTEXTEDIT_H
