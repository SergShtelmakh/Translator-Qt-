#include <QTime>

#include "Token.h"
#include "LexicalAnalyzer.h"
#include "LexicalAnalysisHTMLMarkupGenerator.h"
#include "HTMLColors.h"

QString LexicalAnalysisHTMLMarkupGenerator::getMessageForLog(const LexicalAnalyzer& analyzer) const
{
    QString newLineInLog;

    if (analyzer.getErrorText().isEmpty()) {
        newLineInLog = this->getSuccessfulResultMessage();
    } else {
        newLineInLog = this->getFailedResultMessage(analyzer.getErrorText());
    }

    return newLineInLog;
}

QString LexicalAnalysisHTMLMarkupGenerator::getSuccessfulResultMessage() const
{
    QString newLine = "<font color=" + HTMLColors::darkBlue + ">Lexical analysis completed successfully ";
    newLine += QTime::currentTime().toString() + "\n</font>";

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
    switch (token.getTokenCategory()) {
    case Token::categoryIdentifier:
        return HTMLColors::blue;
    case Token::categoryOperation:
        return HTMLColors::green;
    case Token::categoryNumberLiteral:
        return HTMLColors::violet;
    case Token::categoryComparator:
        return HTMLColors::green;
    case Token::categoryComma:
        return HTMLColors::green;
    case Token::categoryBracket:
        return HTMLColors::green;
    case Token::categoryKeyWord:
        return HTMLColors::gold;
    case Token::categoryStringLiteral:
        return HTMLColors::red;
    default:
        return "";
    }
}

QString LexicalAnalysisHTMLMarkupGenerator::getTokenHTMLRepresentation(const Token &token) const
{
    QString lexeme = token.getLexeme().replace("<","&lt;").replace(">","&gt;");
    if(!token.isCorrect()){
        return "<u>" + lexeme + "</u>";
    }
    if(token.getTokenCategory() == Token::categorySpace){
        return lexeme;
    }
    if(token.getTokenCategory() == Token::categoryLineFeed){
        return "<br>";
    }
    return "<font color=" + getTokenColorName(token) + ">" + lexeme + "</font>";
}

QString LexicalAnalysisHTMLMarkupGenerator::getSourceCodeHTMLMarkup(const LexicalAnalyzer& analyzer) const
{
    QString html;
    foreach (QList < Token > currentTokenSubList, analyzer.getTokenListList()) {
        foreach (Token currentToken, currentTokenSubList) {
            html += getTokenHTMLRepresentation(currentToken);
        }
    }
    return PlainTextToHTML(html);
}


QString PlainTextToHTML(QString plainText)
{
    QStringList plainTextList = plainText.split("\n");
    QString html = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>";
    for(int currentTextLine = 0; currentTextLine < plainTextList.count() - 1; currentTextLine++){
        html += plainTextList.at(currentTextLine) + "<br>";
    }
    html += plainTextList.last() + "</body></html>";
    return html;
}
