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

    if (separatorIndex < 0) {
        return BackusNaurFormRule();
    }
    SyntacticSymbol leftPart = parseLeftPart(ruleSourceString.mid(0,separatorIndex));
    QList <SyntacticSymbol> rightPart = parseRightPart(ruleSourceString.mid(separatorIndex + 3));
    return BackusNaurFormRule(leftPart,rightPart);
}

SyntacticSymbol BackusNaurFormParser::parseLeftPart(QString leftPartString)
{
    DeleteWhitespaceAtBegin(leftPartString);
    if (leftPartString.indexOf("S") == 0)
        return SyntacticSymbol("S",SyntacticSymbol::START_SYMBOL,Token::NONE_CATEGORY);

    return getNonterminalSymbol(leftPartString);
}

QList<SyntacticSymbol> BackusNaurFormParser::parseRightPart(QString rightPartSourceString)
{
    QList<SyntacticSymbol> rightPart;
    while(!rightPartSourceString.isEmpty()) {
        DeleteWhitespaceAtBegin(rightPartSourceString);

        // If lambda-rule than return empty list
        if (rightPartSourceString.indexOf("lambda") == 0)
            return rightPart;

        SyntacticSymbol currentSymbol;
        if (rightPartSourceString.indexOf("<") == 0) {
            currentSymbol = getNonterminalSymbol(rightPartSourceString);
        } else if (rightPartSourceString.indexOf("\"") == 0) {
            currentSymbol = getTerminalSymbol(rightPartSourceString);
        } else if (rightPartSourceString.indexOf("number") == 0) {
            currentSymbol = SyntacticSymbol ("number", SyntacticSymbol::TERMINAL_SYMBOL, Token::NUMBER_LITERAL_CATEGORY);
        } else if (rightPartSourceString.indexOf("id") == 0) {
            currentSymbol = SyntacticSymbol ("id", SyntacticSymbol::TERMINAL_SYMBOL, Token::IDENTIFIER_CATEGORY);
        } else if (rightPartSourceString.indexOf("linefeed") == 0) {
            currentSymbol = SyntacticSymbol ("linefeed", SyntacticSymbol::TERMINAL_SYMBOL, Token::LINE_FEED_CATEGORY);
        } else if (rightPartSourceString.indexOf("literal") == 0) {
            currentSymbol = SyntacticSymbol ("literal", SyntacticSymbol::TERMINAL_SYMBOL, Token::STRING_LITERAL_CATEGORY);
        }
        AppendListByCorrectItem(rightPart, currentSymbol);

        rightPartSourceString.remove(0, GetSyntacticSymbolNameLengthWithBrackets(currentSymbol));
    }
    return rightPart;
}

SyntacticSymbol BackusNaurFormParser::getNonterminalSymbol(QString &string)
{
    int currentCharIndex = -1;
    int state = 0;
    while (currentCharIndex < string.length()) {
        currentCharIndex ++;
        QString currentChar = string.mid(currentCharIndex, 1);
        switch (state) {
        case 0: {
            if (currentChar.contains("<")) {
                state = 1;
            } else {
                return SyntacticSymbol();
            }
            break;
        }
        case 1: {
            if (!currentChar.contains(QRegExp("[<>]"))) {
                state = 2;
            } else {
                string.remove(0,currentCharIndex);
                return SyntacticSymbol();
            }
            break;
        }
        case 2: {
            if (currentChar.contains(">")) {
                SyntacticSymbol nonterminal = SyntacticSymbol (string.mid(1,currentCharIndex - 1),SyntacticSymbol::NONTERMINAL_SYMBOL,Token::NONE_CATEGORY);
                return nonterminal;
            } else if (currentChar.contains("<")) {
                string.remove(0,currentCharIndex);
                return SyntacticSymbol();
            }
            break;
        }
        default:
            break;
        }
    }
    return SyntacticSymbol();
}

SyntacticSymbol BackusNaurFormParser::getTerminalSymbol(QString &string)
{
    int currentCharIndex = -1;
    int state = 0;
    while (currentCharIndex < string.length()) {
        currentCharIndex ++;
        QString currentChar = string.mid(currentCharIndex,1);
        switch (state) {
        case 0: {
            if (currentChar.contains("\"")) {
                state = 1;
            } else {
                string.remove(0,currentCharIndex);
                return SyntacticSymbol();
            }
            break;
        }
        case 1: {
            if (!currentChar.contains("\"")) {
                state = 2;
            } else {
                string.remove(0,currentCharIndex);
                return SyntacticSymbol();
            }
            break;
        }
        case 2: {
            if (currentChar.contains("\"")) {
                SyntacticSymbol terminal = SyntacticSymbol (string.mid(1,currentCharIndex - 1), SyntacticSymbol::TERMINAL_SYMBOL, Token::KEYWORD_CATEGORY);
                if (!string.mid(1,1).contains(QRegExp("[a-zA-Z]"))) {
                    terminal.setCategory(Token::CHAR_TOKEN_CATEGORY);
                }
                return terminal;
            }
            break;
        }
        default:
            break;
        }
    }
    string.remove(0,currentCharIndex);
    return SyntacticSymbol();
}

void DeleteWhitespaceAtBegin(QString &string)
{
    while (string.mid(0,1).contains(QRegExp("[ \t]")))
        string.remove(0,1);
}

void AppendListByCorrectItem(QList<SyntacticSymbol> &list, const SyntacticSymbol &item)
{
    if (item.isCorrect())
        list.append(item);
}

int GetSyntacticSymbolNameLengthWithBrackets(const SyntacticSymbol &symbol)
{
    int symbolNameLength = symbol.name().length();
    if (symbol.category() == Token::KEYWORD_CATEGORY) {
        symbolNameLength += 2;
    } else if (symbol.category() == Token::CHAR_TOKEN_CATEGORY) {
        symbolNameLength += 2;
    } else if (symbol.type() == SyntacticSymbol::NONTERMINAL_SYMBOL) {
        symbolNameLength += 2;
    }
    return symbolNameLength;
}
