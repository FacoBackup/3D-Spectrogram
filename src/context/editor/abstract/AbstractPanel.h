#ifndef TCC_IESB_ABSTRACTPANEL_H
#define TCC_IESB_ABSTRACTPANEL_H

#include <string>
#include <vector>

#include "IPanel.h"

namespace Metal {
    class ApplicationContext;

    class AbstractPanel : public IPanel {
    protected:
        ApplicationContext *context = nullptr;
        std::vector<IPanel *> children;
        const std::string id;

        virtual void onSyncChildren() const;

    public:
        explicit AbstractPanel();

        std::vector<IPanel *> &getChildren();

        void appendChild(AbstractPanel *panel);

        void removeAllChildren() override;

        void setContext(ApplicationContext *context);

        virtual void onRemove() {
        }
    };
}
#endif
