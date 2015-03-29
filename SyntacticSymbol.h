#ifndef SYNTACTICSYMBOL_H
#define SYNTACTICSYMBOL_H

#include <QString>
#include "Token.h"

class Token;

class SyntacticSymbol
{

public:

    enum SyntacticSymbolType{
        nonterminalSymbol   = 0,
        startSymbol         = 1,
        terminalSymbol      = 2
    };

    SyntacticSymbol(){}
    SyntacticSymbol(QString name, SyntacticSymbolType type, Token::TokenCategory category = Token::categoryNone);
    SyntacticSymbol(QString name, QString type, QString category);

    QString name() const;
    void setName(const QString &name);

    SyntacticSymbolType type() const;
    void setType(const SyntacticSymbolType &type);

    Token::TokenCategory category() const;
    void setCategory(const Token::TokenCategory &category);

    static SyntacticSymbolType stringToSyntacticSymbolType(QString string);

    bool isCorrect();

private:

    QString m_name;
    SyntacticSymbolType m_type;
    Token::TokenCategory m_category;

    static QHash <QString, SyntacticSymbolType> m_convertingStringToSyntacticSymbolTypeHash;

};

bool operator==(const SyntacticSymbol &symbol1,const SyntacticSymbol &symbol2);
bool operator==(const Token &token,const SyntacticSymbol &symbol);
bool operator==(const SyntacticSymbol &symbol,const Token &token);

uint qHash(const SyntacticSymbol &symbol);

QString MakeString(QList<SyntacticSymbol> syntacticSymbolList);
QString MakeString(SyntacticSymbol syntacticSymbol);

#endif // SYNTACTICSYMBOL_H
