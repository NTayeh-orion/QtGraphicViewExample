#ifndef CUSTOMTOOLTIP_H
#define CUSTOMTOOLTIP_H

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QWidget>
#include <qgraphicsview.h>
#include <QParallelAnimationGroup>
class CustomToolTip : public QWidget
{
    Q_OBJECT
public:
    explicit CustomToolTip(QWidget *parent = nullptr)
        : QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_ShowWithoutActivating);

        // Text label
        label = new QLabel(this);
        label->setStyleSheet(R"(
            QLabel {
        background-color: #1e1e2a;      /* Dark blue background */
        color: #00c8ff;                 /* Cyan text */
        border: 2px solid #00c8ff;      /* Neon cyan border */
        border-radius: 6px;
        font: 10pt 'Consolas';
        padding: 6px 10px;
            }
        )");
        label->setAlignment(Qt::AlignLeft);
        label->setMargin(8);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(label);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);

        // Shadow effect
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);                 // soft blur
        shadow->setColor(QColor(0, 200, 255, 150)); // cyan glow with transparency
        shadow->setOffset(0, 0);                  // centered glow
        label->setGraphicsEffect(shadow);

        // Fade animation
        fadeAnim = new QPropertyAnimation(this, "windowOpacity", this);
        fadeAnim->setDuration(200);


        scaleAnim = new QPropertyAnimation(this, "geometry", this);
        scaleAnim->setDuration(200);
        scaleAnim->setEasingCurve(QEasingCurve::OutBack);
    }

    void showText(QGraphicsView *view, const QPointF &scenePos, const QString &text)
    {
        if (!view)
            return;

        label->setText(text);
        label->adjustSize();

        // map scene position to view coordinates
        QPoint viewPos = view->mapFromScene(scenePos);

        // map view coordinates to global screen coordinates
        QPoint globalPos = view->viewport()->mapToGlobal(viewPos);

        // offset above the pin
        QPoint offset(0, -label->height() - 5);

        move(globalPos + offset);
        resize(label->size()); // ensure geometry matches label size
        QRect finalRect = geometry();         // final geometry
        QRect startRect(globalPos + offset, QSize(0,0)); // start from zero

        fadeAnim->stop();
        setWindowOpacity(0.0);
        show();
        raise();

        fadeAnim->setStartValue(0.0);
        fadeAnim->setEndValue(1.0);
        fadeAnim->start();


        // Scale in
        scaleAnim->stop();
        scaleAnim->setStartValue(startRect);
        scaleAnim->setEndValue(finalRect);
        scaleAnim->start();

    }

    void hideSmooth()
    {
        // fadeAnim->stop();
        // fadeAnim->setStartValue(windowOpacity());
        // fadeAnim->setEndValue(0.0);
        // connect(fadeAnim, &QPropertyAnimation::finished, this, &QWidget::hide);
        // fadeAnim->start();

        // Stop any running animations
        fadeAnim->stop();
        scaleAnim->stop();

        // Fade out
        fadeAnim->setStartValue(windowOpacity());
        fadeAnim->setEndValue(0.0);

        // Scale down towards center
        QRect currentRect = geometry();
        QPoint center = currentRect.center();
        QRect endRect(center, QSize(0,0)); // shrink to center
        scaleAnim->setStartValue(currentRect);
        scaleAnim->setEndValue(endRect);

        // When both animations finish, hide the widget
        // Using QParallelAnimationGroup ensures sync
        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(fadeAnim);
        group->addAnimation(scaleAnim);
        connect(group, &QParallelAnimationGroup::finished, this, &QWidget::hide);
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }

private:
    QLabel *label;
    QPropertyAnimation *fadeAnim;
    QPropertyAnimation *scaleAnim; // <-- new
};
#endif // CUSTOMTOOLTIP_H
