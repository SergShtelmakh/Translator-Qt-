#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BasicLexicalAnalyzer.h"
#include "LexicalAnalysisHTMLMarkupGenerator.h"
#include "SyntacticAnalyzer.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QTime>

Q_GLOBAL_STATIC(BasicLexicalAnalyzer,globalBasicLexicalAnalyzer)

Q_GLOBAL_STATIC(SyntacticAnalyzer,globalSyntacticAnalyzer)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_markupGenerator = new LexicalAnalysisHTMLMarkupGenerator();

    ui->setupUi(this);

    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateStatusBarSignal(int,int)), SLOT(updateStatusBarSlot(int,int)));
    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateTextByTimerSignal()), SLOT(updateSourceCodeInputTextEditSlot()));

    ui->tokenSequenceTextEdit->setFont(QFont("Courier New", 12));
}

MainWindow::~MainWindow()
{
    delete m_markupGenerator;
    delete ui;
}

void MainWindow::on_actionRun_triggered()
{
    globalBasicLexicalAnalyzer->analyze(ui->sourceCodeInputTextEdit->toPlainText());

    // Add new message to log
    ui->compileOutputTextEdit->addHTMLString(this->getLexicalAnalysisMarkupGenerator()->getMessageForLog(*globalBasicLexicalAnalyzer));

    // Write information about tokens
    ui->tokenSequenceTextEdit->setText(TokenListToString(globalBasicLexicalAnalyzer->tokenList()));

    // Write information about identifiers
    QList <Identifier> identifierList = globalBasicLexicalAnalyzer->identifierList();
    ui->identifierTableWidget->setRowCount(identifierList.count());
    for (int i = 0; i < identifierList.count(); i++) {
        ui->identifierTableWidget->setItem(i,0,new QTableWidgetItem(identifierList.at(i).name()));
        ui->identifierTableWidget->setItem(i,1,new QTableWidgetItem(QString("%1").arg(identifierList.at(i).positionsList().count())));
        ui->identifierTableWidget->setItem(i,2,new QTableWidgetItem(IdentifierPositionsToString(identifierList.at(i))));
    }

    globalSyntacticAnalyzer->analyze(globalBasicLexicalAnalyzer->getTokenListWithoutSpaces());
}

void MainWindow::updateStatusBarSlot(int line, int pos)
{
    ui->statusBar->showMessage(QString("\t(%1 : %2)").arg(line).arg(pos));
}

void MainWindow::updateSourceCodeInputTextEditSlot()
{
    globalBasicLexicalAnalyzer->analyze(ui->sourceCodeInputTextEdit->toPlainText().toUpper());

    ui->sourceCodeInputTextEdit->setHtml(this->getLexicalAnalysisMarkupGenerator()->getSourceCodeHTMLMarkup(*globalBasicLexicalAnalyzer));
}

LexicalAnalysisHTMLMarkupGenerator* MainWindow::getLexicalAnalysisMarkupGenerator() const
{
    return m_markupGenerator;
}

void MainWindow::on_actionSave_triggered()
{
    QFile file (QFileDialog::getSaveFileName(this, tr("Save File"),"untitled.bas",tr("Basic (*.bas)")));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

    QTextStream out(&file);
    out << ui->sourceCodeInputTextEdit->toPlainText();

}

void MainWindow::on_actionOpen_triggered()
{
    QFile file (QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Basic (*.bas)")));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream inputTextStream(&file);
    QString text;
         QString line = inputTextStream.readLine();
         while (!line.isNull()) {
             text += line +"\n";
             line = inputTextStream.readLine();
         }

    ui->sourceCodeInputTextEdit->clear();
    ui->sourceCodeInputTextEdit->setText(text);
}

void MainWindow::on_actionNew_triggered()
{
    ui->sourceCodeInputTextEdit->clear();
}
