#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include <src/Includes/Utils.h>
#include "PlayerControl.hpp"
#include "UnityEngine.hpp"
#include "ShhhBehaviour.hpp"

// ---------- Variables ----------//
static bool initialized = false;

// ---------- Hooking Table ---------- //
static void __fastcall(*Old_Constructor)(__int64);
static void __fastcall New_Constructor(__int64 a1) {
    LOGI(stringFormat("ShhhBehaviour Instance: %p", a1).c_str());
    PlayerControl::clearInstances();
    Old_Constructor(a1);
}

static __int64 __fastcall(*Old_PlayAnimation)(__int64);
static __int64 __fastcall New_PlayAnimation(__int64 a1) {
    LOGI("ShhhBehaviour.PlayAnimation: entry");
    LOGI(stringFormat("Removed instances: %d", PlayerControl::removeDisposedInstances()).c_str());

    if(UnityEngine::IsEnabledNoclip())
        UnityEngine::EnableNoclip();

    return Old_PlayAnimation(a1);
}

ShhhBehaviour::ShhhBehaviour(uintptr_t pointer) : BaseObject(pointer) {}
ShhhBehaviour::~ShhhBehaviour()  = default;

bool ShhhBehaviour::isInitialized() {
    return initialized;
}

bool ShhhBehaviour::isUsable() {
    return initialized;
}

void ShhhBehaviour::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_Constructor),
                    (void *) New_Constructor,
                    (void **) &Old_Constructor);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_PlayAnimation),
                    (void *) New_PlayAnimation,
                    (void **) &Old_PlayAnimation);

    initialized = true;
}