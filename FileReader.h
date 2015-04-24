/*!
 * \file FileReader.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>
#include <QJsonDocument>

class LexicalAnalyzer;
class SyntacticAnalyzer;

class FileReader
{

public:

    static QString getTextFromFile(const QString &fileName);

    static void writeTextToFile(const QString &fileName, const QString &text);

    static void loadLexicalAnalyzerSettings(const QString &fileName, LexicalAnalyzer &lexicalAnalyzer);

    static void loadSyntacticAnalyzerRules(const QString &fileName, SyntacticAnalyzer &syntacticAnalyzer);

    static bool isFileExist(const QString &fileName);

private:

    static QJsonDocument loadJson(const QString &fileName);

    static void loadLexicalAnalyzerSettings(const QJsonObject &lexicalAnalyzerJsonObject, LexicalAnalyzer &lexicalAnalyzer);

};


#endif // FILEREADER_H
