#ifndef NOTIFICATIONREPOSITORY_H
#define NOTIFICATIONREPOSITORY_H
#include <vector>

#include "Notification.h"

namespace Metal {
    struct Notification;

    struct NotificationRepository {
        void addNotification(std::string message, Severity severity = ERROR);

        std::vector<Notification> getNotifications() const;

    private:
        std::vector<Notification> notifications{};
    };
}
#endif //NOTIFICATIONREPOSITORY_H
