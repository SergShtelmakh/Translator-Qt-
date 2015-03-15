#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QHash>
#include "Token.h"
#include "Identifier.h"

/*!
 * @brief This class provides lexical analysis.
 *
 * Lexical analyzer parses source code and returns it as a tokens sequence.
 *
 */

class LexicalAnalyzer
{

public:

    //! This enumerator consists of the most used keyword types.
    enum KeyWordType{
        keyWordAssignment,      //!< DATA,DIM,AS,READ.
        keyWordBooleanLiteral,  //!< TRUE,FALSE.
        keyWordForLoop,         //!< FOR, TO, STEP, NEXT.
        keyWordIfStatement,     //!< IF,THEN,ELSE.
        keyWordOther,           //!< END.
        keyWordTypeName         //!< BOOLEAN,INTEGER,DOUBLE,STRING.
    };

    static const QRegExp possibleTokenEndRegExp;    //!< RegExp that can separate tokens.
    static const QRegExp numberLiteralRegExp;       //!< RegExp for searching number literals.
    static const QRegExp identifierRegExp;          //!< RegExp for searching identifiers.

    static const int maxNumberLiteralLenght = 25;   //!< Maximum number literal lenght.
    static const int maxStringLiteralLenght = 80;   //!< Maximum string literal lenght.
    static const int maxIdentifierNameLenght = 20;  //!< Maximum identifier name lenght.

    LexicalAnalyzer(){}

    /*!
     * This method makes lexical analysis.
     *
     * @param[in] sourceCode Source code for lexical analysis.
     */
    void analyze(QString sourceCode);

    QString getErrorText() const;
    QList<QList<Token> > getTokenListList() const;
    QList<Identifier> getIdentifierList() const;

    /*!
     * This method searches identifier in table by name.
     *
     * @param[in] identifierName Identifier name.
     * @return Index of identifier in table or -1 when identifier not exists.
     */
    int getIdentifierIndex(QString identifierName);

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
    QString errorText;

    QHash <QString, KeyWordType> keyWordsHash;
    QHash <QString, Token::TokenCategory> oneCharacterTokensHash;
    QHash <QString, Token::TokenCategory> twoCharacterTokensHash;
    bool isHashEmpty();

};

QString TokenListListToString(QList<QList<Token> > tokenListList);

#endif // LEXICALANALYZER_H
