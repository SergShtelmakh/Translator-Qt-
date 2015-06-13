#include <QTime>
#include <QRegExp>
#include <QStringList>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "ThreeAddressCodeGenerator.h"
#include "HTMLMarkupGenerator.h"
#include "HTMLColors.h"

QString HTMLMarkupGenerator::getMessageForLog(const LexicalAnalyzer& lexicalAnalyzer, const SyntacticAnalyzer &syntacticAnalyzer, const SemanticAnalyzer &semanticAnalyzer, const ThreeAddressCodeGenerator &codeGenerator) const
{
    QString messageForLog;

    if (lexicalAnalyzer.errorText().isEmpty()) {
        messageForLog += this->getSuccessfulResultMessage("Lexical analyzer");
    } else {
        messageForLog += this->getFailedResultMessage("Lexical analyzer", lexicalAnalyzer.errorText());
    }

    if (syntacticAnalyzer.errorText().isEmpty()) {
        messageForLog += this->getSuccessfulResultMessage("Syntactic analyzer");
    } else {
        messageForLog += this->getFailedResultMessage("Syntactic analyzer", syntacticAnalyzer.errorText());
    }

    if (lexicalAnalyzer.errorText().isEmpty()&&syntacticAnalyzer.errorText().isEmpty()) {
        if (semanticAnalyzer.errorText().isEmpty()) {
            messageForLog += this->getSuccessfulResultMessage("Semantic analyzer");
        } else {
            messageForLog += this->getFailedResultMessage("Semantic analyzer", semanticAnalyzer.errorText());
        }
    }

    if (lexicalAnalyzer.errorText().isEmpty()&&syntacticAnalyzer.errorText().isEmpty()&&semanticAnalyzer.errorText().isEmpty()) {
        if (codeGenerator.error().isEmpty()) {
            messageForLog += this->getSuccessfulResultMessage("Code generator");
        } else {
            messageForLog += this->getFailedResultMessage("Code generator", codeGenerator.error());
        }
    }
    return messageForLog;
}

QString HTMLMarkupGenerator::getSuccessfulResultMessage(const QString &analyzerName) const
{
    return QString("<font color=" + HTMLColors::darkBlue + ">"
                   + analyzerName + ":\n"
                   + "Completed successfully "
                   + QTime::currentTime().toString() + "\n</font>");
}

QString HTMLMarkupGenerator::getFailedResultMessage(const QString &analyzerName, QString errors) const
{
    errors.replace("<","&lt;").replace(">","&gt;");
    int errorCount = errors.split(QRegExp("\n")).count() - 1;
    return QString("<font color=" + HTMLColors::red + ">"
                   + analyzerName + ":\n"
                   + errors
                   + QString("Detected %1 errors    ").arg(errorCount)
                   + QTime::currentTime().toString() + "\n</font>");
}

QString HTMLMarkupGenerator::getTokenColorName(const Token& token) const
{
    switch (token.category()) {
    case Token::IDENTIFIER_CATEGORY:
        return HTMLColors::blue;
    case Token::CHAR_TOKEN_CATEGORY:
        return HTMLColors::green;
    case Token::NUMBER_LITERAL_CATEGORY:
        return HTMLColors::violet;
    case Token::KEYWORD_CATEGORY:
        return HTMLColors::gold;
    case Token::STRING_LITERAL_CATEGORY:
        return HTMLColors::red;
    default:
        return "";
    }
}

QString HTMLMarkupGenerator::getTokenHTMLRepresentation(const Token &token) const
{
    // In HTML &lt; and &gt; use to represent < and >
    QString lexeme = token.lexeme().replace("<", "&lt;").replace(">", "&gt;");

    // Underlining incorrect lexeme
    if (!token.isCorrect())
        return "<u>" + lexeme + "</u>";

    // Spaces represent without markup
    if (token.category() == Token::SPACE_CATEGORY)
        return lexeme;

    // In HTML <br> use as line feed
    if (token.category() == Token::LINE_FEED_CATEGORY)
        return "<br>";

    return "<font color=" + getTokenColorName(token) + ">" + lexeme + "</font>";
}

QString HTMLMarkupGenerator::getSourceCodeHTMLMarkup(const LexicalAnalyzer& analyzer) const
{
    QString plainTextWithMarkedUpTokens;

    foreach (Token currentToken, analyzer.tokenList()) {
        plainTextWithMarkedUpTokens += getTokenHTMLRepresentation(currentToken);
    }
    return PlainTextToHTML(plainTextWithMarkedUpTokens);
}


QString PlainTextToHTML(const QString &plainText)
{
    QString html = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
            + plainText
            + "</body></html>";
    html.replace("\n", "<br>");
    return html;
}
