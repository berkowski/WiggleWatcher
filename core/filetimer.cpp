
#include <core/filetimer.h>

#include <QDateTime>
#include <QTimer>

#include <chrono>

FileTimer::FileTimer(QObject *parent)
    : QObject(parent)
    , interval_(std::chrono::hours(1))
    , timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &FileTimer::syncTimer);
    connect(timer, &QTimer::timeout, this, &FileTimer::timeout);
}

auto FileTimer::setInterval(int msec) noexcept -> void
{
    setInterval(std::chrono::milliseconds(msec));
}

auto FileTimer::setInterval(std::chrono::milliseconds value) noexcept -> void
{
    auto restart = false;
    if (isActive()) {
        stop();
        restart = true;
    }

    interval_ = value;
    if (restart) {
        start();
    }
}

void FileTimer::start()
{
    if (timer->isActive()) {
        stop();
    }
    initial_sync = true;
    const auto now = std::chrono::milliseconds(QDateTime::currentMSecsSinceEpoch());
    const auto initial_delay = ((now / interval_) + 1) * interval_ - now;
    qWarning() << "initial delay: " << initial_delay.count() << "ms";
    timer->setInterval(initial_delay);
    timer->start();
}

void FileTimer::stop()
{
    timer->stop();
}

auto FileTimer::interval() const noexcept -> int
{
    return static_cast<int>(interval_.count());
}

auto FileTimer::intervalAsDuration() const noexcept -> std::chrono::milliseconds
{
    return interval_;
}

auto FileTimer::isActive() const noexcept -> bool
{
    return timer->isActive();
}

void FileTimer::syncTimer()
{
    if (initial_sync) {
        initial_sync = false;
        //timer->stop();
        timer->setInterval(interval_);
        //timer->start();
    }
}
auto FileTimer::remainingTime() const noexcept -> int
{
    return timer->remainingTime();
}
auto FileTimer::remainingTimeAsDuration() const noexcept -> std::chrono::milliseconds
{
    return timer->remainingTimeAsDuration();
}
