#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <QObject>

class SyntacticAnalyzer;
class SemanticAnalyzer;
class LexicalAnalyzer;
class ThreeAddressCodeGenerator;

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = 0);
    ~Translator();

    bool lexicalAnalyzerComplete() const;
    bool syntacticAnalyzerComplete() const;
    bool semanticAnalyzerComplete() const;
    bool threeAddressCodeGeneratorComplete() const;

    SyntacticAnalyzer *syntacticAnalyzer() const;
    SemanticAnalyzer *semanticAnalyzer() const;
    LexicalAnalyzer *lexicalAnalyzer() const;
    ThreeAddressCodeGenerator *threeAddressCodeGenerator() const;

signals:

public slots:
    void translate(const QString &code);

private:

    void prepare();

    SyntacticAnalyzer *m_syntacticAnalyzer;
    SemanticAnalyzer *m_semanticAnalyzer;
    LexicalAnalyzer *m_lexicalAnalyzer;
    ThreeAddressCodeGenerator *m_threeAddressCodeGenerator;

    bool m_lexicalAnalyzerComplete;
    bool m_syntacticAnalyzerComplete;
    bool m_semanticAnalyzerComplete;
    bool m_threeAddressCodeGeneratorComplete;

};

#endif // TRANSLATOR_H
