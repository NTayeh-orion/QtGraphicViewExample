#include "crashhandler.h"
#include<QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <csignal>
#include <cstdlib>
#include <cstring> // for strsignal()
// --- Crash handler for fatal signals ---
void crashHandler(int signal)
{
    QString msg;
    switch (signal) {
    case SIGSEGV:
        msg = "Segmentation fault (invalid memory access).";
        break;
    case SIGABRT:
        msg = "Abnormal termination (abort signal).";
        break;

    case SIGFPE:
        msg = "Floating point exception";
        break;
    case SIGILL:
        msg = "Illegal instruction";
        break;
    case SIGINT:
        msg = "Interrupt from keyboard";
        break;
    default:
        msg = QString("Caught fatal signal: %1").arg(signal);
    }

    // Use a minimal QMessageBox if QApplication is still alive
    if (QApplication::instance()) {
        qCritical() << "Fatal error:" << msg;
        QMessageBox::critical(nullptr, "Application Crash", msg);
    }

    qCritical() << "Fatal error:" << msg;
    std::_Exit(EXIT_FAILURE); // safe immediate exit
}

// --- Call this early in main() ---
void installCrashHandler()
{
    std::signal(SIGSEGV, crashHandler);
    std::signal(SIGABRT, crashHandler);
    std::signal(SIGFPE,  crashHandler);
    std::signal(SIGILL,  crashHandler);
    std::signal(SIGINT,  crashHandler);
}
