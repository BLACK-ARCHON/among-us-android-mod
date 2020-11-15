#pragma once
#include "BaseObject.hpp"

class MapRoom : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_SetSpecialActive = 0x6A35FC,
        MethodOffset_OOBUpdate = 0x6A33E0,
        MethodOffset_SabotageDoors_IsLessThanZero = 0x6A3A88
    };

    static MapRoom& getInstance() {
        static MapRoom* instance = new MapRoom(NULL);
        return *instance;
    }

    static void EnableNoSabotageCooldown();
    static void DisableNoSabotageCooldown();

    MapRoom(uintptr_t pointer);
    ~MapRoom();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};