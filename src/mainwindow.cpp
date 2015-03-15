#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LexicalAnalyzer.h"
#include "LexicalAnalysisHTMLMarkupGenerator.h"
#include "GlobalObjects.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QTime>

LexicalAnalyzer GlobalObjects::lexicalAnalyzer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_markupGenerator = new LexicalAnalysisHTMLMarkupGenerator();

    ui->setupUi(this);

    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateStatusBarSignal(int,int)), SLOT(updateStatusBarSlot(int,int)));
    connect(ui->sourceCodeInputTextEdit, SIGNAL(updateTextByTimerSignal()), SLOT(updateSourceCodeInputTextEditSlot()));

    setDefaultWindowProperty();
}

MainWindow::~MainWindow()
{
    delete m_markupGenerator;
    delete ui;
}

void MainWindow::on_actionRun_triggered()
{
    GlobalObjects::lexicalAnalyzer.analyze(ui->sourceCodeInputTextEdit->toPlainText());

    // Add new message to log
    ui->compileOutputTextEdit->addHTMLString(this->getLexicalAnalysisMarkupGenerator()->getMessageForLog(GlobalObjects::lexicalAnalyzer));

    // Write information about tokens

    ui->tokenSequenceTextEdit->setText(TokenListListToString(GlobalObjects::lexicalAnalyzer.getTokenListList()));

    QList <Identifier> identifierList = GlobalObjects::lexicalAnalyzer.getIdentifierList();
    ui->identifierTableWidget->setRowCount(identifierList.count());
    for(int currentIdentifierIndex = 0; currentIdentifierIndex < identifierList.count(); currentIdentifierIndex++){
        ui->identifierTableWidget->setItem(currentIdentifierIndex,0,new QTableWidgetItem(identifierList.at(currentIdentifierIndex).getName()));
        ui->identifierTableWidget->setItem(currentIdentifierIndex,1,new QTableWidgetItem(IdentifierPositionsCountToString(identifierList.at(currentIdentifierIndex))));
        ui->identifierTableWidget->setItem(currentIdentifierIndex,2,new QTableWidgetItem(IdentifierPositionsToString(identifierList.at(currentIdentifierIndex))));
    }
}

void MainWindow::updateStatusBarSlot(int line, int pos)
{
    ui->statusBar->showMessage(QString("\t(%1 : %2)").arg(line).arg(pos));
}

void MainWindow::updateSourceCodeInputTextEditSlot()
{
    GlobalObjects::lexicalAnalyzer.analyze(ui->sourceCodeInputTextEdit->toPlainText().toUpper());

    ui->sourceCodeInputTextEdit->setHtml(this->getLexicalAnalysisMarkupGenerator()->getSourceCodeHTMLMarkup(GlobalObjects::lexicalAnalyzer));
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

    QTextStream in(&file);
    QString text;
         QString line = in.readLine();
         while (!line.isNull()) {
             text += line +"\n";
             line = in.readLine();
         }

    ui->sourceCodeInputTextEdit->clear();
    ui->sourceCodeInputTextEdit->setText(text);
}

void MainWindow::on_actionNew_triggered()
{
    ui->sourceCodeInputTextEdit->clear();
}

void MainWindow::setDefaultWindowProperty()
{
    ui->tokenSequenceTextEdit->setFont(QFont("Courier New", 12));
}
