#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QJsonDocument>
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"

class FileReader
{

public:

    static QString getTextFromFile(QString fileName);

    static void writeTextToFile(QString fileName, QString text);

    static void loadLexicalAnalyzerSettings(QString fileName, LexicalAnalyzer &lexicalAnalyzer);

    static void loadSyntacticAnalyzerRules(QString fileName, SyntacticAnalyzer &syntacticAnalyzer);

    static bool isFileExist(QString fileName);

private:

    static QJsonDocument loadJson(QString fileName);

    static void loadLexicalAnalyzerSettings(QJsonObject lexicalAnalyzerJsonObject, LexicalAnalyzer &lexicalAnalyzer);

};


#endif // FILEREADER_H
