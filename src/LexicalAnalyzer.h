#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QObject>
#include <QStringListIterator>
#include <QColor>
#include <QHash>
#include "Identifier.h"
#include "Token.h"

class LexicalAnalyzer
{

public:

    enum KeyWordType{
        typeAssignmentKeyWord,
        typeBooleanLiteralKeyWord,
        typeForLoopKeyWord,
        typeIfStatementKeyWord,
        typeOtherKeyWord,
        typeTypeNameKeyWord
    };


    static const QRegExp possibleTokenEndRegExp;
    static const QRegExp numberLiteralRegExp;
    static const QRegExp identifierRegExp;

    static const int maxNumberLiteralLenght = 25;
    static const int maxStringLiteralLenght = 80;
    static const int maxIdentifierNameLenght = 20;


    LexicalAnalyzer(){}

    void analyze(QString source);

    QString getErrorText() const;
    QList<QList<Token> > getTokenListList() const;
    QList<Identifier> getIdentifierList() const;
    QStringList getPlainTextList() const;

    int getIdentifierIndex(QString);

private:

    void setupHash();

    Token getNextToken(QString sourceString);
    Token getSpaceToken(QString sourceString);
    Token getNumberLiteralToken(QString sourceString);
    Token getKeyWordToken(QString sourceString);
    Token getIdentifierToken(QString sourceString);
    Token getStringLiteralToken(QString sourceString);
    Token getTwoCharacterToken(QString sourceString);
    Token getOneCharacterToken(QString sourceString);

    void analyzeLine(QString sourceLine, int lineNumber);
    void addIdentifier(Identifier);
    void addError(QString);
    void clearAllAnalyzingData();

    QList <Identifier> identifierList;
    QList < QList <Token> > tokenListList;
    QStringList plainTextList;
    QString errorText;

    QHash <QString, KeyWordType> keyWordsHash;
    QHash <QString, Token::TokenCategory> oneCharacterTokensHash;
    QHash <QString, Token::TokenCategory> twoCharacterTokensHash;
    bool isHashEmpty();
};

QString TokenListListToString(QList<QList<Token> > tokenListList);

#endif // LEXICALANALYZER_H
