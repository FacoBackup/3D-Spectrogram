#ifndef TCC_IESB_IPANEL_H
#define TCC_IESB_IPANEL_H

#include "../../../common/interface/Synchornizable.h"
#include "../../../common/interface/Initializable.h"

namespace Metal {
    class IPanel : public Syncronizable, public Initializable {
    public:
        virtual void removeAllChildren() {
        }
    };
}
#endif
