#include "dictserver.h"

SimpleDictClient::SimpleDictClient(const QString &host, int port)
    : host(host), port(port)
{
}

QString SimpleDictClient::define(const QString &word)
{
    if (word.trimmed().isEmpty())
        return "";

    QString error;

    if (!connectToServer(error))
        return "Connection error: " + error;

    QString result = handleDefine(word, error);

    // Fallback: try MATCH if DEFINE failed
    if (result.isEmpty())
    {
        QString matchWord = handleMatch(word, error);
        if (!matchWord.isEmpty())
            result = handleDefine(matchWord, error);
    }

    disconnectFromServer();

    if (result.isEmpty() && !error.isEmpty())
        return "Error: " + error;

    return result;
}

bool SimpleDictClient::connectToServer(QString &error)
{
    socket.connectToHost(host, port);

    if (!socket.waitForConnected(5000))
    {
        error = socket.errorString();
        return false;
    }

    QString line;
    if (!readLine(line, error))
        return false;

    if (!line.startsWith("220"))
    {
        error = "Invalid server greeting: " + line;
        return false;
    }

    // Identify client
    if (!sendCommand("CLIENT simple-client\r\n", error))
        return false;

    if (!readLine(line, error))
        return false;

    // Enable MIME (required by spec, but we don't really use it)
    if (!sendCommand("OPTION MIME\r\n", error))
        return false;

    if (!readLine(line, error))
        return false;

    if (!line.startsWith("250"))
    {
        error = "Server does not support MIME: " + line;
        return false;
    }

    return true;
}

void SimpleDictClient::disconnectFromServer()
{
    if (socket.state() == QTcpSocket::ConnectedState)
    {
        socket.write("QUIT\r\n");
        socket.waitForBytesWritten(1000);
    }

    socket.disconnectFromHost();
}

bool SimpleDictClient::sendCommand(const QString &cmd, QString &error)
{
    socket.write(cmd.toUtf8());

    if (!socket.waitForBytesWritten(2000))
    {
        error = socket.errorString();
        return false;
    }

    return true;
}

bool SimpleDictClient::readLine(QString &line, QString &error)
{
    while (!socket.canReadLine())
    {
        if (!socket.waitForReadyRead(2000))
        {
            error = socket.errorString();
            return false;
        }
    }

    line = QString::fromUtf8(socket.readLine());
    return true;
}

QString SimpleDictClient::handleDefine(const QString &word, QString &error)
{
    QString cmd = QString("DEFINE * \"%1\"\r\n").arg(word);

    if (!sendCommand(cmd, error))
        return "";

    QString line;

    if (!readLine(line, error))
        return "";

    if (line.startsWith("552"))
    {
        // No match
        return "";
    }

    if (!line.startsWith("150"))
    {
        error = "DEFINE failed: " + line;
        return "";
    }

    QString result;

    // Read definitions
    while (true)
    {
        if (!readLine(line, error))
            return "";

        if (line.startsWith("250"))
            break;

        if (line.startsWith("151"))
        {
            // Skip header, read definition block
            QString block = readMultilineBlock(error);
            result += block + "\n";
        }
    }

    return result.trimmed();
}

QString SimpleDictClient::handleMatch(const QString &word, QString &error)
{
    QString cmd = QString("MATCH * prefix \"%1\"\r\n").arg(word);

    if (!sendCommand(cmd, error))
        return "";

    QString line;

    if (!readLine(line, error))
        return "";

    if (line.startsWith("552"))
    {
        // No matches
        return "";
    }

    if (!line.startsWith("152"))
    {
        error = "MATCH failed: " + line;
        return "";
    }

    // Read matches, return first one
    while (true)
    {
        if (!readLine(line, error))
            return "";

        if (line.startsWith("."))
            break;

        // Format: database "word"
        int pos = line.indexOf(' ');
        if (pos >= 0)
        {
            QString match = line.mid(pos + 1).trimmed();

            if (match.startsWith('"') && match.endsWith('"'))
                match = match.mid(1, match.length() - 2);

            return match;
        }
    }

    return "";
}

QString SimpleDictClient::readMultilineBlock(QString &error)
{
    QString result;
    QString line;

    while (true)
    {
        if (!readLine(line, error))
            return "";

        if (line == ".\r\n" || line.trimmed() == ".")
            break;

        result += line;
    }

    return result.trimmed();
}