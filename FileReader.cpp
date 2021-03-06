#include "FileReader.h"
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "SyntacticSymbol.h"
#include "BackusNaurFormParser.h"
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "Translator.h"

QString FileReader::getTextFromFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return QString();
    QFile file (fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return QString();

    QTextStream inputTextStream(&file);
    return inputTextStream.readAll();
}

void FileReader::writeTextToFile(const QString &fileName, const QString &text)
{
    if (fileName.isEmpty())
        return;
    QFile file (fileName);
    if (!file.open(QIODevice::ReadWrite| QIODevice::Text))
        return;

    QTextStream out(&file);
    out << text;
}

void FileReader::loadLexicalAnalyzerSettings(const QString &fileName, Translator *translator)
{
    QJsonDocument jsonDocument = loadJson(fileName);
    QJsonObject mainObject = jsonDocument.object();
    QJsonObject lexicalAnalyzerJsonObject = mainObject["LexicalAnalyser"].toObject();
    loadLexicalAnalyzerSettings(lexicalAnalyzerJsonObject, translator);
}

void FileReader::loadSyntacticAnalyzerRules(const QString &fileName, Translator *translator)
{
    QString sourceText = getTextFromFile(fileName);
    QList <BackusNaurFormRule> rulesList;
    rulesList = BackusNaurFormParser::parse(sourceText);
    SyntacticAnalyzer *syntacticAnalyzer = translator->syntacticAnalyzer();
    foreach (BackusNaurFormRule rule, rulesList) {
        syntacticAnalyzer->addProductRule(rule.leftPart(), rule.rightPart());
    }
}

bool FileReader::isFileExist(const QString &fileName)
{
    QFile file (fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    return true;
}

void FileReader::loadLexicalAnalyzerSettings(const QJsonObject &jsonObject, Translator *translator)
{
    LexicalAnalyzer *lexicalAnalyzer = translator->lexicalAnalyzer();
    lexicalAnalyzer->setMaxNumberLiteralLenght(jsonObject["maxNumberLiteralLenght"].toInt());
    lexicalAnalyzer->setMaxIdentifierNameLenght(jsonObject["maxIdentifierNameLenght"].toInt());
    lexicalAnalyzer->setMaxStringLiteralLenght(jsonObject["maxStringLiteralLenght"].toInt());
    lexicalAnalyzer->setIdentifierRegExp(QRegExp(jsonObject["identifierRegExp"].toString()));
    lexicalAnalyzer->setSpaceRegExp(QRegExp(jsonObject["spaceRegExp"].toString()));
    lexicalAnalyzer->setBeginStringLiteral(jsonObject["beginStringLiteral"].toString());

    QJsonArray keywordsArray = jsonObject["keywords"].toArray();
    foreach (const QJsonValue& value, keywordsArray) {
        lexicalAnalyzer->addKeyword(value.toString());
    }

    QJsonArray characterTokenArray = jsonObject["characterToken"].toArray();
    foreach (const QJsonValue& value, characterTokenArray) {
        lexicalAnalyzer->addCharacterToken(value.toString());
    }
}

QJsonDocument FileReader::loadJson(const QString &fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}


