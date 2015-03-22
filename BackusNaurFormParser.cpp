#include "BackusNaurFormParser.h"
#include <QStringList>

QList<BackusNaurFormRule> BackusNaurFormParser::parse(QString sourceString)
{
    deleteComments(sourceString);

    QStringList rulesStringList = sourceString.split("\n");
    QList<BackusNaurFormRule> rulesList;
    foreach (QString ruleString, rulesStringList) {
        if (!ruleString.isEmpty())
            rulesList << parseRule(ruleString);
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

int BackusNaurFormParser::findCommentBeginIndex(QString string, int startIndex)
{
    int beginCommentIndex = string.indexOf("/*",startIndex);
    if (beginCommentIndex > 0) {
        if (!string.mid(beginCommentIndex - 1,1).contains("\"")) {
            return beginCommentIndex;
        } return findCommentBeginIndex(string, beginCommentIndex + 2);
    } else {
        return beginCommentIndex;
    }
}

int BackusNaurFormParser::findCommentEndIndex(QString string, int startIndex)
{
    int endCommentIndex = string.indexOf("*/",startIndex);
    if (endCommentIndex > 0) {
        if (!string.mid(endCommentIndex - 1,1).contains("\"")) {
            return endCommentIndex;
        } return findCommentBeginIndex(string, endCommentIndex + 2);
    } else {
        return endCommentIndex;
    }
}

BackusNaurFormRule BackusNaurFormParser::parseRule(QString ruleString)
{
    int separatorIndex = ruleString.indexOf("::==");

    if (separatorIndex < 0) {
        BackusNaurFormRule emptyRule;
        return emptyRule;
    }

    SyntacticSymbol leftPart = parseLeftPart(ruleString.mid(0,separatorIndex));
    QList <SyntacticSymbol> rightPart = parseRightPart(ruleString.mid(separatorIndex + 4));
    BackusNaurFormRule rule = BackusNaurFormRule(leftPart,rightPart);
    return rule;
}

SyntacticSymbol BackusNaurFormParser::parseLeftPart(QString leftPartString)
{
    if (leftPartString.indexOf("S") == 0) {
        SyntacticSymbol startSymbol = SyntacticSymbol("S",SyntacticSymbol::startSymbol,Token::categoryNone);
        return startSymbol;
    }

    return takeNonterminalSymbol(leftPartString);
}

QList<SyntacticSymbol> BackusNaurFormParser::parseRightPart(QString rightPartString)
{
    QList<SyntacticSymbol> rightPart;
    while(!rightPartString.isEmpty()) {
        deleteWhitespaceAtBegin(rightPartString);
        if (rightPartString.indexOf("<") == 0) {
            SyntacticSymbol nonterminal = takeNonterminalSymbol(rightPartString);
            if (nonterminal.isCorrect())
                rightPart << nonterminal;
        }
        if (rightPartString.indexOf("\"") == 0) {
            SyntacticSymbol terminal = takeTerminalSymbol(rightPartString);
            if (terminal.isCorrect())
                rightPart << terminal;
        }
        if (rightPartString.indexOf("n") == 0) {
            SyntacticSymbol numberLiteral = takeNumberLiteral(rightPartString);
            if (numberLiteral.isCorrect())
                rightPart << numberLiteral;
        }
        if (rightPartString.indexOf("i") == 0) {
            SyntacticSymbol id = takeId(rightPartString);
            if (id.isCorrect())
                rightPart << id;
        }
        if (rightPartString.indexOf("l") == 0) {
            SyntacticSymbol linefeed = takeLinefeed(rightPartString);
            if (linefeed.isCorrect())
                rightPart << linefeed;
            if (rightPartString.indexOf("lambda") == 0)
                return rightPart;
        }
        rightPartString.remove(0,1);
        }
    return rightPart;
}

SyntacticSymbol BackusNaurFormParser::takeNonterminalSymbol(QString &string)
{
    deleteWhitespaceAtBegin(string);
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
            string.remove(0,currentCharIndex);
            SyntacticSymbol emptySymbol;
            return emptySymbol;
        }
        case 1: {
            if (!currentChar.contains(QRegExp("[<>]"))) {
                currentCharIndex ++;
                state = 2;
                break;
            }
            string.remove(0,currentCharIndex);
            SyntacticSymbol emptySymbol;
            return emptySymbol;
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
            SyntacticSymbol emptySymbol;
            return emptySymbol;
        }
        default:
            break;
        }
    }
    SyntacticSymbol emptySymbol;
    return emptySymbol;
}

SyntacticSymbol BackusNaurFormParser::takeTerminalSymbol(QString &string)
{
    deleteWhitespaceAtBegin(string);
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
            SyntacticSymbol emptySymbol;
            return emptySymbol;
        }
        case 1: {
            if (!currentChar.contains("\"")) {
                currentCharIndex ++;
                state = 2;
                break;
            }
            string.remove(0,currentCharIndex);
            SyntacticSymbol emptySymbol;
            return emptySymbol;
        }
        case 2: {
            if (currentChar.contains("\"")) {
                SyntacticSymbol terminal = SyntacticSymbol (string.mid(1,currentCharIndex - 1),
                                                            SyntacticSymbol::terminalSymbol,
                                                            Token::categoryKeyword);
                if (!string.mid(1,1).contains(QRegExp("[a-zA-Z]")))
                    terminal.setCategory(Token::categoryCharToken);
                string.remove(0,currentCharIndex + 1);
                return terminal;
            } else {
                currentCharIndex ++;
                break;
            }
        }
        default:
            break;
        }
    }
    string.remove(0,currentCharIndex);
    SyntacticSymbol emptySymbol;
    return emptySymbol;
}

SyntacticSymbol BackusNaurFormParser::takeNumberLiteral(QString &string)
{
    if (string.indexOf("number") == 0) {
        SyntacticSymbol numberLiteral = SyntacticSymbol ("number",
                                                    SyntacticSymbol::terminalSymbol,
                                                    Token::categoryNumberLiteral);
        return numberLiteral;

    }
    string.remove(0,1);
    SyntacticSymbol emptySymbol;
    return emptySymbol;
}

SyntacticSymbol BackusNaurFormParser::takeId(QString &string)
{
    if (string.indexOf("id") == 0) {
        SyntacticSymbol identifier = SyntacticSymbol ("id",
                                                    SyntacticSymbol::terminalSymbol,
                                                    Token::categoryIdentifier);
        return identifier;

    }
    string.remove(0,1);
    SyntacticSymbol emptySymbol;
    return emptySymbol;
}

SyntacticSymbol BackusNaurFormParser::takeLinefeed(QString &string)
{
    if (string.indexOf("linefeed") == 0) {
        SyntacticSymbol linefeed = SyntacticSymbol ("linefeed",
                                                    SyntacticSymbol::terminalSymbol,
                                                    Token::categoryLineFeed);
        return linefeed;

    }
    string.remove(0,1);
    SyntacticSymbol emptySymbol;
    return emptySymbol;
}

void BackusNaurFormParser::deleteWhitespaceAtBegin(QString &string)
{
    QString firstChar = string.mid(0,1);
    while (firstChar.contains(QRegExp("[ \t]"))) {
        string.remove(0,1);
        firstChar = string.mid(0,1);
    }
}

