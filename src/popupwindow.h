#pragma once

#include <QWidget>
#include <QTextBrowser>

class PopupWindow : public QWidget {
    Q_OBJECT
public:
    PopupWindow();

    void showWindow();
    void setHtml(const QString &html);

protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    QTextBrowser *browser;
};