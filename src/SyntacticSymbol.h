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

    SyntacticSymbol(QString name, SyntacticSymbolType type, Token::TokenCategory category = Token::categoryNone);

    QString name() const;
    void setName(const QString &name);

    SyntacticSymbolType type() const;
    void setType(const SyntacticSymbolType &type);

    Token::TokenCategory category() const;
    void setCategory(const Token::TokenCategory &category);

private:

    QString m_name;
    SyntacticSymbolType m_type;
    Token::TokenCategory m_category;

};

bool operator==(const SyntacticSymbol &symbol1,const SyntacticSymbol &symbol2);
bool operator==(const Token &token,const SyntacticSymbol &symbol);
bool operator==(const SyntacticSymbol &symbol,const Token &token);

uint qHash(const SyntacticSymbol &symbol);

#endif // SYNTACTICSYMBOL_H
