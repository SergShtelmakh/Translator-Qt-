#include "LexicalAnalyzer.h"
#include <QDebug>

const QRegExp LexicalAnalyzer::possibleTokenEndRegExp = QRegExp("[ \t\\+\\-\\*\\/\\=\\<\\>\\,\\(\\)\"]");
const QRegExp LexicalAnalyzer::numberLiteralRegExp = QRegExp("\\d*(\\.\\d+)?(E[\\+\\-]?\\d+)?");
const QRegExp LexicalAnalyzer::identifierRegExp = QRegExp("[A-Z]+[A-Z0-9]*");

void LexicalAnalyzer::analyzeLine(QString line, int lineNumber)
{
    int tokenBeginIndexInLine = 0;
    QList <Token> currentLineTokenList;

    if(!tokenListList.isEmpty()){
        tokenListList.last().append(Token("\n",Token::categoryLineFeed));
    }

    while(tokenBeginIndexInLine < line.length()){

        Token nextToken = getNextToken(line.mid(tokenBeginIndexInLine));
        nextToken.setPosition(QPoint(lineNumber,tokenBeginIndexInLine));
        currentLineTokenList.append(nextToken);

        if(nextToken.getTokenCategory() == Token::categoryIdentifier){
            addIdentifier(Identifier(nextToken.getLexeme(),nextToken.getPosition()));
        }

        if(!nextToken.isCorrect()){
            addError(QString("(%1:%2)\t ").arg(lineNumber).arg(tokenBeginIndexInLine) + nextToken.getErrorInformation());
        }

        tokenBeginIndexInLine += nextToken.getLexeme().length();
    }
    tokenListList.append(currentLineTokenList);
}

void LexicalAnalyzer::addIdentifier(Identifier newIdentifier)
{
    int newIdentifierIndex = identifierList.indexOf(newIdentifier);
    if(identifierList.contains(newIdentifier)){
        identifierList[newIdentifierIndex].addPosition(newIdentifier.getFirstPosition());
    }else{
        identifierList.append(newIdentifier);
    }
}

void LexicalAnalyzer::addError(QString error)
{
    errorText += error + "\n";
}

void LexicalAnalyzer::clearAllAnalyzingData()
{
    errorText.clear();
    identifierList.clear();
    tokenListList.clear();
}

Token LexicalAnalyzer::getNextToken(QString sourceString)
{
    QString firstChar = sourceString.mid(0,1);
    Token nextToken;

    if(firstChar.contains(QRegExp("[ \t]"))){
        // get space token
        return getSpaceToken(sourceString);
    }

    if(firstChar.contains(QRegExp("[0-9\\.]"))){
        // try to get number
        return getNumberLiteralToken(sourceString);
    }

    if(firstChar.contains(QRegExp("[A-Z]"))){
        // try to get keyWord or identifier
        nextToken = getKeyWordToken(sourceString);
        if(nextToken.isCorrect()){
            return nextToken;
        }else{
            return getIdentifierToken(sourceString);
        }
    }

    if(firstChar.contains("\"")){
        // try to get string literal
        return getStringLiteralToken(sourceString);
    }

    // try to get two-character token
    nextToken = getTwoCharacterToken(sourceString);
    if(nextToken.isCorrect()){
        return nextToken;
    }

    // try to get one-character token
    nextToken = getOneCharacterToken(sourceString);
    if(nextToken.isCorrect()){
        return nextToken;
    }

    // return incorrect token
    QString wrongLexema = sourceString.mid(0,sourceString.indexOf(possibleTokenEndRegExp));
    return Token(wrongLexema,Token::categoryNone,"Unknown string");

}

bool LexicalAnalyzer::isHashEmpty()
{
    return keyWordsHash.isEmpty();
}

Token LexicalAnalyzer::getSpaceToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(QRegExp("[^\t ]")));
    return Token(lexema,Token::categorySpace);
}

QList<Identifier> LexicalAnalyzer::getIdentifierList() const
{
    return identifierList;
}

int LexicalAnalyzer::getIdentifierIndex(QString identifierName)
{
    for(int identifierIndex = 0; identifierIndex < identifierList.count(); identifierIndex ++){
        if(identifierList[identifierIndex].getName() == identifierName){
            return identifierIndex;
        }
    }
    return -1;
}

