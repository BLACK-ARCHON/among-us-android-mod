#include <unistd.h>
#include <src/KittyMemory/MemoryPatch.h>
#include "NoShadowBehaviour.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static struct PatchTable {
    MemoryPatch SuperVision;
} patchTable;

void NoShadowBehaviour::EnableSuperVision() {
    if(initialized)
        patchTable.SuperVision.Modify();
}
void NoShadowBehaviour::DisableSuperVision() {
    if(initialized)
        patchTable.SuperVision.Restore();
}

NoShadowBehaviour::NoShadowBehaviour(uintptr_t pointer) : BaseObject(pointer) {}
NoShadowBehaviour::~NoShadowBehaviour() = default;

bool NoShadowBehaviour::isInitialized() {
    return initialized;
}

bool NoShadowBehaviour::isUsable() {
    return initialized;
}

void NoShadowBehaviour::init() {
    if(initialized)
        return;

    patchTable.SuperVision = MemoryPatch::createWithHex(libName, MethodOffset_LateUpdate_IsDead, "E8000035");
    initialized = true;
}