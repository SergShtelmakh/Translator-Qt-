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
class Translator;

class FileReader
{

public:

    static QString getTextFromFile(const QString &fileName);

    static void writeTextToFile(const QString &fileName, const QString &text);

    static void loadLexicalAnalyzerSettings(const QString &fileName, Translator *translator);

    static void loadSyntacticAnalyzerRules(const QString &fileName, Translator *translator);

    static bool isFileExist(const QString &fileName);

private:

    static QJsonDocument loadJson(const QString &fileName);

    static void loadLexicalAnalyzerSettings(const QJsonObject &lexicalAnalyzerJsonObject, Translator *translator);

};


#endif // FILEREADER_H
