#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "SaveManager.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noAds = false;

// ---------- Hooking Table ---------- //
bool __cdecl(*Old_get_BoughtNoAds)(__int64);
bool __cdecl New_get_BoughtNoAds(__int64 a1) {
    return noAds ? true : Old_get_BoughtNoAds(a1);
}

void SaveManager::EnableNoAds() {
    noAds = true;
}

void SaveManager::DisableNoAds() {
    noAds = false;
}

SaveManager::SaveManager(uintptr_t pointer) : BaseObject(pointer) {}
SaveManager::~SaveManager() = default;

bool SaveManager::isInitialized() {
    return initialized;
}

bool SaveManager::isUsable() {
    return initialized;
}

void SaveManager::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_BoughtNoAds),
                    (void *) New_get_BoughtNoAds,
                    (void **) &Old_get_BoughtNoAds);

    initialized = true;
}