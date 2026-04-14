#pragma once

#include <QString>
#include <QTcpSocket>

class SimpleDictClient
{
public:
    explicit SimpleDictClient(const QString &host, int port = 2628);

    // Main API you will use
    QString define(const QString &word);

private:
    QString host;
    int port;

    QTcpSocket socket;

    // --- Core steps ---
    bool connectToServer(QString &error);
    void disconnectFromServer();

    bool sendCommand(const QString &cmd, QString &error);
    bool readLine(QString &line, QString &error);

    // --- DICT logic ---
    QString handleDefine(const QString &word, QString &error);
    QString handleMatch(const QString &word, QString &error);

    QString readMultilineBlock(QString &error);
};
