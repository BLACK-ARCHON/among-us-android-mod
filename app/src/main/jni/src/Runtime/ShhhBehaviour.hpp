#pragma once
#include "BaseObject.hpp"

class ShhhBehaviour : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_Constructor = 0xF01640,
        MethodOffset_PlayAnimation = 0xF01364
    };

    static ShhhBehaviour& getInstance() {
        static ShhhBehaviour* instance = new ShhhBehaviour(NULL);
        return *instance;
    }

    ShhhBehaviour(uintptr_t pointer);
    ~ShhhBehaviour();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};