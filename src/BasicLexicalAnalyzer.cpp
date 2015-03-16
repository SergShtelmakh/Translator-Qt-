#include "BasicLexicalAnalyzer.h"

BasicLexicalAnalyzer::BasicLexicalAnalyzer()
{
    addKeyword("BOOLEAN",LexicalAnalyzer::keyWordTypeName);
    addKeyword("DOUBLE",LexicalAnalyzer::keyWordTypeName);
    addKeyword("INTEGER",LexicalAnalyzer::keyWordTypeName);
    addKeyword("STRING",LexicalAnalyzer::keyWordTypeName);

    addKeyword("FALSE",LexicalAnalyzer::keyWordBooleanLiteral);
    addKeyword("TRUE",LexicalAnalyzer::keyWordBooleanLiteral);

    addKeyword("IF",LexicalAnalyzer::keyWordIfStatement);
    addKeyword("THEN",LexicalAnalyzer::keyWordIfStatement);

    addKeyword("AS",LexicalAnalyzer::keyWordAssignment);
    addKeyword("DATA",LexicalAnalyzer::keyWordAssignment);
    addKeyword("DIM",LexicalAnalyzer::keyWordAssignment);
    addKeyword("READ",LexicalAnalyzer::keyWordAssignment);

    addKeyword("FOR",LexicalAnalyzer::keyWordForLoop);
    addKeyword("NEXT",LexicalAnalyzer::keyWordForLoop);
    addKeyword("STEP",LexicalAnalyzer::keyWordForLoop);
    addKeyword("TO",LexicalAnalyzer::keyWordForLoop);

    addKeyword("END",LexicalAnalyzer::keyWordOther);

    addCharacterToken("+",Token::categoryOperation);
    addCharacterToken("-",Token::categoryOperation);
    addCharacterToken("/",Token::categoryOperation);
    addCharacterToken("*",Token::categoryOperation);

    addCharacterToken("=",Token::categoryComparator);
    addCharacterToken("<",Token::categoryComparator);
    addCharacterToken(">",Token::categoryComparator);

    addCharacterToken(",",Token::categoryComma);

    addCharacterToken("(",Token::categoryBracket);
    addCharacterToken(")",Token::categoryBracket);

    addCharacterToken("<=",Token::categoryComparator);
    addCharacterToken(">=",Token::categoryComparator);

}

BasicLexicalAnalyzer::~BasicLexicalAnalyzer()
{

}

