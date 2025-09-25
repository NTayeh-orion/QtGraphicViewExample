#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class CodeHighlighter : public QSyntaxHighlighter
{
public:
    CodeHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    const QStringList svKeywords
        = {"always",     "and",         "assign",      "automatic", "begin",        "break",
           "buf",        "case",        "casex",       "casez",     "cell",         "cmos",
           "config",     "const",       "continue",    "deassign",  "default",      "defparam",
           "design",     "disable",     "edge",        "else",      "end",          "endcase",
           "endconfig",  "endfunction", "endgenerate", "endmodule", "endprimitive", "endspecify",
           "endtable",   "endtask",     "enum",        "event",     "for",          "force",
           "forever",    "fork",        "function",    "generate",  "genvar",       "if",
           "ifdef",      "ifndef",      "include",     "initial",   "inout",        "input",
           "instance",   "integer",     "join",        "large",     "liblist",      "library",
           "localparam", "macromodule", "module",      "nand",      "negedge",      "nmos",
           "nor",        "not",         "or",          "output",    "parameter",    "pmos",
           "posedge",    "primitive",   "pull0",       "pull1",     "pulldown",     "pullup",
           "rcmos",      "real",        "realtime",    "reg",       "release",      "repeat",
           "rnmos",      "rpmos",       "rtran",       "rtranif0",  "rtranif1",     "scalared",
           "signed",     "small",       "specify",     "specparam", "strength",     "strong0",
           "strong1",    "supply0",     "supply1",     "table",     "task",         "time",
           "tran",       "tranif0",     "tranif1",     "unsigned",  "vectored",     "wait",
           "wand",       "weak0",       "weak1",       "while",     "wire",         "wor",
           "xnor",       "xor"};
    QString keywordPattern;
};

#endif // CODEHIGHLIGHTER_H
