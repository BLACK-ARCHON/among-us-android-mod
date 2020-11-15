//0x73B03C, 0x73B044 = NOP

#pragma once
#include "BaseObject.hpp"

class ChatController : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_AddChat_IsDead_A = 0x73B03C,
        MethodOffset_AddChat_IsDead_B = 0x73B044,
        MethodOffset_AddChat_IsImpostor = 0x73B194
    };

    static ChatController& getInstance() {
        static ChatController* instance = new ChatController(NULL);
        return *instance;
    }



    static void EnableCanSeeGhostsChat();
    static void DisableCanSeeGhostsChat();

    static void EnableCrewmateCanSeeImpostor();
    static void DisableCrewmateCanSeeImpostor();

    ChatController(uintptr_t pointer);
    ~ChatController();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};