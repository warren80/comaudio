#ifndef LOGS_H
#define LOGS_H

#include <QObject>
#include <QFile>

class Logs:public QObject
{
    Q_OBJECT

public:
    explicit Logs(QString pathName, QString title);
    /**
     * Call this function to write to a log in 'error' format.
     *
     * @author Karl Castillo
     * @arg error - error message
     *      errNo - the errno associated with the error.
     */
    void writeToLog(const char *error, int errNo);
    /**
     * Call this function to write to a log in 'generic' format.
     *
     * @author Karl Castillo
     * @arg message - the message that will be written to the log
     */
    void writeToLog(const char *message);
    /**
     * Call this function to write to a log in 'generic' format.
     *
     * @author Karl Castillo
     * @arg message - the message that will be written to the log
     */
    void writeToLog(QString message);

private:
    /**
     * The file where the log will be saved to.
     */
    QFile log_;
};

#endif // LOGS_H
