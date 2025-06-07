#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QElapsedTimer>

class Stopwatch
{
public:
    Stopwatch();

    void start();
    void stop();
    void reset();
    qint64 elapsedMilliseconds() const;
    double elapsedSeconds() const;
    bool isRunning() const;

private:
    QElapsedTimer timer;
    qint64 accumulatedTime;
    bool running;
};

#endif // STOPWATCH_H
