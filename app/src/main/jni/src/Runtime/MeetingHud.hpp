#pragma once
#include "BaseObject.hpp"
#include <type_traits>

class MeetingHud : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_CmdCastVote = 0x6A6FC4,
        MethodOffset_CreateButton_IsImpostor = 0x6A9E48
    };

    static MeetingHud& getInstance() {
        static MeetingHud* instance = new MeetingHud(NULL);
        return *instance;
    }

    static void EnableCrewmateCanSeeImpostor();
    static void DisableCremwateCanSeeImpostor();
    static void EnableOneShotOneExile();
    static void DisableOneShotOneExile();

    MeetingHud(uintptr_t pointer);
    ~MeetingHud();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};