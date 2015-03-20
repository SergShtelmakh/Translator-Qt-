#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QJsonDocument>
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"

class FileReader
{

public:

    FileReader();
    ~FileReader();

    static QString getTextFromFile(QString fileName);

    static void writeTextToFile(QString fileName, QString text);

    static void loadAnalyzerSettings(QString fileName, LexicalAnalyzer &lexicalAnalyzer, SyntacticAnalyzer &syntacticAnalyzer);

    static bool isFileExist(QString fileName);

private:

    static QJsonDocument loadJson(QString fileName);

    static void loadLexicalAnalyzerSettings(QJsonObject lexicalAnalyzerJsonObject, LexicalAnalyzer &lexicalAnalyzer);

    static void loadSyntacticAnalyzerSettings(QJsonObject syntacticAnalyzerJsonObject, SyntacticAnalyzer &syntacticAnalyzer);

};

SyntacticSymbol jsonObjectToSyntacticSymbol(QJsonObject object);

#endif // FILEREADER_H
