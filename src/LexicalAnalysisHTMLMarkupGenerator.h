#ifndef LEXICALANALYSISHTMLMARKUPGENERATOR_H
#define LEXICALANALYSISHTMLMARKUPGENERATOR_H

#include <QString>

class LexicalAnalyzer;
class Token;

/*!
 * @brief This class provides HTML markup.
 *
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

    QString getSuccessfulResultMessage() const;
    QString getFailedResultMessage(QString errors) const;

    QString getTokenColorName(const Token &token) const;
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
