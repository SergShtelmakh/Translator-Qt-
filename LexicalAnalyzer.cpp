#include "LexicalAnalyzer.h"
#include "ErrorGenerator.h"
#include <QStringList>
#include "Token.h"

LexicalAnalyzer::LexicalAnalyzer() :
    m_possibleTokenEndRegExp(QRegExp("( |\t|\n)")),
    m_maxCharacterTokensLenght(0)
{}

void LexicalAnalyzer::analyzeLine(const QString &line, int lineNumber)
{
    int tokenBeginIndexInLine = 0;
    while (tokenBeginIndexInLine < line.length()) {
        Token nextToken = getNextToken(QPoint(tokenBeginIndexInLine, lineNumber), line.mid(tokenBeginIndexInLine));
        m_tokenList.append(nextToken);
        tokenBeginIndexInLine += nextToken.lexeme().length();
    }
}

void LexicalAnalyzer::addError(const QString &error)
{
    m_errorText += QString("%1:\t").arg(m_errorText.split("\n").count()) + error + "\n";
}

void LexicalAnalyzer::clearAllAnalyzingData()
{
    m_errorText.clear();
    m_tokenList.clear();
}

Token LexicalAnalyzer::getNumberLiteralTokenWithCorrectLength(const QString &lexeme)
{
    if (lexeme.length() > m_maxNumberLiteralLenght) {
        return Token(lexeme, Token::categoryNone, QString("Number literal lenght greater than %1 characters.").arg(m_maxNumberLiteralLenght));
    } else {
        return Token(lexeme, Token::categoryNumberLiteral);
    }
}

QRegExp LexicalAnalyzer::identifierRegExp() const
{
    return m_identifierRegExp;
}

void LexicalAnalyzer::setIdentifierRegExp(const QRegExp &identifierRegExp)
{
    m_identifierRegExp = identifierRegExp;
}

QRegExp LexicalAnalyzer::spaceRegExp() const
{
    return m_spaceRegExp;
}

void LexicalAnalyzer::setSpaceRegExp(const QRegExp &spaceRegExp)
{
    m_spaceRegExp = spaceRegExp;
}

QString LexicalAnalyzer::beginStringLiteral() const
{
    return m_beginStringLiteral;
}

void LexicalAnalyzer::setBeginStringLiteral(const QString &beginStringLiteral)
{
    m_beginStringLiteral = beginStringLiteral;
}

Token LexicalAnalyzer::getNextToken(const QPoint currentPosition, const QString &sourceString)
{
    if (sourceString == "\n")
        return Token("linefeed",Token::categoryLineFeed,"",currentPosition);

    QString firstChar = sourceString.mid(0,1);
    Token nextToken;
    if (firstChar.contains(m_spaceRegExp)) {
        nextToken = getSpaceToken(sourceString);
    } else if (firstChar.contains(QRegExp("[0-9\\.]"))) {
        nextToken = getNumberLiteralToken(sourceString);
    } else if (firstChar.contains(QRegExp("[A-Za-z_]"))) {
        nextToken = getKeywordToken(sourceString);
        if (!nextToken.isCorrect()) {
            nextToken = getIdentifierToken(sourceString);
        }
    } else if (sourceString.indexOf(m_beginStringLiteral) == 0) {
        nextToken =  getStringLiteralToken(sourceString);
    } else {
        nextToken = getCharacterToken(sourceString);
    }

    nextToken.setPosition(currentPosition);

    if (!nextToken.isCorrect()) {
        QString wrongLexema = sourceString.mid(0,sourceString.indexOf(m_possibleTokenEndRegExp,1));
        nextToken = Token(wrongLexema,Token::categoryNone,"Unknown string",currentPosition);
        addError(ErrorGenerator::lexicalError(nextToken));
    }
    return nextToken;
}

int LexicalAnalyzer::maxIdentifierNameLenght() const
{
    return m_maxIdentifierNameLenght;
}

void LexicalAnalyzer::setMaxIdentifierNameLenght(int maxIdentifierNameLenght)
{
    m_maxIdentifierNameLenght = maxIdentifierNameLenght;
}

void LexicalAnalyzer::addKeyword(const QString &keyword)
{
    m_keyWordsHash.insert(keyword,m_keyWordsHash.size());
}

void LexicalAnalyzer::addCharacterToken(const QString &characterToken)
{
    AddPossibleVariantToRegExpPattern(m_possibleTokenEndRegExp, characterToken.mid(0,1));

    if (characterToken.length() > m_maxCharacterTokensLenght)
        m_maxCharacterTokensLenght = characterToken.length();
    m_characterTokensHash.insert(characterToken,m_keyWordsHash.size());
}

int LexicalAnalyzer::maxStringLiteralLenght() const
{
    return m_maxStringLiteralLenght;
}

void LexicalAnalyzer::setMaxStringLiteralLenght(int maxStringLiteralLenght)
{
    m_maxStringLiteralLenght = maxStringLiteralLenght;
}

int LexicalAnalyzer::maxNumberLiteralLenght() const
{
    return m_maxNumberLiteralLenght;
}

void LexicalAnalyzer::setMaxNumberLiteralLenght(int maxNumberLiteralLenght)
{
    m_maxNumberLiteralLenght = maxNumberLiteralLenght;
}

Token LexicalAnalyzer::getSpaceToken(const QString &sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("[^ \t]")));
    return Token(lexema,Token::categorySpace);
}

QList<Token> LexicalAnalyzer::getTokenListWithoutSpaces() const
{
    QList <Token> listWithoutSpaces;
    foreach (Token currentToken, m_tokenList) {
        if (currentToken.tokenCategory() != Token::categorySpace)
            listWithoutSpaces << currentToken;
    }
    return listWithoutSpaces;
}

