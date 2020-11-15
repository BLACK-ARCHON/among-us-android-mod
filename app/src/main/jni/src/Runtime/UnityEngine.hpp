#pragma once
#include "BaseObject.hpp"
#include <type_traits>

class UnityEngine : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_Component_get_gameObject = 0xAEE5D8,
        MethodOffset_LayerMask_NameToLayer = 0xAF630C,
        MethodOffset_GameObject_get_layer = 0xAF5834,
        MethodOffset_GameObject_set_layer = 0xAF5874
    };

    static UnityEngine& getInstance() {
        static UnityEngine* instance = new UnityEngine(NULL);
        return *instance;
    }

    static bool IsEnabledNoclip();
    static void EnableNoclip();
    static void DisableNoclip();
    static void SetLayerMask(uintptr_t pInstance, int32_t layerMask);

    UnityEngine(uintptr_t pointer);
    ~UnityEngine();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};