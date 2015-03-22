#ifndef BACKUSNAURFORMPARSER_H
#define BACKUSNAURFORMPARSER_H

#include "BackusNaurFormRule.h"
#include <QList>
#include <QString>

class BackusNaurFormParser
{


public:

     BackusNaurFormParser();
    ~BackusNaurFormParser();

    static QList <BackusNaurFormRule> parse(QString sourceString);

private:

    static void deleteComments(QString &oldString);

    static int findCommentBeginIndex(QString string, int startIndex);

    static int findCommentEndIndex(QString string, int startIndex);

    static BackusNaurFormRule parseRule(QString ruleString);

    static SyntacticSymbol parseLeftPart(QString leftPartString);

    static QList <SyntacticSymbol> parseRightPart(QString rightPartString);

    static SyntacticSymbol takeNonterminalSymbol(QString &string);
    static SyntacticSymbol takeTerminalSymbol(QString &string);
    static SyntacticSymbol takeNumberLiteral(QString &string);
    static SyntacticSymbol takeId(QString &string);
    static SyntacticSymbol takeLinefeed(QString &string);

    static void deleteWhitespaceAtBegin(QString &string);
};

#endif // BACKUSNAURFORMPARSER_H
