#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <string>

namespace Metal {
    enum Severity {
        ERROR,
        WARNING,
        SUCCESS
    };

    struct Notification final {
        std::string message;
        Severity severity;
        bool stale = false;

        [[nodiscard]] long long getStartTime() const {
            return startTime;
        }

        Notification(const std::string &message, Severity severity)
            : message(message),
              severity(severity) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime.time_since_epoch()).count();
        }

    private:
        long long startTime;
    };
}
#endif //NOTIFICATION_H
