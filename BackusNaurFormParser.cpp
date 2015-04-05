#include "BackusNaurFormParser.h"
#include <QStringList>

QList<BackusNaurFormRule> BackusNaurFormParser::parse(QString &sourceString)
{
    deleteComments(sourceString);

    QStringList rulesStringList = sourceString.split("\n");
    QList<BackusNaurFormRule> rulesList;
    foreach (QString ruleString, rulesStringList) {
        if (!ruleString.isEmpty()) {
            rulesList << parseRule(ruleString);
        }
    }
    return rulesList;
}

void BackusNaurFormParser::deleteComments(QString &oldString)
{
    int beginCommentIndex = findCommentBeginIndex(oldString,0);
    while (beginCommentIndex != -1) {
        int endCommentIndex = findCommentEndIndex(oldString,beginCommentIndex + 2);
        if (endCommentIndex != -1) {
            oldString.remove(beginCommentIndex, endCommentIndex - beginCommentIndex + 2);
        }
        beginCommentIndex = findCommentBeginIndex(oldString,0);
    }
}

int BackusNaurFormParser::findCommentBeginIndex(const QString &string, int startIndex)
{
    int beginCommentIndex = string.indexOf("/*",startIndex);
    if (beginCommentIndex > 0) {
        if (!string.mid(beginCommentIndex - 1,1).contains("\"")) {
            return beginCommentIndex;
        }
        return findCommentBeginIndex(string, beginCommentIndex + 2);
    } else {
        return beginCommentIndex;
    }
}

int BackusNaurFormParser::findCommentEndIndex(const QString &string, int startIndex)
{
    int endCommentIndex = string.indexOf("*/",startIndex);
    if (endCommentIndex > 0) {
        if (!string.mid(endCommentIndex - 1,1).contains("\"")) {
            return endCommentIndex;
        }
        return findCommentBeginIndex(string, endCommentIndex + 2);
    } else {
        return endCommentIndex;
    }
}

BackusNaurFormRule BackusNaurFormParser::parseRule(const QString &ruleSourceString)
{
    int separatorIndex = ruleSourceString.indexOf("::=");

    if (separatorIndex < 0)
        return BackusNaurFormRule();

    SyntacticSymbol leftPart = parseLeftPart(ruleSourceString.mid(0,separatorIndex));
    QList <SyntacticSymbol> rightPart = parseRightPart(ruleSourceString.mid(separatorIndex + 3));
    BackusNaurFormRule rule = BackusNaurFormRule(leftPart,rightPart);
    return rule;
}

SyntacticSymbol BackusNaurFormParser::parseLeftPart(QString leftPartString)
{
    deleteWhitespaceAtBegin(leftPartString);
    if (leftPartString.indexOf("S") == 0)
        return SyntacticSymbol("S",SyntacticSymbol::startSymbol,Token::categoryNone);

    return takeNonterminalSymbol(leftPartString);
}

QList<SyntacticSymbol> BackusNaurFormParser::parseRightPart(QString rightPartSourceString)
{
    QList<SyntacticSymbol> rightPart;
    while(!rightPartSourceString.isEmpty()) {
        deleteWhitespaceAtBegin(rightPartSourceString);

        if (rightPartSourceString.indexOf("<") == 0)
            appendListByCorrectItem(rightPart,takeNonterminalSymbol(rightPartSourceString));
        if (rightPartSourceString.indexOf("\"") == 0)
            appendListByCorrectItem(rightPart,takeTerminalSymbol(rightPartSourceString));
        if (rightPartSourceString.indexOf("n") == 0)
            appendListByCorrectItem(rightPart,takeNumberLiteral(rightPartSourceString));
        if (rightPartSourceString.indexOf("i") == 0)
            appendListByCorrectItem(rightPart,takeId(rightPartSourceString));
        if (rightPartSourceString.indexOf("lin") == 0)
            appendListByCorrectItem(rightPart,takeLinefeed(rightPartSourceString));
        if (rightPartSourceString.indexOf("lit") == 0)
            appendListByCorrectItem(rightPart,takeLiteral(rightPartSourceString));

        if (rightPartSourceString.indexOf("lambda") == 0) {
            return rightPart;
        }
        rightPartSourceString.remove(0,1);
    }
    return rightPart;
}

