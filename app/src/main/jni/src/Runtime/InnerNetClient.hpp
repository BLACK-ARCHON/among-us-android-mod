#pragma once
#include "BaseObject.hpp"

class InnerNetClient : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_CanBan = 0x742AE8,
        MethodOffset_CanKick = 0x742B0C,
        MethodOffset_CmdAddVote = 0x776F78 //ALPHA
    };

    static InnerNetClient& getInstance() {
        static InnerNetClient* instance = new InnerNetClient(NULL);
        return *instance;
    }

    static void EnableOneShotOneBan();
    static void DisableOneShotOneBan();

    static void EnableOneShotOneKick();
    static void DisableOneShotOneKick();

    InnerNetClient(uintptr_t pointer);
    ~InnerNetClient();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};