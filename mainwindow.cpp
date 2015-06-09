#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HTMLMarkupGenerator.h"
#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "ThreeAddressCodeGenerator.h"
#include "Identifier.h"
#include "FileReader.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QTime>

Q_GLOBAL_STATIC(LexicalAnalyzer,globalLexicalAnalyzer)

Q_GLOBAL_STATIC(SyntacticAnalyzer,globalSyntacticAnalyzer)

Q_GLOBAL_STATIC(SemanticAnalyzer,globalSemanticAnalyzer)

Q_GLOBAL_STATIC(ThreeAddressCodeGenerator,globalThreeAddressCodeGenerator)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_markupGenerator(new HTMLMarkupGenerator),
    m_rulesStringListModel(new QStringListModel),
    m_lexicalAnalyzerSettingsFileName("LexicalAnalyzersSettings.json"),
    m_syntacticAnalyzerSettingsFileName("SyntacticAnalyzersSetting.rules")
{
    ui->setupUi(this);

    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateStatusBarSignal(int,int)), SLOT(updateStatusBarSlot(int,int)));
    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateTextByTimerSignal()), SLOT(updateSourceCodeInputTextEditSlot()));

    ui->tokenSequenceTextEdit->setFont(QFont("Courier New", 12));
    ui->rulesListView->setFont(QFont("Courier New", 12));
    ui->threeAddressCodeTextEdit->setFont(QFont("Courier New", 12));

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete m_markupGenerator;
    delete m_rulesStringListModel;
    delete ui;
}

void MainWindow::on_actionRun_triggered()
{
    globalLexicalAnalyzer->analyze(ui->sourceCodeInputTextEdit->toPlainText());

    globalSyntacticAnalyzer->analyze(globalLexicalAnalyzer->getTokenListWithoutSpaces());

    if (globalLexicalAnalyzer->errorText().isEmpty() && globalSyntacticAnalyzer->errorText().isEmpty()) {
        QList<Token> tokenList = globalLexicalAnalyzer->tokenList();
        globalSemanticAnalyzer->analyze(tokenList);
        globalThreeAddressCodeGenerator->generate(tokenList);
    }

    // Add new message to log
    ui->compileOutputTextEdit->addHTMLString(this->getMarkupGenerator()->getMessageForLog(*globalLexicalAnalyzer, *globalSyntacticAnalyzer, *globalSemanticAnalyzer, *globalThreeAddressCodeGenerator));

    // Write information about tokens
    ui->tokenSequenceTextEdit->setText(MakeStringRepresentation(globalLexicalAnalyzer->tokenList()));
    m_rulesStringListModel->setStringList(globalSyntacticAnalyzer->usedRuleList());
    ui->rulesListView->setModel(m_rulesStringListModel);
    ui->programBlockTreeWidget->setData(globalSemanticAnalyzer->mainBlock());
    ui->threeAddressCodeTextEdit->setText(globalThreeAddressCodeGenerator->threeAddressCode());

}

void MainWindow::updateStatusBarSlot(int line, int pos)
{
    ui->statusBar->showMessage(QString("\t(%1 : %2)").arg(pos).arg(line));
}

void MainWindow::updateSourceCodeInputTextEditSlot()
{
    globalLexicalAnalyzer->analyze(ui->sourceCodeInputTextEdit->toPlainText().toUpper());

    ui->sourceCodeInputTextEdit->setHtml(this->getMarkupGenerator()->getSourceCodeHTMLMarkup(*globalLexicalAnalyzer));
}

HTMLMarkupGenerator* MainWindow::getMarkupGenerator() const
{
    return m_markupGenerator;
}

void MainWindow::loadSettings()
{
    if (!FileReader::isFileExist(m_lexicalAnalyzerSettingsFileName))
        m_lexicalAnalyzerSettingsFileName = QFileDialog::getOpenFileName(this, tr("Open Settings File"),"LexicalAnalyzersSettings.json", tr("JSON (*.json)"));
    FileReader::loadLexicalAnalyzerSettings(m_lexicalAnalyzerSettingsFileName,*globalLexicalAnalyzer);

    if (!FileReader::isFileExist(m_syntacticAnalyzerSettingsFileName))
        m_syntacticAnalyzerSettingsFileName = QFileDialog::getOpenFileName(this, tr("Open Rules File"),"SyntacticAnalyzersSetting.rules", tr("RULES (*.rules)"));
    FileReader::loadSyntacticAnalyzerRules(m_syntacticAnalyzerSettingsFileName,*globalSyntacticAnalyzer);

}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"sourceCode.txt",tr(" (*.txt)"));
    FileReader::writeTextToFile(fileName,ui->sourceCodeInputTextEdit->toPlainText());
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr(" (*.txt)"));
    if (!fileName.isEmpty()) {
        QString text = FileReader::getTextFromFile(fileName);
        ui->sourceCodeInputTextEdit->clear();
        ui->sourceCodeInputTextEdit->setText(text);
    }
}

void MainWindow::on_actionNew_triggered()
{
    ui->sourceCodeInputTextEdit->clear();
}
