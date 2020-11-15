#pragma once
#include "BaseObject.hpp"

class NoShadowBehaviour : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_LateUpdate_IsDead = 0x6AEF78
    };

    static NoShadowBehaviour& getInstance() {
        static NoShadowBehaviour* instance = new NoShadowBehaviour(NULL);
        return *instance;
    }

    static void EnableSuperVision();
    static void DisableSuperVision();

    NoShadowBehaviour(uintptr_t pointer);
    ~NoShadowBehaviour();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};