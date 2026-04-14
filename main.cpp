#include <QApplication>
#include <QClipboard>
#include <QTimer>

#include "popupwindow.h"
#include "dictserver.h"
// #include "popupwindowold.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SimpleDictClient dictClient("localhost");
    PopupWindow popup;
    popup.showWindow();

    QTimer::singleShot(50, [&popup, &dictClient]()
                       {
                           QString clipboardContent = QGuiApplication::clipboard()->text().trimmed();
                           // 1. Validate clipboard content (check length, word count, only alphabetic chars (and chinese or japanese chars)) TODO:later
                           QString displayString = dictClient.define(clipboardContent);
                           popup.setHtml(displayString); });
    return app.exec();
}
