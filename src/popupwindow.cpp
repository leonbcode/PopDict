#include "popupwindow.h"
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QTimer>
#include <QApplication>
#include <QShortcut>
#include <QVBoxLayout>
#include <QScreen>
#include <QCursor>

PopupWindow::PopupWindow()
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumSize(250, 50);
    setMaximumSize(500, 400);

    browser = new QTextBrowser(this);
    browser->setFrameShape(QFrame::NoFrame);
    browser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    browser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    browser->setStyleSheet("background: transparent;");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(browser);

    QShortcut *esc = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    esc->setContext(Qt::ApplicationShortcut);
    connect(esc, &QShortcut::activated, qApp, &QApplication::quit);
}

void PopupWindow::showWindow()
{
    show();
    raise();
    activateWindow();
}

void PopupWindow::setHtml(const QString &html)
{
    browser->setPlainText(html);
    browser->document()->adjustSize();

    int w = browser->document()->idealWidth();
    int h = browser->document()->size().height();

    int margin = 20;

    w += margin;
    h += margin;

    w = qMin(w, maximumWidth());
    h = qMin(h, maximumHeight());

    resize(w, h);
}

void PopupWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(palette().color(QPalette::Window));
    painter.setPen(Qt::NoPen);
    QPainterPath path;
    path.addRoundedRect(rect(), 12, 12);
    painter.drawPath(path);
}

void PopupWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::ActivationChange && !isActiveWindow())
    {
        QTimer::singleShot(0, qApp, &QApplication::quit);
    }
}