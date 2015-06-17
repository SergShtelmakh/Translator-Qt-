#include "CompileOutputTextEdit.h"
#include "HTMLColors.h"
#include "HTMLMarkupGenerator.h"
#include <QScrollBar>

CompileOutputTextEdit::CompileOutputTextEdit(QWidget *parent): QTextEdit(parent)
{
    setFont(QFont("Courier New", 12));
}

void CompileOutputTextEdit::scrollToBottom()
{
    QScrollBar *scrollBar = verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void CompileOutputTextEdit::addHTMLString(const QString &newString)
{
    QString editorValue = "<font color=" + HTMLColors::gray + ">" + toPlainText() + "</font>" + newString;
    this->setHtml(PlainTextToHTML(editorValue));
    this->scrollToBottom();
}



