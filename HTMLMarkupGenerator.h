/*!
 * \file HTMLMarkupGenerator.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef LEXICALANALYSISHTMLMARKUPGENERATOR_H
#define LEXICALANALYSISHTMLMARKUPGENERATOR_H

#include <QString>

class LexicalAnalyzer;
class SyntacticAnalyzer;
class Token;

/*!
 * @brief This class provides HTML markup.
 */
class HTMLMarkupGenerator
{

public:

    /*!
     * This method returns result of lexical analysis.
     *
     * @param[in] analyzer Lexical analyzer.
     * @return Result of lexical analysis.
     */
    QString getMessageForLog(const LexicalAnalyzer &lexicalAnalyzer, const SyntacticAnalyzer &syntacticAnalyzer) const;

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
     * @param[in] analyzerName Ananyzer name.
     * @return String with successful result.
     */
    QString getSuccessfulResultMessage(const QString &analyzerName) const;

    /*!
     * This method returns string with failed result and errors list.
     *
     * @param[in] analyzerName Ananyzer name.
     * @param[in] errors Errors string.
     * @return String with failed result and errors list.
     */
    QString getFailedResultMessage(const QString &analyzerName, QString errors) const;


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
QString PlainTextToHTML(const QString &plainText);

#endif // LEXICALANALYSISHTMLMARKUPGENERATOR_H
