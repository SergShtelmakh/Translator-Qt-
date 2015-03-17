#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QHash>
#include <QRegExp>
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

    LexicalAnalyzer();

    /*!
     * This method makes lexical analysis.
     *
     * @param[in] sourceCode Source code for lexical analysis.
     */
    void analyze(QString sourceCode);

    QString errorText() const;
    QList<QList<Token> > tokenListList() const;
    QList<Identifier> identifierList() const;

    /*!
     * This method searches identifier in table by name.
     *
     * @param[in] identifierName Identifier name.
     * @return Index of identifier in table or -1 when identifier not exists.
     */
    int getIdentifierIndex(QString identifierName);

    int maxNumberLiteralLenght() const;
    void setMaxNumberLiteralLenght(int maxNumberLiteralLenght);

    int maxStringLiteralLenght() const;
    void setMaxStringLiteralLenght(int maxStringLiteralLenght);

    int maxIdentifierNameLenght() const;
    void setMaxIdentifierNameLenght(int maxIdentifierNameLenght);

    void addKeyword(QString keyword, KeyWordType keywordType);
    void addCharacterToken(QString lexeme, Token::TokenCategory tokenCategory);

    QString beginStringLiteral() const;
    void setBeginStringLiteral(const QString &beginStringLiteral);

    QRegExp spaceRegExp() const;
    void setSpaceRegExp(const QRegExp &spaceRegExp);

protected:

    Token getNextToken(QString sourceString);
    Token getSpaceToken(QString sourceString);
    Token getNumberLiteralToken(QString sourceString);
    Token getKeyWordToken(QString sourceString);
    Token getIdentifierToken(QString sourceString);
    Token getStringLiteralToken(QString sourceString);
    Token getCharacterToken(QString sourceString);

private:

    void analyzeLine(QString sourceLine, int lineNumber);
    void addIdentifier(Identifier);
    void addError(QString);
    void clearAllAnalyzingData();

    QList <Identifier> m_identifierList;
    QList < QList <Token> > m_tokenListList;
    QString m_errorText;

    QHash <QString, KeyWordType> m_keyWordsHash;
    QHash <QString, Token::TokenCategory> m_definedCharacterTokensHash;

    QRegExp m_possibleTokenEndRegExp;   //!< RegExp that can separate tokens.
    QRegExp m_numberLiteralRegExp;      //!< RegExp for searching number literals.
    QRegExp m_identifierRegExp;         //!< RegExp for searching identifiers.
    QRegExp m_spaceRegExp;
    QString m_beginStringLiteral;

    int m_maxNumberLiteralLenght;       //!< Maximum number literal lenght.
    int m_maxStringLiteralLenght;       //!< Maximum string literal lenght.
    int m_maxIdentifierNameLenght;      //!< Maximum identifier name lenght.
    int m_maxCharacterTokensLenght;
};

QString TokenListListToString(QList<QList<Token> > tokenListList);
QRegExp AddPossibleVariantToRegExpPattern(QRegExp oldRegExp, QString string);

#endif // LEXICALANALYZER_H