QList<Token> LexicalAnalyzer::tokenList() const
{
    return m_tokenList;
}

QString LexicalAnalyzer::errorText() const
{
    return m_errorText;
}

void LexicalAnalyzer::analyze(const QString &sourceCode)
{
    clearAllAnalyzingData();
    QStringList plainTextList = sourceCode.split("\n");
    for ( int i = 0; i < plainTextList.count() - 1; i++) {
        plainTextList[i] += "\n";
    }
    for (int lineIndex = 0; lineIndex < plainTextList.count(); lineIndex ++)
        analyzeLine(plainTextList[lineIndex], lineIndex);
}

Token LexicalAnalyzer::getNumberLiteralToken(const QString &sourceString)
{
    int state = 0;
    int tokenEndIndex = -1;
    QString currentChar;
    for (;;) {
        tokenEndIndex++;
        int possibleLexemeEndIndex = sourceString.indexOf(m_possibleTokenEndRegExp,tokenEndIndex);
        QString possibleLexeme = sourceString.mid(0,possibleLexemeEndIndex);
        switch (state) {
        case 0: { //  0124
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 0;
            } else if (currentChar.contains(".")) {
                state = 1;
            } else if (currentChar.contains("E")) {
                state = 3;
            } else if (currentChar.contains(m_possibleTokenEndRegExp)||currentChar.isEmpty()) {
                return getNumberLiteralTokenWithCorrectLength(possibleLexeme);
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal. After digits might be \".\" or \"E\".");
            }
            break;
        }
        case 1: { //  01234.
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 2;
            } else if (currentChar.contains(m_possibleTokenEndRegExp)||currentChar.isEmpty()) {
                return getNumberLiteralTokenWithCorrectLength(possibleLexeme);
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal. After \".\" must be digit!");
            }
            break;
        }
        case 2: { //  01234.567
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 2;
            } else if (currentChar.contains("E")) {
                state = 3;
            } else if (currentChar.contains(m_possibleTokenEndRegExp)||currentChar.isEmpty()) {
                return getNumberLiteralTokenWithCorrectLength(possibleLexeme);
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal.");
            }
            break;
        }
        case 3: { //  01234.567E
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 5;
            } else if (currentChar.contains(QRegExp("[\\+\\-]"))) {
                state = 4;
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal. After \"E\" might be \"+\", \"-\" or digit.");
            }
            break;
        }
        case 4: { //  01234.567E-
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 5;
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal. Digits are missing.");
            }
            break;
        }
        case 5: { //  01234.567E-89
            currentChar = sourceString.mid(tokenEndIndex,1);
            if (currentChar.contains(QRegExp("[0-9]"))) {
                state = 5;
            } else if (currentChar.contains(m_possibleTokenEndRegExp)||currentChar.isEmpty()) {
                return getNumberLiteralTokenWithCorrectLength(possibleLexeme);
            } else {
                return Token(possibleLexeme,Token::categoryNone,"Wrong number literal.");
            }
            break;
        }
        default:
            break;
        }
    }
}

Token LexicalAnalyzer::getKeywordToken(const QString &sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("\\W")));
    if (m_keyWordsHash.contains(lexema)) {
        return Token(lexema, Token::categoryKeyword);
    } else {
        return Token(lexema, Token::categoryNone);
    }
}

Token LexicalAnalyzer::getIdentifierToken(const QString &sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("\\W")));
    if (m_identifierRegExp.exactMatch(lexema)) {
        if (lexema.count() <= m_maxIdentifierNameLenght) {
            return Token(lexema, Token::categoryIdentifier);
        } else {
            return Token(lexema, Token::categoryNone, QString("Identifier lenght greater than %1 characters.").arg(m_maxIdentifierNameLenght));
        }
    } else {
        return Token(lexema, Token::categoryNone, "Wrong identifier");
    }
}

Token LexicalAnalyzer::getStringLiteralToken(const QString &sourceString)
{
    int lexemaEndIndex = sourceString.indexOf(m_beginStringLiteral,1);
    if (lexemaEndIndex == -1) {
        return Token(sourceString,Token::categoryNone,"Character " + m_beginStringLiteral +" is missing");
    } else {
        QString lexema = sourceString.mid(0,lexemaEndIndex + m_beginStringLiteral.length());
        if (lexema.length() <= m_maxStringLiteralLenght + (m_beginStringLiteral.length() * 2)) {
            return Token(lexema,Token::categoryStringLiteral);
        } else {
            return Token(lexema,Token::categoryNone,QString("String literal lenght greater than %1 characters.").arg(m_maxStringLiteralLenght));
        }
    }
}

Token LexicalAnalyzer::getCharacterToken(const QString &sourceString)
{
    QString lexema;
    for (int i = m_maxIdentifierNameLenght; i > 0; i--) {
        lexema = sourceString.mid(0,i);
        if (m_characterTokensHash.contains(lexema))
            return Token(lexema,Token::categoryCharToken);
    }
    return Token(lexema,Token::categoryNone);
}

QString MakeStringRepresentation(const QList<Token> &tokenList)
{
    QString tokenSequenceString;
    foreach (Token currentToken, tokenList) {
        tokenSequenceString += MakeStringRepresentation(currentToken);
        if (currentToken.tokenCategory() == Token::categoryLineFeed)
            tokenSequenceString += "\n";
    }
    return tokenSequenceString;
}

void AddPossibleVariantToRegExpPattern(QRegExp &oldRegExp, const QString &variant)
{
    if (oldRegExp.exactMatch(variant))
        return;
    QString oldPattern = oldRegExp.pattern();
    oldRegExp.setPattern("(" + oldPattern + "|" + "(" + QRegExp::escape(variant) + "))");
    return;
}

