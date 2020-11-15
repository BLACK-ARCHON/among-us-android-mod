#pragma once
#include "BaseObject.hpp"

class KillButtonManager : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_SetCoolDown = 0x74E1C4
    };

    static KillButtonManager& getInstance() {
        static KillButtonManager* instance = new KillButtonManager(NULL);
        return *instance;
    }

    static void EnableNoKillCooldown();
    static void DisableNoKillCooldown();

    KillButtonManager(uintptr_t pointer);
    ~KillButtonManager();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};