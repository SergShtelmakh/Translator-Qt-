#include "LexicalAnalyzer.h"
#include <QStringList>

void LexicalAnalyzer::analyzeLine(QString line, int lineNumber)
{
    int tokenBeginIndexInLine = 0;

    if (!m_tokenList.isEmpty())
        m_tokenList.append(Token("\n",Token::categoryLineFeed));

    while (tokenBeginIndexInLine < line.length()) {

        Token nextToken = getNextToken(line.mid(tokenBeginIndexInLine));
        nextToken.setPosition(QPoint(lineNumber,tokenBeginIndexInLine));
        m_tokenList.append(nextToken);

        if (nextToken.tokenCategory() == Token::categoryIdentifier)
            addIdentifier(Identifier(nextToken.lexeme(),nextToken.position()));

        if (!nextToken.isCorrect())
            addError(QString("(%1:%2)\t ").arg(lineNumber).arg(tokenBeginIndexInLine) + nextToken.errorInformation());

        tokenBeginIndexInLine += nextToken.lexeme().length();
    }
}

void LexicalAnalyzer::addIdentifier(Identifier identifier)
{
    int newIdentifierIndex = m_identifierList.indexOf(identifier);
    if (m_identifierList.contains(identifier)) {
        m_identifierList[newIdentifierIndex].addPosition(identifier.getFirstPosition());
    } else {
        m_identifierList.append(identifier);
    }
}

void LexicalAnalyzer::addError(QString error)
{
    m_errorText += error + "\n";
}

void LexicalAnalyzer::clearAllAnalyzingData()
{
    m_errorText.clear();
    m_identifierList.clear();
    m_tokenList.clear();
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

Token LexicalAnalyzer::getNextToken(QString sourceString)
{
    QString firstChar = sourceString.mid(0,1);
    Token nextToken;

    // get space token
    if (firstChar.contains(m_spaceRegExp))
        return getSpaceToken(sourceString);

    // try to get number
    if (firstChar.contains(QRegExp("[0-9\\.]")))
        return getNumberLiteralToken(sourceString);

    // try to get keyWord or identifier
    if (firstChar.contains(QRegExp("[A-Za-z_]"))) {
        nextToken = getKeywordToken(sourceString);
        if (nextToken.isCorrect()) {
            return nextToken;
        } else {
            return getIdentifierToken(sourceString);
        }
    }

    // try to get string literal
    if (sourceString.indexOf(m_beginStringLiteral) == 0)
        return getStringLiteralToken(sourceString);

    // try to get two-character token
    nextToken = getCharacterToken(sourceString);
    if (nextToken.isCorrect())
        return nextToken;

    // return incorrect token
    QString wrongLexema = sourceString.mid(0,sourceString.indexOf(m_possibleTokenEndRegExp));
    return Token(wrongLexema,Token::categoryNone,"Unknown string");

}

int LexicalAnalyzer::maxIdentifierNameLenght() const
{
    return m_maxIdentifierNameLenght;
}

void LexicalAnalyzer::setMaxIdentifierNameLenght(int maxIdentifierNameLenght)
{
    m_maxIdentifierNameLenght = maxIdentifierNameLenght;
}

void LexicalAnalyzer::addKeyword(QString keyword)
{
    int number = m_keyWordsHash.size();
    m_keyWordsHash.insert(keyword,number);
}

void LexicalAnalyzer::addCharacterToken(QString characterToken)
{
    int number = m_keyWordsHash.size();
    m_possibleTokenEndRegExp = AddPossibleVariantToRegExpPattern(m_possibleTokenEndRegExp, characterToken.mid(0,1));

    if (characterToken.length() > m_maxCharacterTokensLenght)
        m_maxCharacterTokensLenght = characterToken.length();
    m_characterTokensHash.insert(characterToken,number);
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


Token LexicalAnalyzer::getSpaceToken(QString sourceString)
{

    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("[^ \t]")));
    return Token(lexema,Token::categorySpace);
}

QList<Identifier> LexicalAnalyzer::identifierList() const
{
    return m_identifierList;
}

