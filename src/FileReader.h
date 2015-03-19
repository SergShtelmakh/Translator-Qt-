#ifndef FILEREADER_H
#define FILEREADER_H

#include <QString>

class FileReader
{

public:

    FileReader();
    ~FileReader();

    static QString getTextFromFile(QString fileName);

    static void writeTextToFile(QString fileName, QString text);

private:



};

#endif // FILEREADER_H