SyntacticSymbol BackusNaurFormParser::takeNonterminalSymbol(QString &string)
{
    int currentCharIndex = 0;
    int state = 0;
    while (currentCharIndex < string.length()) {
        QString currentChar = string.mid(currentCharIndex,1);
        switch (state) {
        case 0: {
            if (currentChar.contains("<")) {
                currentCharIndex ++;
                state = 1;
                break;
            }
            return SyntacticSymbol();
        }
        case 1: {
            if (!currentChar.contains(QRegExp("[<>]"))) {
                currentCharIndex ++;
                state = 2;
                break;
            }
            string.remove(0,currentCharIndex);
            return SyntacticSymbol();
        }
        case 2: {
            if (currentChar.contains(">")) {
                SyntacticSymbol nonterminal = SyntacticSymbol (string.mid(1,currentCharIndex - 1),
                                                               SyntacticSymbol::nonterminalSymbol,
                                                               Token::categoryNone);
                string.remove(0,currentCharIndex + 1);
                return nonterminal;
            }
            if (!currentChar.contains("<")) {
                currentCharIndex ++;
                break;
            }
            string.remove(0,currentCharIndex);
            return SyntacticSymbol();
        }
        default:
            break;
        }
    }
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::takeTerminalSymbol(QString &string)
{
    int currentCharIndex = 0;
    int state = 0;
    while (currentCharIndex < string.length()) {
        QString currentChar = string.mid(currentCharIndex,1);
        switch (state) {
        case 0: {
            if (currentChar.contains("\"")) {
                currentCharIndex ++;
                state = 1;
                break;
            }
            string.remove(0,currentCharIndex);
            return SyntacticSymbol();
        }
        case 1: {
            if (!currentChar.contains("\"")) {
                currentCharIndex ++;
                state = 2;
                break;
            }
            string.remove(0,currentCharIndex);
            return SyntacticSymbol();
        }
        case 2: {
            if (currentChar.contains("\"")) {
                SyntacticSymbol terminal = SyntacticSymbol (string.mid(1,currentCharIndex - 1),
                                                            SyntacticSymbol::terminalSymbol,
                                                            Token::categoryKeyword);
                if (!string.mid(1,1).contains(QRegExp("[a-zA-Z]"))) {
                    terminal.setCategory(Token::categoryCharToken);
                }
                string.remove(0,currentCharIndex + 1);
                return terminal;
            }
            currentCharIndex ++;
            break;
        }
        default:
            break;
        }
    }
    string.remove(0,currentCharIndex);
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::takeNumberLiteral(QString &string)
{
    if (string.indexOf("number") == 0)
        return SyntacticSymbol ("number", SyntacticSymbol::terminalSymbol, Token::categoryNumberLiteral);
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::takeId(QString &string)
{
    if (string.indexOf("id") == 0)
        return SyntacticSymbol ("id", SyntacticSymbol::terminalSymbol, Token::categoryIdentifier);
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::takeLinefeed(QString &string)
{
    if (string.indexOf("linefeed") == 0)
        return SyntacticSymbol ("linefeed", SyntacticSymbol::terminalSymbol, Token::categoryLineFeed);
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::takeLiteral(QString &string)
{
    if (string.indexOf("literal") == 0)
        return SyntacticSymbol ("literal", SyntacticSymbol::terminalSymbol, Token::categoryStringLiteral);
    return SyntacticSymbol();
}

void BackusNaurFormParser::deleteWhitespaceAtBegin(QString &string)
{
    while (string.mid(0,1).contains(QRegExp("[ \t]")))
        string.remove(0,1);
}

void appendListByCorrectItem(QList<SyntacticSymbol> &list, SyntacticSymbol item)
{
    if (item.isCorrect())
        list.append(item);
}