int LexicalAnalyzer::getIdentifierIndex(QString identifierName)
{
    for (int identifierIndex = 0; identifierIndex < m_identifierList.count(); identifierIndex ++) {
        if (m_identifierList[identifierIndex].name() == identifierName) {
            return identifierIndex;
        }
    }
    return -1;
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

LexicalAnalyzer::LexicalAnalyzer()
{
    // Set default values
    m_maxNumberLiteralLenght = 25;
    m_maxStringLiteralLenght = 80;
    m_maxIdentifierNameLenght = 20;
    m_possibleTokenEndRegExp = QRegExp("( |\t)");
    m_numberLiteralRegExp = QRegExp("\\d*(\\.\\d+)?(E[\\+\\-]?\\d+)?");
    m_identifierRegExp = QRegExp("^[A-Z][A-Z0-9]*");
    m_spaceRegExp = QRegExp("[ \t]");
    m_beginStringLiteral = "\"";
    m_maxCharacterTokensLenght = 0;
}

void LexicalAnalyzer::analyze(QString sourceCode)
{
    clearAllAnalyzingData();
    QStringList plainTextList = sourceCode.split("\n");
    for (int lineIndex = 0; lineIndex < plainTextList.count(); lineIndex ++)
        analyzeLine(plainTextList[lineIndex], lineIndex);
}

Token LexicalAnalyzer::getNumberLiteralToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("[^0-9\\.E]")));
    if (m_numberLiteralRegExp.exactMatch(lexema)) {
        if (lexema.count() <= m_maxNumberLiteralLenght) {
            return Token(lexema,Token::categoryNumberLiteral);
        } else {
            return Token(lexema,Token::categoryNone,QString("Number literal lenght greater than %1 characters.").arg(m_maxNumberLiteralLenght));
        }
    } else {
        return Token(lexema,Token::categoryNone,"Wrong number literal");
    }
}

Token LexicalAnalyzer::getKeywordToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("\\W")));
    if (m_keyWordsHash.contains(lexema)) {
        return Token(lexema,Token::categoryKeyword);
    } else {
        return Token(lexema,Token::categoryNone);
    }
}

Token LexicalAnalyzer::getIdentifierToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("\\W")));
    if (m_identifierRegExp.exactMatch(lexema)) {
        if (lexema.count() <= m_maxIdentifierNameLenght) {
            return Token(lexema,Token::categoryIdentifier);
        } else {
            return Token(lexema,Token::categoryNone,QString("Identifier lenght greater than %1 characters.").arg(m_maxIdentifierNameLenght));
        }
    } else {
        return Token(lexema,Token::categoryNone,"Wrong identifier");
    }
}

Token LexicalAnalyzer::getStringLiteralToken(QString sourceString)
{
    int lexemaEndIndex = sourceString.indexOf(m_beginStringLiteral,1);
    if (lexemaEndIndex == -1) {
        return Token(sourceString,Token::categoryNone,"Character " + m_beginStringLiteral +" is missing");
    } else {
        QString lexema = sourceString.mid(0,lexemaEndIndex + m_beginStringLiteral.length());
        if (lexema.length() <= m_maxStringLiteralLenght) {
            return Token(lexema,Token::categoryStringLiteral);
        } else {
            return Token(lexema,Token::categoryNone,QString("Identifier lenght greater than %1 characters.").arg(m_maxStringLiteralLenght));
        }
    }
}

Token LexicalAnalyzer::getCharacterToken(QString sourceString)
{
    QString lexema;
    for (int i = maxIdentifierNameLenght(); i > 0; i--) {
        lexema = sourceString.mid(0,i);
        if (m_characterTokensHash.contains(lexema))
            return Token(lexema,Token::categoryCharToken);
    }
    return Token(lexema,Token::categoryNone);
}

QString TokenListToString(QList<Token> tokenList)
{
    QString tokenSequenceString;
    foreach (Token currentToken, tokenList) {
        tokenSequenceString += currentToken.getTokenRepresentation();
    }
    return tokenSequenceString;
}

QRegExp AddPossibleVariantToRegExpPattern(QRegExp oldRegExp, QString variant)
{
    if (oldRegExp.exactMatch(variant))
        return oldRegExp;
    QString oldPattern = oldRegExp.pattern();
    QString pattern = "(" + oldPattern + "|" + "(" + QRegExp::escape(variant) + "))";
    oldRegExp.setPattern(pattern);
    return oldRegExp;
}

