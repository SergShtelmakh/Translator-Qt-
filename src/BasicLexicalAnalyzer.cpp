#include "BasicLexicalAnalyzer.h"

BasicLexicalAnalyzer::BasicLexicalAnalyzer()
{
    addKeyword("BOOLEAN");
    addKeyword("DOUBLE");
    addKeyword("INTEGER");
    addKeyword("STRING");

    addKeyword("FALSE");
    addKeyword("TRUE");

    addKeyword("IF");
    addKeyword("THEN");

    addKeyword("AS");
    addKeyword("DATA");
    addKeyword("DIM");
    addKeyword("READ");

    addKeyword("FOR");
    addKeyword("NEXT");
    addKeyword("STEP");
    addKeyword("TO");

    addKeyword("END");

    addCharacterToken("+");
    addCharacterToken("-");
    addCharacterToken("/");
    addCharacterToken("*");

    addCharacterToken("=");
    addCharacterToken("<");
    addCharacterToken(">");

    addCharacterToken(",");

    addCharacterToken("(");
    addCharacterToken(")");

    addCharacterToken("<=");
    addCharacterToken(">=");

}

BasicLexicalAnalyzer::~BasicLexicalAnalyzer()
{

}

