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
};

#endif // CODEHIGHLIGHTER_H
