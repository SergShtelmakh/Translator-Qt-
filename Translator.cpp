#include "Translator.h"
#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "ThreeAddressCodeGenerator.h"

Translator::Translator(QObject *parent) :
    QObject(parent),
    m_syntacticAnalyzer(new SyntacticAnalyzer),
    m_semanticAnalyzer(new SemanticAnalyzer),
    m_lexicalAnalyzer(new LexicalAnalyzer),
    m_threeAddressCodeGenerator(new ThreeAddressCodeGenerator)
{}

Translator::~Translator()
{
    delete m_syntacticAnalyzer;
    delete m_semanticAnalyzer;
    delete m_lexicalAnalyzer;
    delete m_threeAddressCodeGenerator;
}

void Translator::translate(const QString &code)
{
    this->prepare();

    m_lexicalAnalyzer->analyze(code);
    m_lexicalAnalyzerComplete = m_lexicalAnalyzer->errorText().isEmpty();

    m_syntacticAnalyzer->analyze(m_lexicalAnalyzer->getTokenListWithoutSpaces());
    m_syntacticAnalyzerComplete = m_syntacticAnalyzer->errorText().isEmpty();

    if (m_lexicalAnalyzerComplete && m_syntacticAnalyzerComplete) {
        QList<Token> tokenList = m_lexicalAnalyzer->tokenList();

        m_semanticAnalyzer->analyze(tokenList);
        m_semanticAnalyzerComplete = m_semanticAnalyzer->errorText().isEmpty();

        m_threeAddressCodeGenerator->generate(tokenList);
        m_threeAddressCodeGeneratorComplete = m_threeAddressCodeGenerator->errorText().isEmpty();
    }
}

void Translator::prepare()
{
    m_lexicalAnalyzerComplete = false;
    m_syntacticAnalyzerComplete = false;
    m_semanticAnalyzerComplete = false;
    m_threeAddressCodeGeneratorComplete = false;
}

ThreeAddressCodeGenerator *Translator::threeAddressCodeGenerator() const
{
    return m_threeAddressCodeGenerator;
}

LexicalAnalyzer *Translator::lexicalAnalyzer() const
{
    return m_lexicalAnalyzer;
}

SemanticAnalyzer *Translator::semanticAnalyzer() const
{
    return m_semanticAnalyzer;
}

SyntacticAnalyzer *Translator::syntacticAnalyzer() const
{
    return m_syntacticAnalyzer;
}

bool Translator::threeAddressCodeGeneratorComplete() const
{
    return m_threeAddressCodeGeneratorComplete;
}

bool Translator::semanticAnalyzerComplete() const
{
    return m_semanticAnalyzerComplete;
}

bool Translator::syntacticAnalyzerComplete() const
{
    return m_syntacticAnalyzerComplete;
}

bool Translator::lexicalAnalyzerComplete() const
{
    return m_lexicalAnalyzerComplete;
}

