#ifndef LEXICALANALYSISHTMLMARKUPGENERATOR_H
#define LEXICALANALYSISHTMLMARKUPGENERATOR_H

#include <QString>

class LexicalAnalyzer;
class Token;

/*!
 * @brief This class provides HTML markup.
 */
class LexicalAnalysisHTMLMarkupGenerator
{

public:

    /*!
     * This method returns result of lexical analysis.
     *
     * @param[in] analyzer Lexical analyzer.
     * @return Result of lexical analysis.
     */
    QString getMessageForLog(const LexicalAnalyzer &analyzer) const;

    /*!
     * This method returns marked up source code.
     *
     * @param[in] analyzer Lexixal analyzer.
     * @return Marked up source code.
     */
    QString getSourceCodeHTMLMarkup(const LexicalAnalyzer &analyzer) const;

private:

    /*!
     * This method returns string with successful result.
     *
     * @return String with successful result.
     */
    QString getSuccessfulResultMessage() const;

    /*!
     * This method returns string with failed result and errors list.
     *
     * @return String with failed result and errors list.
     */
    QString getFailedResultMessage(QString errors) const;

    /*!
     * This method returns string with color code of token.
     *
     * @param[in] token Token.
     * @return String with color code.
     */
    QString getTokenColorName(const Token &token) const;

    /*!
     * This method returns string with marked up tokens lexeme. Use to markup source code.
     *
     * @param[in] token Token.
     * @return String with marked up tokens lexeme.
     */
    QString getTokenHTMLRepresentation(const Token &token) const;

};

/*!
 * This function adds tags to plain text and creates HTML.
 *
 * @param[in] plainText Plain text.
 * @return HTML representation of plain text.
 */
QString PlainTextToHTML(QString plainText);

#endif // LEXICALANALYSISHTMLMARKUPGENERATOR_H
