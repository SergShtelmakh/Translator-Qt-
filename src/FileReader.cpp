#include "FileReader.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "SyntacticSymbol.h"

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

void FileReader::loadAnalyzerSettings(QString fileName, LexicalAnalyzer& lexicalAnalyzer, SyntacticAnalyzer &syntacticAnalyzer)
{
    QJsonDocument jsonDocument = loadJson(fileName);
    QJsonObject mainObject = jsonDocument.object();

    QJsonObject lexicalAnalyzerJsonObject = mainObject["LexicalAnalyser"].toObject();
    loadLexicalAnalyzerSettings(lexicalAnalyzerJsonObject,lexicalAnalyzer);

    QJsonObject syntacticAnalyzerJsonObject = mainObject["SyntacticAnalyzer"].toObject();
    loadSyntacticAnalyzerSettings(syntacticAnalyzerJsonObject,syntacticAnalyzer);

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

void FileReader::loadSyntacticAnalyzerSettings(QJsonObject syntacticAnalyzerJsonObject, SyntacticAnalyzer &syntacticAnalyzer)
{
    QJsonArray productRulesJsonArray = syntacticAnalyzerJsonObject["ProductRules"].toArray();
    foreach (const QJsonValue& produtRuleJsonValue, productRulesJsonArray) {
        QJsonObject productRuleJsonObject = produtRuleJsonValue.toObject();

        QJsonObject leftPartJsonObject = productRuleJsonObject["LeftPart"].toObject();

        QList <SyntacticSymbol> rightSyntacticSymbolList;
        QJsonArray rightPartJsonArray = productRuleJsonObject["RightPart"].toArray();
        foreach (const QJsonValue& rightPartSymbolJsonValue, rightPartJsonArray) {
            QJsonObject rightPartJsonObject = rightPartSymbolJsonValue.toObject();
            rightSyntacticSymbolList << jsonObjectToSyntacticSymbol(rightPartJsonObject);
        }

        syntacticAnalyzer.addProductRule(jsonObjectToSyntacticSymbol(leftPartJsonObject),rightSyntacticSymbolList);
    }
}

QJsonDocument FileReader::loadJson(QString fileName)
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

SyntacticSymbol jsonObjectToSyntacticSymbol(QJsonObject object)
{
    SyntacticSymbol syntacticSymbol = SyntacticSymbol(object["name"].toString(),
            object["symbolType"].toString(),
            object["tokenCategory"].toString());
    return syntacticSymbol;
}


