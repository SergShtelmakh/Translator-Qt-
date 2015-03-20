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

    LexicalAnalyzer();

    QString errorText() const;

    QList <Token> tokenList() const;

    QList <Identifier> identifierList() const;

    int maxNumberLiteralLenght() const;
    void setMaxNumberLiteralLenght(int maxNumberLiteralLenght);

    int maxStringLiteralLenght() const;
    void setMaxStringLiteralLenght(int maxStringLiteralLenght);

    int maxIdentifierNameLenght() const;
    void setMaxIdentifierNameLenght(int maxIdentifierNameLenght);

    QString beginStringLiteral() const;
    void setBeginStringLiteral(const QString &beginStringLiteral);

    QRegExp spaceRegExp() const;
    void setSpaceRegExp(const QRegExp &spaceRegExp);

    QRegExp identifierRegExp() const;
    void setIdentifierRegExp(const QRegExp &identifierRegExp);

    /*!
     * This method makes lexical analysis.
     *
     * @param[in] sourceCode Source code for lexical analysis.
     */
    void analyze(QString sourceCode);

    /*!
     * This method searches identifier in table by name.
     *
     * @param[in] identifierName Identifier name.
     * @return Index of identifier in table or -1 when identifier not exists.
     */
    int getIdentifierIndex(QString identifierName);

    /*!
     * This method returns tokens list without space tokens.
     *
     * @return Tokens list without space tokens.
     */
    QList<Token> getTokenListWithoutSpaces() const;

    /*!
     * This method used to add keywords of program language. (e.g. int, void, double, const ...)
     *
     * @param[in] identifierName Keyword.
     */
    void addKeyword(QString keyword);

    /*!
     * This method used to add character tokens of program language. (e.g. *, <, >, = ...)
     *
     * @param[in] characterToken Character token.
     */
    void addCharacterToken(QString characterToken);



protected:

    /*!
     * This method used to get token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Next token.
     */
    Token getNextToken(QString sourceString);

    /*!
     * This method used to get space token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Space token.
     */
    Token getSpaceToken(QString sourceString);

    /*!
     * This method used to get number literal token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Number literal token.
     */
    Token getNumberLiteralToken(QString sourceString);

    /*!
     * This method used to get keyword token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Keyword token.
     */
    Token getKeywordToken(QString sourceString);

    /*!
     * This method used to get identifier token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Identifier token.
     */
    Token getIdentifierToken(QString sourceString);

    /*!
     * This method used to get string literal token from source code.
     *
     * @param[in] sourceString Source string.
     * @return string literal token.
     */
    Token getStringLiteralToken(QString sourceString);

    /*!
     * This method used to get character token from source code.
     *
     * @param[in] sourceString Source string.
     * @return Character token.
     */
    Token getCharacterToken(QString sourceString);

private:

    /*!
     * This method used to analyse line of source code.
     *
     * @param[in] sourceCodeLine Source code line.
     * @param[in] lineNumber Line number.
     */
    void analyzeLine(QString sourceCodeLine, int lineNumber);

    /*!
     * This method used to add identifier to identifiers list.
     *
     * @param[in] identifier Identifier.
     */
    void addIdentifier(Identifier identifier);

    /*!
     * This method used to add analysis errors.
     *
     * @param[in] errorString Errors string.
     */
    void addError(QString errorString);

    /*!
     * This method used to clear token list, identifiers list and errors of last analysis.
     */
    void clearAllAnalyzingData();

    QList <Identifier> m_identifierList;        //!< List of identifiers.
    QList <Token> m_tokenList;                  //!< List of tokens.
    QString m_errorText;                        //!< Errors string.

    QHash <QString, int> m_keyWordsHash;        //!< Hash with keywords.
    QHash <QString, int> m_characterTokensHash; //!< Hash with character tokens.

    QRegExp m_possibleTokenEndRegExp;           //!< RegExp that can separate tokens.
    QRegExp m_numberLiteralRegExp;              //!< RegExp for searching number literals.
    QRegExp m_identifierRegExp;                 //!< RegExp for searching identifiers.
    QRegExp m_spaceRegExp;                      //!< RegExp for searching spaces.
    QString m_beginStringLiteral;               //!< RegExp for searching string literal.

    int m_maxNumberLiteralLenght;               //!< Maximum number literal lenght.
    int m_maxStringLiteralLenght;               //!< Maximum string literal lenght.
    int m_maxIdentifierNameLenght;              //!< Maximum identifier name lenght.
    int m_maxCharacterTokensLenght;             //!< Maximum character token lenght.

};

/*!
 * This function used to convert list of tokens to string.
 *
 * @param[in] tokenList List of tokens.
 * @return List of tokens as string.
 */
QString TokenListToString(QList<Token> tokenList);

/*!
 * This function used to add possible variant to regular expression.
 *
 * @param[in] oldRegExp Regular expression.
 * @param[in] string Possible variant.
 * @return New regular expression.
 */
QRegExp AddPossibleVariantToRegExpPattern(QRegExp oldRegExp, QString string);

#endif // LEXICALANALYZER_H
