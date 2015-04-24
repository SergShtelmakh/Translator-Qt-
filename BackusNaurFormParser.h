/*!
 * \file BackusNaurFormParser.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

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

    static SyntacticSymbol getNonterminalSymbol(QString &string);
    static SyntacticSymbol getTerminalSymbol(QString &string);

    static void deleteComments(QString &oldString);

    static int findCommentBeginIndex(const QString &string, int startIndex);
    static int findCommentEndIndex(const QString &string, int startIndex);

};

void DeleteWhitespaceAtBegin(QString &string);
void AppendListByCorrectItem(QList<SyntacticSymbol> &list, const SyntacticSymbol &item);
int GetSyntacticSymbolNameLengthWithBrackets(const SyntacticSymbol &symbol);

#endif // BACKUSNAURFORMPARSER_H
