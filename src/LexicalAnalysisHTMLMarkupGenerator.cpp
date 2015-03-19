#include <QTime>
#include <QRegExp>
#include <QStringList>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "LexicalAnalysisHTMLMarkupGenerator.h"
#include "HTMLColors.h"

QString LexicalAnalysisHTMLMarkupGenerator::getMessageForLog(const LexicalAnalyzer& analyzer) const
{
    QString newLineInLog;

    if (analyzer.errorText().isEmpty()) {
        newLineInLog = this->getSuccessfulResultMessage();
    } else {
        newLineInLog = this->getFailedResultMessage(analyzer.errorText());
    }
    return newLineInLog;
}

QString LexicalAnalysisHTMLMarkupGenerator::getSuccessfulResultMessage() const
{
    QString newLine = "<font color=" + HTMLColors::darkBlue + ">"
            + "Lexical analysis completed successfully "
            + QTime::currentTime().toString() + "\n</font>";
    return newLine;
}

QString LexicalAnalysisHTMLMarkupGenerator::getFailedResultMessage(QString errors) const
{
    int errorCount = errors.split(QRegExp("\n")).count() - 1;
    QString newLine = "<font color=" + HTMLColors::red + ">"
            + "Lexical analyzer:\n"
            + errors
            + QString("Detected %1 errors    ").arg(errorCount)
            + QTime::currentTime().toString() + "\n</font>";

    return newLine;
}

QString LexicalAnalysisHTMLMarkupGenerator::getTokenColorName(const Token& token) const
{
    switch (token.tokenCategory()) {
    case Token::categoryIdentifier:
        return HTMLColors::blue;
    case Token::categoryCharToken:
        return HTMLColors::green;
    case Token::categoryNumberLiteral:
        return HTMLColors::violet;
    case Token::categoryKeyword:
        return HTMLColors::gold;
    case Token::categoryStringLiteral:
        return HTMLColors::red;
    default:
        return "";
    }
}

QString LexicalAnalysisHTMLMarkupGenerator::getTokenHTMLRepresentation(const Token &token) const
{
    // In HTML &lt; and &gt; use to represent < and >
    QString lexeme = token.lexeme().replace("<","&lt;").replace(">","&gt;");

    // Underlining incorrect lexeme
    if (!token.isCorrect())
        return "<u>" + lexeme + "</u>";

    // Spaces represent without markup
    if (token.tokenCategory() == Token::categorySpace)
        return lexeme;

    // In HTML <br> use as line feed
    if (token.tokenCategory() == Token::categoryLineFeed)
        return "<br>";

    return "<font color=" + getTokenColorName(token) + ">" + lexeme + "</font>";
}

QString LexicalAnalysisHTMLMarkupGenerator::getSourceCodeHTMLMarkup(const LexicalAnalyzer& analyzer) const
{
    QString html;

    foreach (Token currentToken, analyzer.tokenList()) {
        html += getTokenHTMLRepresentation(currentToken);
    }
    return PlainTextToHTML(html);
}


QString PlainTextToHTML(QString plainText)
{
    QString html = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
            + plainText
            + "</body></html>";
    html.replace("\n","<br>");
    return html;
}