void LexicalAnalyzer::setupHash()
{
    oneCharacterTokensHash.insert("+",Token::categoryOperation);
    oneCharacterTokensHash.insert("-",Token::categoryOperation);
    oneCharacterTokensHash.insert("/",Token::categoryOperation);
    oneCharacterTokensHash.insert("*",Token::categoryOperation);

    oneCharacterTokensHash.insert("=",Token::categoryComparator);
    oneCharacterTokensHash.insert("<",Token::categoryComparator);
    oneCharacterTokensHash.insert(">",Token::categoryComparator);

    oneCharacterTokensHash.insert(",",Token::categoryComma);

    oneCharacterTokensHash.insert("(",Token::categoryBracket);
    oneCharacterTokensHash.insert(")",Token::categoryBracket);

    twoCharacterTokensHash.insert("<=",Token::categoryComparator);
    twoCharacterTokensHash.insert(">=",Token::categoryComparator);

    keyWordsHash.insert("BOOLEAN",LexicalAnalyzer::keyWordTypeName);
    keyWordsHash.insert("DOUBLE",LexicalAnalyzer::keyWordTypeName);
    keyWordsHash.insert("INTEGER",LexicalAnalyzer::keyWordTypeName);
    keyWordsHash.insert("STRING",LexicalAnalyzer::keyWordTypeName);

    keyWordsHash.insert("FALSE",LexicalAnalyzer::keyWordBooleanLiteral);
    keyWordsHash.insert("TRUE",LexicalAnalyzer::keyWordBooleanLiteral);

    keyWordsHash.insert("IF",LexicalAnalyzer::keyWordIfStatement);
    keyWordsHash.insert("THEN",LexicalAnalyzer::keyWordIfStatement);

    keyWordsHash.insert("AS",LexicalAnalyzer::keyWordAssignment);
    keyWordsHash.insert("DATA",LexicalAnalyzer::keyWordAssignment);
    keyWordsHash.insert("DIM",LexicalAnalyzer::keyWordAssignment);
    keyWordsHash.insert("READ",LexicalAnalyzer::keyWordAssignment);

    keyWordsHash.insert("FOR",LexicalAnalyzer::keyWordForLoop);
    keyWordsHash.insert("NEXT",LexicalAnalyzer::keyWordForLoop);
    keyWordsHash.insert("STEP",LexicalAnalyzer::keyWordForLoop);
    keyWordsHash.insert("TO",LexicalAnalyzer::keyWordForLoop);

    keyWordsHash.insert("END",LexicalAnalyzer::keyWordOther);
}

QList<QList<Token> > LexicalAnalyzer::getTokenListList() const
{
    return tokenListList;
}

QString LexicalAnalyzer::getErrorText() const
{
    return errorText;
}

void LexicalAnalyzer::analyze(QString sourceCode)
{
    if(isHashEmpty()){
        setupHash();
    }
    clearAllAnalyzingData();
    QStringList plainTextList = sourceCode.split("\n");
    for(int lineIndex = 0; lineIndex < plainTextList.count(); lineIndex ++){
        analyzeLine(plainTextList[lineIndex], lineIndex);
    }
}

Token LexicalAnalyzer::getNumberLiteralToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(possibleTokenEndRegExp));
    if (numberLiteralRegExp.exactMatch(lexema)){
        if(lexema.count() <= maxNumberLiteralLenght){
            return Token(lexema,Token::categoryNumberLiteral);
        }else{
            return Token(lexema,Token::categoryNone,QString("Number literal lenght greater than %1 characters.").arg(maxNumberLiteralLenght));
        }
    }else{
        return Token(lexema,Token::categoryNone,"Wrong number literal");
    }
}

Token LexicalAnalyzer::getKeyWordToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(possibleTokenEndRegExp));
    if (keyWordsHash.contains(lexema)){
        return Token(lexema,Token::categoryKeyWord);
    }else{
        return Token(lexema,Token::categoryNone);
    }
}

Token LexicalAnalyzer::getIdentifierToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,sourceString.indexOf(possibleTokenEndRegExp));
    if (identifierRegExp.exactMatch(lexema)){
        if(lexema.count() <= maxIdentifierNameLenght){
            return Token(lexema,Token::categoryIdentifier);
        }else{
            return Token(lexema,Token::categoryNone,QString("Identifier lenght greater than %1 characters.").arg(maxIdentifierNameLenght));
        }
    }else{
        return Token(lexema,Token::categoryNone,"Wrong identifier");
    }
}

Token LexicalAnalyzer::getStringLiteralToken(QString sourceString)
{
    int lexemaEndIndex = sourceString.indexOf("\"",1);
    if(lexemaEndIndex == -1){
        return Token(sourceString,Token::categoryNone,"Character \" is missing");
    }else{
        QString lexema = sourceString.mid(0,lexemaEndIndex + 1);
        if(lexema.length() <= maxStringLiteralLenght){
            return Token(lexema,Token::categoryStringLiteral);
        }else{
            return Token(lexema,Token::categoryNone,QString("Identifier lenght greater than %1 characters.").arg(maxStringLiteralLenght));
        }
    }
}

Token LexicalAnalyzer::getTwoCharacterToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,2);
    if (twoCharacterTokensHash.contains(lexema)){
        return Token(lexema,twoCharacterTokensHash.value(lexema));
    }else{
        return Token(lexema,Token::categoryNone);
    }
}

Token LexicalAnalyzer::getOneCharacterToken(QString sourceString)
{
    QString lexema = sourceString.mid(0,1);
    if (oneCharacterTokensHash.contains(lexema)){
        return Token(lexema,oneCharacterTokensHash.value(lexema));
    }else{
        return Token(lexema,Token::categoryNone);
    }
}

QString TokenListListToString(QList<QList<Token> > tokenListList)
{
    QString tokenSequenceString;
    foreach (QList <Token> tokenSubList, tokenListList) {
        foreach (Token currentToken, tokenSubList) {
            tokenSequenceString += currentToken.getTokenRepresentation();
        }
    }
    return tokenSequenceString;
}
