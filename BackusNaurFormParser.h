#ifndef BACKUSNAURFORMPARSER_H
#define BACKUSNAURFORMPARSER_H

#include "BackusNaurFormRule.h"
#include <QList>
#include <QString>

class BackusNaurFormParser
{

public:

    static QList <BackusNaurFormRule> parse(QString &sourceString);

private:

    static BackusNaurFormRule parseRule(const QString &ruleString);

    static SyntacticSymbol parseLeftPart(QString leftPartString);
    static QList <SyntacticSymbol> parseRightPart(QString rightPartString);

    static SyntacticSymbol takeNonterminalSymbol(QString &string);
    static SyntacticSymbol takeTerminalSymbol(QString &string);
    static SyntacticSymbol takeNumberLiteral(QString &string);
    static SyntacticSymbol takeId(QString &string);
    static SyntacticSymbol takeLinefeed(QString &string);

    static void deleteComments(QString &oldString);
    static void deleteWhitespaceAtBegin(QString &string);

    static int findCommentBeginIndex(const QString &string, int startIndex);
    static int findCommentEndIndex(const QString &string, int startIndex);
};

void appendListByCorrectItem(QList<SyntacticSymbol> &list, SyntacticSymbol item);

#endif // BACKUSNAURFORMPARSER_H
