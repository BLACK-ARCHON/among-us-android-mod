#pragma once
#include "BaseObject.hpp"
#include <type_traits>

class StatsManager : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_get_Am_Banned = 0x678404
    };

    static StatsManager& getInstance() {
        static StatsManager* instance = new StatsManager(NULL);
        return *instance;
    }

    static void EnableNoLeavePenalty();
    static void DisableNoLeavePenalty();

    StatsManager(uintptr_t pointer);
    ~StatsManager();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};