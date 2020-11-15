#include <unistd.h>
#include <src/KittyMemory/KittyMemory.h>
#include <src/KittyMemory/KittyUtils.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include <src/Includes/obfuscate.h>
#include "PlayerControl.hpp"
#include "UnityEngine.hpp"

// ---------- Variables ---------- //
static bool initialized = false;
static unsigned char layerName_Ghost[48] = { 0, };
static unsigned char layerName_Players[48] = { 0, };

// ---------- Patch Table ---------- //
static bool noclip = false;

// ---------- Function Table ---------- //
__int64 __cdecl(*PF_Component_get_gameObject)(__int64, __int64);
int32_t __cdecl(*PF_LayerMask_NameToLayer)(__int64, __int64);
int32_t __cdecl(*PF_GameObject_get_layer)(__int64, __int64);
void __cdecl(*PF_GameObject_set_layer)(__int64, int32_t, __int64);

bool UnityEngine::IsEnabledNoclip() {
    return noclip;
}

void UnityEngine::EnableNoclip() {
    SetLayerMask(PlayerControl::getLocalPlayer().getPointer(), PF_LayerMask_NameToLayer((__int64)layerName_Ghost, NULL));
    noclip = true;
}

void UnityEngine::DisableNoclip() {
    SetLayerMask(PlayerControl::getLocalPlayer().getPointer(), PF_LayerMask_NameToLayer((__int64)layerName_Players, NULL));
    noclip = false;
}

void UnityEngine::SetLayerMask(uintptr_t pInstance, int32_t layerMask) {
    PlayerControl instance(pInstance);

    if(!initialized || !instance.isUsable())
        return;

    auto gameObject = PF_Component_get_gameObject(pInstance, NULL);
    PF_GameObject_set_layer(gameObject, layerMask, NULL);
}

UnityEngine::UnityEngine(uintptr_t pointer) : BaseObject(pointer) { }
UnityEngine::~UnityEngine() = default;

bool UnityEngine::isInitialized() {
    return initialized;
}

bool UnityEngine::isUsable() {
    return initialized;
}

void UnityEngine::init() {
    if(initialized)
        return;

    KittyUtils::fromHex("0000000000000000000000000000000005000000470068006F0073007400", layerName_Ghost);
    KittyUtils::fromHex("000000000000000000000000000000000700000050006C0061007900650072007300", layerName_Players);

    PF_Component_get_gameObject = (__int64(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_Component_get_gameObject);
    PF_LayerMask_NameToLayer = (int32_t(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_LayerMask_NameToLayer);
    PF_GameObject_get_layer = (int32_t(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_GameObject_get_layer);
    PF_GameObject_set_layer = (void(*)(__int64, int32_t, __int64))getAbsoluteAddress(libName, MethodOffset_GameObject_set_layer);

    initialized = true;
}


