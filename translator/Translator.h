#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <QObject>

class SyntacticAnalyzer;
class SemanticAnalyzer;
class LexicalAnalyzer;
class ThreeAddressCodeGenerator;
class HTMLMarkupGenerator;
class Block;

class Translator : public QObject
{
    Q_OBJECT
public:

    static const QString defaultLexicalAnalyzerSettingsFileName;
    static const QString defaultSyntacticAnalyzerSettingsFileName;

    explicit Translator(QObject *parent = 0);
    ~Translator();

    QString getMarkedUpSourceCode(QString inputCode);

    SyntacticAnalyzer *syntacticAnalyzer() const;
    LexicalAnalyzer *lexicalAnalyzer() const;

    bool lexicalAnalyzerComplete() const;
    bool syntacticAnalyzerComplete() const;
    bool semanticAnalyzerComplete() const;
    bool threeAddressCodeGeneratorComplete() const;

    SemanticAnalyzer *semanticAnalyzer() const;
    ThreeAddressCodeGenerator *threeAddressCodeGenerator() const;

signals:

    void addToLog(const QString &log);
    void setTokenList(const QString &tokenList);
    void setRuleList(const QStringList &ruleList);
    void setBlockTree(Block *block);
    void setThreeAddressCode(const QString &code);

public slots:

    void translate(const QString &code);

private:

    void prepare();

    SyntacticAnalyzer *m_syntacticAnalyzer;
    SemanticAnalyzer *m_semanticAnalyzer;
    LexicalAnalyzer *m_lexicalAnalyzer;
    ThreeAddressCodeGenerator *m_threeAddressCodeGenerator;
    HTMLMarkupGenerator *m_HTMLMarkupGenerator;

    bool m_lexicalAnalyzerComplete;
    bool m_syntacticAnalyzerComplete;
    bool m_semanticAnalyzerComplete;
    bool m_threeAddressCodeGeneratorComplete;

};

#endif // TRANSLATOR_H
