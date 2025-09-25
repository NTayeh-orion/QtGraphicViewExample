#include "codehighlighter.h"

CodeHighlighter::CodeHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::cyan);
    keywordFormat.setFontWeight(QFont::Bold);
    highlightingRules.append(
        {QRegularExpression("\\b(class|int|float|if|else|while|for|return)\\b"), keywordFormat});

    keywordPattern = "\\b(" + svKeywords.join("|") + ")\\b";
    highlightingRules.append({QRegularExpression(keywordPattern), keywordFormat});
    // Strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::green);
    highlightingRules.append({QRegularExpression("\".*\""), stringFormat});

    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    highlightingRules.append({QRegularExpression("//[^\n]*"), commentFormat});

    // Numbers like [3:0] or 32'd10
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#ff8800")); // orange
    highlightingRules.append({QRegularExpression("\\b\\d+'[bhd][0-9a-fA-FxXzZ]+\\b"), numberFormat});
    highlightingRules.append({QRegularExpression("\\b\\d+\\b"), numberFormat});
}

void CodeHighlighter::highlightBlock(const QString &text)
{
    for (const auto &rule : highlightingRules) {
        auto matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            auto match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
