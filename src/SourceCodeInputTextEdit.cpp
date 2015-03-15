#include "SourceCodeInputTextEdit.h"
#include "LexicalAnalysisHTMLMarkupGenerator.h"
#include <QScrollBar>

SourceCodeInputTextEdit::SourceCodeInputTextEdit(QWidget *parent) : QTextEdit(parent)
{
    textUpdateTimer = new QTimer(this);

    connect(this,SIGNAL(textChanged()),SLOT(onTextChangedSlot()));
    connect(this,SIGNAL(cursorPositionChanged()),SLOT(onCursorPositionChangedSlot()));
    connect(textUpdateTimer,SIGNAL(timeout()),SLOT(onTextUpdateTimerTimeoutSlot()));

    this->setFont(QFont("Courier New", 16));
    this->setTabStopWidth(40);
}

SourceCodeInputTextEdit::~SourceCodeInputTextEdit()
{
    delete textUpdateTimer;
}

QPoint SourceCodeInputTextEdit::getCursorCoordinate()
{
    QPoint cursorCoordinate;
    QString plainText = toPlainText();
    int currentCharIndex = textCursor().position();

    plainText.remove(currentCharIndex,plainText.length() - currentCharIndex);
    cursorCoordinate.setY(plainText.count("\n"));

    plainText.remove(0,plainText.lastIndexOf("\n") + 1);
    cursorCoordinate.setX(plainText.length());

    return cursorCoordinate;
}

void SourceCodeInputTextEdit::onCursorPositionChangedSlot()
{
    QPoint cursorCoordinate = getCursorCoordinate();
    emit updateStatusBarSignal(cursorCoordinate.y(),cursorCoordinate.x());
}

void SourceCodeInputTextEdit::onTextChangedSlot()
{
    if(!textUpdateTimer->isActive()){
        textUpdateTimer->start(100);
    }
}

void SourceCodeInputTextEdit::onTextUpdateTimerTimeoutSlot()
{
    int cursorPos = textCursor().position();
    int scrollPos = verticalScrollBar()->value();

    emit updateTextByTimerSignal();

    QTextCursor cursor = textCursor();
    cursor.setPosition(cursorPos, QTextCursor::MoveAnchor);
    setTextCursor(cursor);

    QScrollBar *scrollBar = verticalScrollBar();
    scrollBar->setValue(scrollPos);

    textUpdateTimer->stop();
}

