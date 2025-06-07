#include "../include/stopwatch.h"

Stopwatch::Stopwatch()
    : accumulatedTime(0), running(false)
{
}
void Stopwatch::start()
{
    if (!running) {
        timer.start();
        running = true;
    }
}

void Stopwatch::stop()
{
    if (running) {
        accumulatedTime += timer.elapsed();
        running = false;
    }
}

void Stopwatch::reset()
{
    accumulatedTime = 0;
    running = false;
}

qint64 Stopwatch::elapsedMilliseconds() const
{
    if (running)
        return accumulatedTime + timer.elapsed();
    else
        return accumulatedTime;
}

double Stopwatch::elapsedSeconds() const
{
    return elapsedMilliseconds() / 1000.0;
}

bool Stopwatch::isRunning() const
{
    return running;
}
