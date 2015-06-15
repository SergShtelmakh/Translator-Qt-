#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HTMLMarkupGenerator.h"
#include "Identifier.h"
#include "FileReader.h"
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"
#include "ThreeAddressCodeGenerator.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QTime>
#include "Translator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_markupGenerator(new HTMLMarkupGenerator),
    m_rulesStringListModel(new QStringListModel),
    m_lexicalAnalyzerSettingsFileName("LexicalAnalyzersSettings.json"),
    m_syntacticAnalyzerSettingsFileName("SyntacticAnalyzersSetting.rules"),
    m_translator(new Translator)
{
    ui->setupUi(this);

    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateStatusBarSignal(int, int)), SLOT(updateStatusBarSlot(int, int)));
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
    delete m_translator;
    delete ui;
}

void MainWindow::on_actionRun_triggered()
{
    m_translator->translate(ui->sourceCodeInputTextEdit->toPlainText());

    // Add new message to log
    ui->compileOutputTextEdit->addHTMLString(this->getMarkupGenerator()->getMessageForLog(m_translator));

    // Write information about tokens
    ui->tokenSequenceTextEdit->setText(MakeStringRepresentation(m_translator->lexicalAnalyzer()->tokenList()));
    m_rulesStringListModel->setStringList(m_translator->syntacticAnalyzer()->usedRuleList());
    ui->rulesListView->setModel(m_rulesStringListModel);
    ui->programBlockTreeWidget->setData(m_translator->semanticAnalyzer()->mainBlock());
    ui->threeAddressCodeTextEdit->setText(m_translator->threeAddressCodeGenerator()->threeAddressCode());

}

void MainWindow::updateStatusBarSlot(int line, int pos)
{
    ui->statusBar->showMessage(QString("\t(%1 : %2)").arg(pos).arg(line));
}

void MainWindow::updateSourceCodeInputTextEditSlot()
{
    m_translator->lexicalAnalyzer()->analyze(ui->sourceCodeInputTextEdit->toPlainText().toUpper());

    ui->sourceCodeInputTextEdit->setHtml(this->getMarkupGenerator()->getSourceCodeHTMLMarkup(m_translator->lexicalAnalyzer()));
}

HTMLMarkupGenerator* MainWindow::getMarkupGenerator() const
{
    return m_markupGenerator;
}

void MainWindow::loadSettings()
{
    if (!FileReader::isFileExist(m_lexicalAnalyzerSettingsFileName))
        m_lexicalAnalyzerSettingsFileName = QFileDialog::getOpenFileName(this, tr("Open Settings File"), "LexicalAnalyzersSettings.json", tr("JSON (*.json)"));
    FileReader::loadLexicalAnalyzerSettings(m_lexicalAnalyzerSettingsFileName, m_translator);

    if (!FileReader::isFileExist(m_syntacticAnalyzerSettingsFileName))
        m_syntacticAnalyzerSettingsFileName = QFileDialog::getOpenFileName(this, tr("Open Rules File"), "SyntacticAnalyzersSetting.rules", tr("RULES (*.rules)"));
    FileReader::loadSyntacticAnalyzerRules(m_syntacticAnalyzerSettingsFileName, m_translator);

}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "sourceCode.txt", tr(" (*.txt)"));
    FileReader::writeTextToFile(fileName, ui->sourceCodeInputTextEdit->toPlainText());
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr(" (*.txt)"));
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
