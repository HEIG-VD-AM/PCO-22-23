//     ____  __________ ___   ____ ___  ___
//    / __ \/ ____/ __ \__ \ / __ \__ \|__ \
//   / /_/ / /   / / / /_/ // / / /_/ /__/ /
//  / ____/ /___/ /_/ / __// /_/ / __// __/
// /_/    \____/\____/____/\____/____/____/
//
// Rien à modifier ici

#include "mainwindow.h"

#define CONSOLE_MINIMUM_WIDTH 200

MainWindow::MainWindow(unsigned int nbSkiers, QWidget *parent)
: QMainWindow(parent){
    m_nbConsoles = nbSkiers;
    m_consoles = std::vector<QTextEdit*>(nbSkiers);

    m_ccarConsole = new QTextEdit(this);
    m_ccarConsole->setMinimumWidth(CONSOLE_MINIMUM_WIDTH * 2);


    for(unsigned int i = 0; i < nbSkiers; i++) {
        m_consoles[i] = new QTextEdit(this);
        m_consoles[i]->setMinimumWidth(CONSOLE_MINIMUM_WIDTH);
    }

    m_ccarDocks = new QDockWidget(this);
    m_ccarDocks->setWidget(m_ccarConsole);

    m_docks = std::vector<QDockWidget*>(m_nbConsoles);
    for(unsigned int i = 0; i < m_nbConsoles; i++) {
        m_docks[i] = new QDockWidget(this);
        m_docks[i]->setWidget(m_consoles[i]);
    }

    this->addDockWidget(Qt::BottomDockWidgetArea, m_ccarDocks);

    for(unsigned int i = 0; i < nbSkiers/2; i++) {
        this->addDockWidget(Qt::LeftDockWidgetArea, m_docks[i]);
    }

    for(unsigned int i = nbSkiers/2; i < nbSkiers; i++) {
        this->addDockWidget(Qt::RightDockWidgetArea, m_docks[i]);
    }

    m_ccarDocks->setWindowTitle("Cable car status");
    for(unsigned int i = 0; i < nbSkiers; i++) {
        setConsoleTitle(i, QString("Skier number : %1").arg(i));
    }

    m_display = new SlopeDisplay(nbSkiers, this);
    setCentralWidget(m_display);
}


void MainWindow::setConsoleTitle(unsigned int consoleId,const QString &title)
{
    if (consoleId >= m_nbConsoles)
        return;
    m_docks[consoleId]->setWindowTitle(title);
}

void MainWindow::consoleAppendText(unsigned int consoleId, const QString &text)
{
    if (consoleId >= m_nbConsoles)
        return;
    m_consoles[consoleId]->append(text);
}

void MainWindow::consoleAppendTextCcar(const QString &text)
{
    m_ccarConsole->append(text);
}


void MainWindow::cCarTravelUp(unsigned int ms)
{
    m_display->cCarTravelUp(ms);
}

void MainWindow::cCarTravelDown(unsigned int ms)
{
    m_display->cCarTravelDown(ms);
}

void MainWindow::skierTravelIn(unsigned id, unsigned int ms)
{
    m_display->skierTravelIn(id, ms);
}

void MainWindow::skierTravelOut(unsigned id, unsigned int ms)
{
    m_display->skierTravelOut(id, ms);
}

void MainWindow::skierSlide(unsigned id, unsigned int ms)
{
    m_display->skierSlide(id, ms);
}

void MainWindow::skierWait(unsigned id, unsigned int ms)
{
    m_display->skierWait(id, ms);
}

MainWindow::~MainWindow() = default;

