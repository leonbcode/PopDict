#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QGuiApplication>
#include <QRegularExpression>

#include "popupwindow.h"
#include "dictserver.h"

// Forward declaration
bool validateClipboardContent(const QString &text);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SimpleDictClient dictClient("localhost");
    PopupWindow popup;
    popup.showWindow();

    QTimer::singleShot(50, [&popup, &dictClient]()
                       {
        QString clipboardContent = QGuiApplication::clipboard()->text().trimmed();
        QString displayString;

        if (validateClipboardContent(clipboardContent)) {
            displayString = dictClient.define(clipboardContent);
        } else {
            displayString = "Invalid clipboard content";
        }

        popup.setHtml(displayString); });
    return app.exec();
}

bool validateClipboardContent(const QString &text)
{
    // Rule 1: length limit
    if (text.length() > 20)
        return false;

    // Rule 2: word count max 5
    const QStringList words = text.split(QRegularExpression("\\s+"),
                                         Qt::SkipEmptyParts);
    if (words.size() > 5)
        return false;

    // Rule 3: allowed characters (alphanumeric + CJK + spaces)
    static const QRegularExpression allowedPattern(R"(^[\p{Han}0-9A-Za-z\s]+$)");

    if (!allowedPattern.match(text).hasMatch())
        return false;

    return true;
}