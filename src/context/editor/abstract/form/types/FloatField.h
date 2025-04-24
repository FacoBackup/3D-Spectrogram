#ifndef TCC_IESB_FLOATFIELD_H
#define TCC_IESB_FLOATFIELD_H

#include "../../AbstractPanel.h"
#include "../../../../../common/inspection/InspectedField.h"


namespace Metal {

    class FloatField final : public AbstractPanel {
        InspectedField<float> &field;
    public:

        explicit FloatField(InspectedField<float> &field);

        void onSync() override;

    };

} // Metal

#endif
