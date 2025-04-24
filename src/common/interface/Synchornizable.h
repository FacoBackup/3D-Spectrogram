#ifndef TCC_IESB_SYNCHORNIZABLE_H
#define TCC_IESB_SYNCHORNIZABLE_H

namespace Metal {
    class Syncronizable {
    public:
        virtual ~Syncronizable() = default;

        virtual void onSync() {}
    };
}

#endif
