#include "verilogparser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

ModuleInfo parseVerilogModule(const QString &filePath) {
    ModuleInfo info;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return info;

    QTextStream in(&file);
    QRegularExpression moduleRegex(R"(module\s+(\w+)\s*\(([^)]*)\))");
    // QRegularExpression portRegex(R"((input|output)\s*(?:\[[^\]]+\]\s*)?(\w+))");
    QRegularExpression portRegex(R"((input|output)\s*(?:\[[^\]]+\]\s*)?(\w+))");


    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = moduleRegex.match(line);
        if (match.hasMatch()) {
            info.name = match.captured(1);
            QString portList = match.captured(2);

            QRegularExpressionMatchIterator it = portRegex.globalMatch(portList);
            while (it.hasNext()) {
                QRegularExpressionMatch m = it.next();
                Port p;
                p.dir = m.captured(1);
                p.name = m.captured(2);
                info.ports.append(p);
            }
            break;
        }
    }
    return info;
}
