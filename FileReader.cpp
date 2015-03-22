#include "FileReader.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "SyntacticSymbol.h"
#include "BackusNaurFormParser.h"

QString FileReader::getTextFromFile(QString fileName)
{
    QString text;
    QFile file (fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return text;

    QTextStream inputTextStream(&file);
    QString line = inputTextStream.readLine();
    while (!line.isNull()) {
        text += line + "\n";
        line = inputTextStream.readLine();
    }
    return text;
}

void FileReader::writeTextToFile(QString fileName, QString text)
{
    QFile file (fileName);
    if (!file.open(QIODevice::ReadWrite| QIODevice::Text))
        return;

    QTextStream out(&file);
    out << text;
}

void FileReader::loadLexicalAnalyzerSettings(QString fileName, LexicalAnalyzer& lexicalAnalyzer)
{
    QJsonDocument jsonDocument = loadJson(fileName);
    QJsonObject mainObject = jsonDocument.object();

    QJsonObject lexicalAnalyzerJsonObject = mainObject["LexicalAnalyser"].toObject();
    loadLexicalAnalyzerSettings(lexicalAnalyzerJsonObject,lexicalAnalyzer);
}

void FileReader::loadSyntacticAnalyzerRules(QString fileName, SyntacticAnalyzer &syntacticAnalyzer)
{
    QString rulesString = getTextFromFile(fileName);
    QList <BackusNaurFormRule> rulesList;
    rulesList = BackusNaurFormParser::parse(rulesString);
    foreach (BackusNaurFormRule rule, rulesList) {
        syntacticAnalyzer.addProductRule(rule.leftPart(),rule.rightPart());
    }
}

bool FileReader::isFileExist(QString fileName)
{
    QFile file (fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    return true;
}

void FileReader::loadLexicalAnalyzerSettings(QJsonObject jsonObject, LexicalAnalyzer& lexicalAnalyzer)
{
    lexicalAnalyzer.setMaxNumberLiteralLenght(jsonObject["maxNumberLiteralLenght"].toInt());
    lexicalAnalyzer.setMaxIdentifierNameLenght(jsonObject["maxIdentifierNameLenght"].toInt());
    lexicalAnalyzer.setMaxStringLiteralLenght(jsonObject["maxStringLiteralLenght"].toInt());
    lexicalAnalyzer.setIdentifierRegExp(QRegExp(jsonObject["identifierRegExp"].toString()));
    lexicalAnalyzer.setSpaceRegExp(QRegExp(jsonObject["spaceRegExp"].toString()));
    lexicalAnalyzer.setBeginStringLiteral(jsonObject["beginStringLiteral"].toString());

    QJsonArray keywordsArray = jsonObject["keywords"].toArray();
    foreach (const QJsonValue& value, keywordsArray) {
        lexicalAnalyzer.addKeyword(value.toString());
    }

    QJsonArray characterTokenArray = jsonObject["characterToken"].toArray();
    foreach (const QJsonValue& value, characterTokenArray) {
        lexicalAnalyzer.addCharacterToken(value.toString());
    }
}

QJsonDocument FileReader::loadJson(QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}


