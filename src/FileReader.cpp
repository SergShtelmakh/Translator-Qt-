#include "FileReader.h"
#include <QFile>
#include <QTextStream>

FileReader::FileReader()
{

}

FileReader::~FileReader()
{

}

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
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

    QTextStream out(&file);
    out << text;
}

