#pragma once
#include "BaseObject.hpp"

class SabotageSystemType : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_get_AnyActive = 0xEF33A8,
        MethodOffset_get_PercentCool = 0xEF3398,
        MethodOffset_get_Timer = 0xEF3388
    };

    static SabotageSystemType& getInstance() {
        static SabotageSystemType* instance = new SabotageSystemType(NULL);
        return *instance;
    }

    static void EnableNoSabotageCooldown();
    static void DisableNoSabotageCooldown();

    SabotageSystemType(uintptr_t pointer);
    ~SabotageSystemType();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};