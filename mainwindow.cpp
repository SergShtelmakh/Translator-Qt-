#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileReader.h"
#include "Translator.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_rulesStringListModel(new QStringListModel),
    m_lexicalAnalyzerSettingsFileName(Translator::defaultLexicalAnalyzerSettingsFileName),
    m_syntacticAnalyzerSettingsFileName(Translator::defaultSyntacticAnalyzerSettingsFileName),
    m_translator(new Translator)
{
    ui->setupUi(this);

    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateStatusBarSignal(int, int)), SLOT(updateStatusBarSlot(int, int)));
    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateTextByTimerSignal()), SLOT(updateSourceCodeInputTextEditSlot()));
    connect(m_translator, SIGNAL(addToLog(QString)), this, SLOT(addToLog(QString)));
    connect(m_translator, SIGNAL(setTokenList(QString)), ui->tokenSequenceTextEdit, SLOT(setText(QString)));
    connect(m_translator, SIGNAL(setRuleList(QStringList)), this, SLOT(setRuleList(QStringList)));
    connect(m_translator, SIGNAL(setBlockTree(Block*)), this, SLOT(setBlockTree(Block*)));
    connect(m_translator, SIGNAL(setThreeAddressCode(QString)), ui->threeAddressCodeTextEdit, SLOT(setText(QString)));

    ui->tokenSequenceTextEdit->setFont(QFont("Courier New", 12));
    ui->rulesListView->setFont(QFont("Courier New", 12));
    ui->threeAddressCodeTextEdit->setFont(QFont("Courier New", 12));
    ui->rulesListView->setModel(m_rulesStringListModel);

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete m_rulesStringListModel;
    delete m_translator;
    delete ui;
}

void MainWindow::on_actionRun_triggered()
{
    m_translator->translate(ui->sourceCodeInputTextEdit->toPlainText());
}

void MainWindow::updateStatusBarSlot(int line, int pos)
{
    ui->statusBar->showMessage(QString("\t(%1 : %2)").arg(pos).arg(line));
}

void MainWindow::updateSourceCodeInputTextEditSlot()
{
    QString sourceCode = ui->sourceCodeInputTextEdit->toPlainText().toUpper();
    QString markedUpSourceCode = m_translator->getMarkedUpSourceCode(sourceCode);
    ui->sourceCodeInputTextEdit->setHtml(markedUpSourceCode);
}

void MainWindow::addToLog(const QString &log)
{
    ui->compileOutputTextEdit->addHTMLString(log);
}

void MainWindow::setRuleList(const QStringList &ruleList)
{
    m_rulesStringListModel->setStringList(ruleList);
}

void MainWindow::setBlockTree(Block *block)
{
    ui->programBlockTreeWidget->setData(block);
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
