/*!
 * \file SyntacticSymbol.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef SYNTACTICSYMBOL_H
#define SYNTACTICSYMBOL_H

#include <QString>
#include "Token.h"

class SyntacticSymbol
{

public:

    enum SyntacticSymbolType {
        NONTERMINAL_SYMBOL   = 0,
        START_SYMBOL         = 1,
        TERMINAL_SYMBOL      = 2
    };

    SyntacticSymbol(){}
    SyntacticSymbol(const QString &name, SyntacticSymbolType type, Token::TokenCategory category = Token::NONE_CATEGORY);
    SyntacticSymbol(const QString &name, const QString &type, const QString &category);

    QString name() const;
    void setName(const QString &name);

    SyntacticSymbolType type() const;
    void setType(const SyntacticSymbolType &type);

    Token::TokenCategory category() const;
    void setCategory(const Token::TokenCategory &category);

    static SyntacticSymbolType stringToSyntacticSymbolType(const QString &string);

    bool isCorrect() const;

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

QString MakeStringRepresentation(const QList<SyntacticSymbol> &syntacticSymbolList);
QString MakeStringRepresentation(const SyntacticSymbol &syntacticSymbol);

#endif // SYNTACTICSYMBOL_H
