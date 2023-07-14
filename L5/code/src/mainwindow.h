//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QDockWidget>
#include "display.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(unsigned int nbSkiers, QWidget *parent = 0);
    ~MainWindow();

    std::vector<QDockWidget* > m_docks;
    std::vector<QTextEdit* > m_consoles;
    QDockWidget* m_ccarDocks;
    QTextEdit* m_ccarConsole;
    SlopeDisplay *m_display;

    void setConsoleTitle(unsigned int consoleId, const QString &title);

protected:
    unsigned int m_nbConsoles;

public slots:
    void consoleAppendTextCcar(const QString &text);
    void consoleAppendText(unsigned int consoleId, const QString &text);
    void cCarTravelUp(unsigned int ms);
    void cCarTravelDown(unsigned int ms);
    void skierTravelIn(unsigned id, unsigned int ms);
    void skierTravelOut(unsigned id, unsigned int ms);
    void skierSlide(unsigned id, unsigned int ms);
    void skierWait(unsigned id, unsigned int ms);
};

#endif // MAINWINDOW_H
