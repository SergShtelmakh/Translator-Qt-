#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class LexicalAnalyzer;
class LexicalAnalysisHTMLMarkupGenerator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRun_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void updateStatusBarSlot(int line, int pos);
    void updateSourceCodeInputTextEditSlot();

private:
    LexicalAnalysisHTMLMarkupGenerator* getLexicalAnalysisMarkupGenerator() const;

    void setDefaultWindowProperty();

    Ui::MainWindow *ui;
    LexicalAnalysisHTMLMarkupGenerator* m_markupGenerator;
};

#endif // MAINWINDOW_H
