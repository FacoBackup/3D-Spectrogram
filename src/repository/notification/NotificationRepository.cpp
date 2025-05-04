#include "NotificationRepository.h"

namespace Metal {
    void NotificationRepository::addNotification(std::string message, Severity severity) {
        notifications.emplace_back(message, severity);
    }

    std::vector<Notification> NotificationRepository::getNotifications() const {
        return notifications;
    }

}
