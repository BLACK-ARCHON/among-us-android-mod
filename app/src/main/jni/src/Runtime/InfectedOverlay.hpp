#pragma once
#include "BaseObject.hpp"

class InfectedOverlay : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_get_CanUseDoors = 0x73C974,
        MethodOffset_get_CanUseSpecial = 0x73C9FC
    };

    static InfectedOverlay& getInstance() {
        static InfectedOverlay* instance = new InfectedOverlay(NULL);
        return *instance;
    }

    static void EnableNoSabotageCooldown();
    static void DisableNoSabotageCooldown();

    InfectedOverlay(uintptr_t pointer);
    ~InfectedOverlay();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};