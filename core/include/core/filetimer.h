#ifndef WIGGLEWATCHER_FILETIMER_H
#define WIGGLEWATCHER_FILETIMER_H

#include <QObject>

class QObject;
class QTimer;

class FileTimer : public QObject
{
    Q_OBJECT
public:
    Q_SIGNAL void timeout();

    explicit FileTimer(QObject *parent = nullptr);
    ~FileTimer() override = default;

    auto setInterval(int msec) noexcept -> void;
    auto setInterval(std::chrono::milliseconds value) noexcept -> void;

    [[nodiscard]] auto interval() const noexcept -> int;
    [[nodiscard]] auto intervalAsDuration() const noexcept -> std::chrono::milliseconds;

    [[nodiscard]] auto remainingTime() const noexcept -> int;
    [[nodiscard]] auto remainingTimeAsDuration() const noexcept -> std::chrono::milliseconds;

    [[nodiscard]] auto isActive() const noexcept -> bool;

    Q_SLOT void start();
    Q_SLOT void stop();

private:
    Q_SLOT void syncTimer();
    QTimer *timer;
    bool initial_sync = true;
    std::chrono::milliseconds interval_;
};

#endif //WIGGLEWATCHER_FILETIMER_H
