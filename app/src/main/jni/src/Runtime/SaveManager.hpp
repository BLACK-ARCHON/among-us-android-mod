#pragma once
#include "BaseObject.hpp"

class SaveManager : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_get_BoughtNoAds = 0xEF67C8
    };

    static SaveManager& getInstance() {
        static SaveManager* instance = new SaveManager(NULL);
        return *instance;
    }

    static void EnableNoAds();
    static void DisableNoAds();

    SaveManager(uintptr_t pointer);
    ~SaveManager();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};