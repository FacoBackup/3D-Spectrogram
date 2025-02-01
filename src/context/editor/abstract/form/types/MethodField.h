
#ifndef TCC_IESB_METHODFIELD_H
#define TCC_IESB_METHODFIELD_H

#include "../../AbstractPanel.h"
#include "../../../../../common/inspection/InspectedMethod.h"


namespace Metal {

    class MethodField : public AbstractPanel {
        InspectedMethod &method;

    public:
        explicit MethodField(InspectedMethod &method);

        void onSync() override;

    };

} // Metal

#endif
