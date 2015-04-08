#include "SyntacticSymbol.h"
#include <QHash>

QHash <QString, SyntacticSymbol::SyntacticSymbolType> SyntacticSymbol::m_convertingStringToSyntacticSymbolTypeHash;

SyntacticSymbol::SyntacticSymbol(const QString &name, SyntacticSymbol::SyntacticSymbolType type, Token::TokenCategory category) :
    m_name(name),
    m_type(type),
    m_category(category)
{}

SyntacticSymbol::SyntacticSymbol(const QString &name, const QString &type, const QString &category) :
    m_name(name),
    m_type(SyntacticSymbol::stringToSyntacticSymbolType(type)),
    m_category(Token::stringToTokenCategory(category))
{}

QString SyntacticSymbol::name() const
{
    return m_name;
}

void SyntacticSymbol::setName(const QString &name)
{
    m_name = name;
}

SyntacticSymbol::SyntacticSymbolType SyntacticSymbol::type() const
{
    return m_type;
}

void SyntacticSymbol::setType(const SyntacticSymbolType &type)
{
    m_type = type;
}

Token::TokenCategory SyntacticSymbol::category() const
{
    return m_category;
}

void SyntacticSymbol::setCategory(const Token::TokenCategory &category)
{
    m_category = category;
}

SyntacticSymbol::SyntacticSymbolType SyntacticSymbol::stringToSyntacticSymbolType(const QString &string)
{
    if (m_convertingStringToSyntacticSymbolTypeHash.isEmpty()) {
        m_convertingStringToSyntacticSymbolTypeHash.insert("nonterminalSymbol",SyntacticSymbol::nonterminalSymbol);
        m_convertingStringToSyntacticSymbolTypeHash.insert("startSymbol",SyntacticSymbol::startSymbol);
        m_convertingStringToSyntacticSymbolTypeHash.insert("terminalSymbol",SyntacticSymbol::terminalSymbol);
    }

    return m_convertingStringToSyntacticSymbolTypeHash.value(string);
}

bool SyntacticSymbol::isCorrect() const
{
    if(m_name.isNull())
        return false;
    return true;
}

bool operator==(const SyntacticSymbol &symbol1, const SyntacticSymbol &symbol2)
{
    bool areSymbolsTypeStart = (symbol1.type() == SyntacticSymbol::startSymbol)&&(symbol2.type() == SyntacticSymbol::startSymbol);
    bool areSymbolsSame = (symbol1.name() == symbol2.name())&&(symbol1.type() == symbol2.type());
    return (areSymbolsSame || areSymbolsTypeStart);
}

uint qHash(const SyntacticSymbol &symbol)
{
    return qHash(symbol.name());
}

bool operator==(const Token &token, const SyntacticSymbol &symbol)
{
    switch (token.tokenCategory()) {
    case Token::categoryNumberLiteral:
        return symbol.category() == Token::categoryNumberLiteral;
    case Token::categoryStringLiteral:
        return symbol.category() == Token::categoryStringLiteral;
    case Token::categoryIdentifier:
        return symbol.category() == Token::categoryIdentifier;
    case Token::categoryLineFeed:
        return symbol.category() == Token::categoryLineFeed;
    default:
        return(token.lexeme() == symbol.name())&&(symbol.type() == SyntacticSymbol::terminalSymbol); //compare by lexemes
    }
}

bool operator==(const SyntacticSymbol &symbol, const Token &token)
{
    return token == symbol;
}

QString MakeStringRepresentation(const QList<SyntacticSymbol> &syntacticSymbolList)
{
    if (syntacticSymbolList.isEmpty()) {
        return "Λ";
    }
    QString result;
    foreach (SyntacticSymbol symbol, syntacticSymbolList) {
        result += MakeStringRepresentation(symbol) + " ";
    }
    return result;
}


QString MakeStringRepresentation(const SyntacticSymbol &syntacticSymbol)
{
    switch (syntacticSymbol.type()) {
    case SyntacticSymbol::nonterminalSymbol:
        return QString("<" + syntacticSymbol.name() + ">");
    case SyntacticSymbol::terminalSymbol: {
       if ((syntacticSymbol.category() == Token::categoryKeyword)||(syntacticSymbol.category() == Token::categoryCharToken)) {
           return QString("\"" + syntacticSymbol.name() + "\"");
       } else {
           return syntacticSymbol.name();
       }
    }
    case SyntacticSymbol::startSymbol:
        return "S";
    default:
        return QString();
    }
}
