#include "verilogparser.h"
#include <QFile>
#include <QJsonArray>
#include <QRegularExpression>
#include <QTextStream>
ModuleInfo parseVerilogModule(const QString &filePath)
{
    qDebug() << " ----------------------------- Inside parseVerilogModule "
                "------------------------------ ";
    ModuleInfo info;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return info;

    // QTextStream in(&file);
    // // qDebug() << in.readAll() ;
    // QRegularExpression moduleRegex(R"(module\s+(\w+)\s*\(([^)]*)\))" , QRegularExpression::CaseInsensitiveOption);
    // // QRegularExpression portRegex(R"((input|output)\s*(?:\[[^\]]+\]\s*)?(\w+))");
    // QRegularExpression portRegex(R"((input|output)\s*(?:\[[^\]]+\]\s*)?(\w+))");

    // while (!in.atEnd())
    // {
    //     QString line = in.readLine();
    //     qDebug() << line;

    //     QRegularExpressionMatch match = moduleRegex.match(line);
    //     qDebug() << match.captured(0);
    //     qDebug() << match.hasMatch();
    //     if (match.hasMatch())
    //     {
    //         qDebug() << "match and value = " << match.captured(0);
    //         info.name = match.captured(1);
    //         QString portList = match.captured(2);

    //         QRegularExpressionMatchIterator it = portRegex.globalMatch(portList);
    //         while (it.hasNext())
    //         {
    //             QRegularExpressionMatch m = it.next();
    //             Port p;
    //             p.dir = m.captured(1);
    //             p.name = m.captured(2);
    //             info.ports.append(p);
    //         }
    //         break;
    //     }
    // }
    // return info;
    QString text = QTextStream(&file).readAll();
    file.close();

    // 1. Extract module name
    QRegularExpression moduleRe(R"(module\s+(\w+))");
    QRegularExpressionMatch m = moduleRe.match(text);
    info.name = m.hasMatch() ? m.captured(1) : "UnknownDUT";

    // 2. Extract port block
    QRegularExpression portBlockRe(R"(\((.*?)\))", QRegularExpression::DotMatchesEverythingOption);
    m = portBlockRe.match(text);
    QString portBlock = m.hasMatch() ? m.captured(1) : "";
    qDebug() << "the result of regex  = " + portBlock + " \n \n \n \n \n";
    // 3. Remove comments and newlines
    portBlock.replace(QRegularExpression("//.*"), ""); // remove single-line comments
    portBlock.replace(QRegularExpression("/\\*.*?\\*/",
                                         QRegularExpression::DotMatchesEverythingOption),
                      "");              // remove multi-line comments
    portBlock.replace("\n", " ");       // replace line breaks with space
    portBlock.replace("\r", "");        // remove carriage returns
    portBlock = portBlock.simplified(); // remove extra whitespace

    // 4. Split by commas to handle multiple ports
    QStringList portDecls = portBlock.split(',', Qt::SkipEmptyParts);

    // 5. Parse each port declaration
    // QJsonArray portsArray;

    // Regex to capture port declarations with optional type and width
    QRegularExpression portRe(
        R"((input|output|inout)\s+(?:logic|wire|reg)?\s*(?:\[[^\]]+\])?\s*(\w+)(?:\s*,\s*(\w+))*)",
        QRegularExpression::CaseInsensitiveOption);

    QString currentDirection;
    QString currentType = "logic";
    QString currentWidth = "1";
    qDebug() << "ports ======== "<< portDecls;
    qDebug() << "\n \n \n " ;
    for (QString decl : portDecls) {
        decl = decl.trimmed();
        qDebug() << "Processing: " << decl;

        // Check if this line contains a direction keyword
        if (decl.contains(QRegularExpression("^(input|output|inout)",
                                             QRegularExpression::CaseInsensitiveOption))) {
            // This is a full declaration with direction, type, width, and signal names
            QRegularExpression fullDeclRe(
                R"((input|output|inout)\s+(logic|wire|reg)?\s*(\[[^\]]+\])?\s*([\w\s,]+))",
                QRegularExpression::CaseInsensitiveOption);

            QRegularExpressionMatch match = fullDeclRe.match(decl);
            if (match.hasMatch()) {
                currentDirection = match.captured(1).toLower();
                QString signalNames = match.captured(4).trimmed();

                // Split multiple signal names
                QStringList names = signalNames.split(',', Qt::SkipEmptyParts);
                for (QString name : names) {
                    name = name.trimmed();
                    if (!name.isEmpty()) {
                        Port p;
                        p.dir = currentDirection;
                        p.name = name;
                        info.ports.append(p);
                        qDebug() << "Added port: " << name << "with direction:" << currentDirection;
                    }
                }
            }
        } else {
            // This is just a signal name continuing from previous declaration
            QStringList names = decl.split(',', Qt::SkipEmptyParts);
            for (QString name : names) {
                name = name.trimmed();
                if (!name.isEmpty()) {
                    Port p;
                    p.dir = currentDirection;
                    p.name = name;
                    info.ports.append(p);
                    qDebug() << "Added continued port: " << name
                             << "with direction:" << currentDirection;
                }
            }
        }
    }

    qDebug() <<" the size of ports = " ;
    qDebug() << info.ports.size();
    foreach (Port p, info.ports) {
        qDebug() << " name = " + p.name + " |||||||||||  dir = " + p.dir ;

    }
    return info;
}
