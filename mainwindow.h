/*!
 * \file mainWindow.h
 * \date 2015/04/24
 *
 * \author SergShtelmakh
 * Contact: sh.serg1993@gmail.com
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

class LexicalAnalyzer;
class HTMLMarkupGenerator;

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
    HTMLMarkupGenerator* getMarkupGenerator() const;
    void loadSettings();

    Ui::MainWindow *ui;                             //!< Main window.
    HTMLMarkupGenerator* m_markupGenerator;         //!< Markup generator.
    QStringListModel* m_rulesStringListModel;
    QString m_lexicalAnalyzerSettingsFileName;
    QString m_syntacticAnalyzerSettingsFileName;

};

#endif // MAINWINDOW_H
