#ifndef LEXICALANALYSISHTMLMARKUPGENERATOR_H
#define LEXICALANALYSISHTMLMARKUPGENERATOR_H

#include <QString>

class LexicalAnalyzer;
class Token;

class LexicalAnalysisHTMLMarkupGenerator
{
public:

    QString getMessageForLog(const LexicalAnalyzer& analyzer) const;
    QString getSourceCodeHTMLMarkup(const LexicalAnalyzer& analyzer) const;

private:
    QString getSuccessfulResultMessage() const;
    QString getFailedResultMessage(QString errors) const;

    QString getTokenColorName(const Token& token) const;
    QString getTokenHTMLRepresentation(const Token& token) const;
};

QString PlainTextToHTML(QString plainText);

#endif // LEXICALANALYSISHTMLMARKUPGENERATOR_H
