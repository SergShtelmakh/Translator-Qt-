#ifndef COMPILEOUTPUT_H
#define COMPILEOUTPUT_H
#include <QTextEdit>


class CompileOutputTextEdit : public QTextEdit
{
public:

    explicit CompileOutputTextEdit(QWidget *parent = 0);

    void scrollToBottom();
    void addHTMLString(const QString& newString);

};

#endif // COMPILEOUTPUT_H
