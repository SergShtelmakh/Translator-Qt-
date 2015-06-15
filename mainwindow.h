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

class Translator;
class Block;

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

    void addToLog(const QString& log);
    void setRuleList(const QStringList& ruleList);
    void setBlockTree(Block *block);

private:

    /*!
     * This method returns pointer on markup generator.
     *
     * @return Markup generator.
     */

    void loadSettings();

    Ui::MainWindow *ui;                             //!< Main window.
    QStringListModel* m_rulesStringListModel;
    QString m_lexicalAnalyzerSettingsFileName;
    QString m_syntacticAnalyzerSettingsFileName;
    Translator *m_translator;

};

#endif // MAINWINDOW_H
