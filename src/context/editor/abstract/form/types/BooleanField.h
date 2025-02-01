#ifndef TCC_IESB_BOOLEANFIELD_H
#define TCC_IESB_BOOLEANFIELD_H

#include "../../AbstractPanel.h"
#include "../../../../../common/inspection/InspectedField.h"

namespace Metal {
    class BooleanField final : public AbstractPanel {
        InspectedField<bool> &field;
    public:
        explicit BooleanField(InspectedField<bool> &field);

        void onSync() override;
    };

} // Metal

#endif
