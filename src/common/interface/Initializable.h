#ifndef TCC_IESB_INITIALIZABLE_H
#define TCC_IESB_INITIALIZABLE_H

namespace Metal {
    class Initializable {
    public:
        virtual ~Initializable() = default;

        virtual void onInitialize() {}
    };
}

#endif
