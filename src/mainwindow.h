#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class LexicalAnalyzer;
class LexicalAnalysisHTMLMarkupGenerator;

namespace Ui {
class MainWindow;
}

/*!
 * @brief This class is main window.
 */
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

    /*!
     * This slot used to update information about cursor in status bar.
     *
     * @param[in] line Current line number.
     * @param[in] pos Current character in line number.
     */
    void updateStatusBarSlot(int line, int pos);

    /*!
     * This slot used to update source code text.
     */
    void updateSourceCodeInputTextEditSlot();

private:

    /*!
     * This method returns pointer on markup generator.
     *
     * @return Markup generator.
     */
    LexicalAnalysisHTMLMarkupGenerator* getLexicalAnalysisMarkupGenerator() const;
    void loadSettings();

    Ui::MainWindow *ui;                                     //!< Main window.
    LexicalAnalysisHTMLMarkupGenerator* m_markupGenerator;  //!< Markup generator.

};

#endif // MAINWINDOW_H
