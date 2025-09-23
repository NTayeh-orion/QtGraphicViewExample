#ifndef VERILOGPARSER_H
#define VERILOGPARSER_H
#include <QList>
#include <QString>

struct Port
{
    QString dir; // "input" or "output"
    QString name;
};

struct ModuleInfo
{
    QString name;
    QList<Port> ports;
};

ModuleInfo parseVerilogModule(const QString &filePath);

#endif // VERILOGPARSER_H
