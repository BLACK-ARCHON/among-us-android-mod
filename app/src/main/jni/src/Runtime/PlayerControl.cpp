#include <random>
#include <unistd.h>
#include <vector>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "src/KittyMemory/MemoryPatch.h"
#include "../Includes/Utils.h"
#include "PlayerInfo.hpp"
#include "PlayerControl.hpp"

// ---------- Variables ---------- //
static bool initialized = false;
static std::vector<PlayerControl> instances;
static unsigned char emptyNameString[48] = { 0, };

// ---------- Patch Table ---------- //
static struct PatchTable {
    MemoryPatch ImpostorCanKillImpostor;
} patchTable;

static bool counterImpostor = false;
static bool lockdownImpostor = false;
static bool killByOtherHand = false;
static bool canMoveInVent = false;
static bool emptyName = false;

// ---------- Function Table ---------- //
static void __fastcall (*PF_CmdCheckColor)(__int64, unsigned int);
static void __fastcall (*PF_CmdCheckName)(__int64, __int64);
static void __fastcall (*PF_CmdReportDeadBody)(__int64, __int64);
static void __fastcall (*PF_RpcMurderPlayer)(__int64, __int64);

// ---------- Hooking Table ---------- //
static __int64 __fastcall(*Old_Constructor)(__int64);
static __int64 __fastcall New_Constructor(__int64 a1) {
    LOGI(stringFormat("PlayerControl Instance: %p", a1).c_str());
    instances.emplace_back(a1);
    return Old_Constructor(a1);
}

static void __fastcall(*Old_MurderPlayer)(__int64, __int64);
static void __fastcall New_MurderPlayer(__int64 a1, __int64 a2) {
    PlayerControl killer(a1);
    PlayerControl victim(a2);

    Old_MurderPlayer(a1, a2);

    if(counterImpostor && victim.isLocalPlayer())
        PF_RpcMurderPlayer(a1, a1);
}

static void __fastcall(*Old_RpcMurderPlayer)(__int64, __int64);
static void __fastcall New_RpcMurderPlayer(__int64 a1, __int64 a2) {
    PlayerControl killer(a1);
    PlayerControl victim(a2);

    if(killByOtherHand) {
        for(auto instance: PlayerControl::getInstances()) {
            if(!instance.isLocalPlayer()) {
                if (instance.getPlayerInfo()
                            .get<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset_IsImpostor>>(
                                    PlayerInfo::FieldOffset_IsImpostor)
                    && !instance.getPlayerInfo()
                        .get<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset_IsDead>>(
                                PlayerInfo::FieldOffset_IsDead)) {

                    a1 = instance.getPointer();
                }
            }
        }
    }

    Old_RpcMurderPlayer(a1, a2);
}

static void __fastcall(*Old_RpcSetName)(__int64, __int64);
static void __fastcall New_RpcSetName(__int64 a1, __int64 a2) {
    PlayerControl caller(a1);

    if(caller.isLocalPlayer() && emptyName)
        a2 = (__int64)emptyNameString;

    Old_RpcSetName(a1, a2);
}

static bool __fastcall(*Old_get_CanMove)(__int64, int, char, int, int, int, __int64, char);
static bool __fastcall New_get_CanMove(__int64 a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8) {
    if(canMoveInVent)
        return true;
    return Old_get_CanMove(a1, a2, a3, a4, a5, a6, a7, a8);
}

// ---------- Static Function Table ---------- //
void PlayerControl::addInstance(uintptr_t pointer) {
    instances.emplace_back(pointer);
}

void PlayerControl::clearInstances() {
    instances.clear();
}

size_t PlayerControl::removeDisposedInstances() {
    size_t result = 0;

    for(auto iter = instances.begin(); iter != instances.end(); ++iter) {
        if(!iter->isUsable()) {
            instances.erase(iter);
            ++result;
        }
    }

    return result;
}

std::vector<PlayerControl> PlayerControl::getInstances() {
    return instances;
}

PlayerControl PlayerControl::getLocalPlayer() {
    PlayerControl result(NULL);

    for(auto instance: instances) {
        if(instance.isLocalPlayer()) {
            result.setPointer(instance.getPointer());
            break;
        }
    }

    return result;
}

void PlayerControl::EnableCounterImpostor() {
    counterImpostor = true;
}

void PlayerControl::DisableCounterImpostor() {
    counterImpostor = false;
}

void PlayerControl::EnableLockdownImpostor() {
    lockdownImpostor = true;
}

void PlayerControl::DisableLockdownImpostor() {
    lockdownImpostor = false;
}

void PlayerControl::EnableKillByOtherHand() {
    killByOtherHand = true;
}

void PlayerControl::DisableKillByOtherHand() {
    killByOtherHand = false;
}

void PlayerControl::EnableImpostorCanKillImpostor() {
    if(initialized)
        patchTable.ImpostorCanKillImpostor.Modify();
}

void PlayerControl::DisableImpostorCanKillImpostor() {
    if(initialized)
        patchTable.ImpostorCanKillImpostor.Restore();
}

void PlayerControl::EnableCanMoveInVent() {
    canMoveInVent = true;
}

void PlayerControl::DisableCanMoveInVent() {
    canMoveInVent = false;
}

void PlayerControl::EnableEmptyName() {
    emptyName = true;
}

void PlayerControl::DisableEmptyName() {
    emptyName = false;
}

void PlayerControl::ReportSomeone() {
    if(initialized) {
        int retries = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        PlayerControl targetA(NULL);
        PlayerControl targetB(NULL);

        std::uniform_int_distribution<int> dis(0, instances.size());

        while(true) {
            ++retries;

            if(retries > 30)
                break;

            targetA.setPointer(instances[dis(gen)].getPointer());
            targetB.setPointer(instances[dis(gen)].getPointer());

            if(!targetA.isUsable() || !targetB.isUsable())
                continue;

            if(targetA.isLocalPlayer())
                continue;

            targetA.CmdReportDeadBody(targetB);
            break;
        }
    }
}

PlayerControl::PlayerControl(uintptr_t pointer) : BaseObject(pointer) { }
PlayerControl::~PlayerControl()  = default;

bool PlayerControl::isInitialized() {
    return initialized;
}

bool PlayerControl::isUsable() {
    return initialized && getPointer() != NULL && get<DetermineFieldType<FieldOffset_NetId>>(FieldOffset_NetId) < UINT32_MAX;
}

void PlayerControl::init() {
    if(initialized)
        return;

    KittyUtils::fromHex("00000000000000000000000000000000040000002000200020002000", emptyNameString);

    PF_CmdCheckColor = (void(*)(__int64, unsigned int))getAbsoluteAddress(libName, MethodOffset_CmdCheckColor);
    PF_CmdCheckName = (void(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_CmdCheckName);
    PF_CmdReportDeadBody = (void(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_CmdReportDeadBody);
    PF_RpcMurderPlayer = (void(*)(__int64, __int64))getAbsoluteAddress(libName, MethodOffset_RpcMurderPlayer);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_Constructor),
                    (void *) New_Constructor,
                    (void **) &Old_Constructor);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_CanMove),
                    (void *) New_get_CanMove,
                    (void **) &Old_get_CanMove);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_MurderPlayer),
                    (void *) New_MurderPlayer,
                    (void **) &Old_MurderPlayer);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_RpcMurderPlayer),
                    (void *) New_RpcMurderPlayer,
                    (void **) &Old_RpcMurderPlayer);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_RpcSetName),
                    (void *) New_RpcSetName,
                    (void **) &Old_RpcSetName);

    patchTable.ImpostorCanKillImpostor = MemoryPatch::createWithHex(libName, MethodOffset_FindClosestTarget_IsImpostor, "1F2003D5");
    initialized = true;
}

bool PlayerControl::isLocalPlayer() {
    return isUsable() && get<DetermineFieldType<FieldOffset_myLight>>(FieldOffset_myLight) != NULL;
}

PlayerInfo PlayerControl::getPlayerInfo() {
    if(!isUsable())
        return { NULL };

    return PlayerInfo(get<DetermineFieldType<FieldOffset__cachedData>>(FieldOffset__cachedData));
}

void PlayerControl::CmdCheckColor(unsigned int colorId) {
      if(isUsable())
          PF_CmdCheckColor(getPointer(), colorId);
}

void PlayerControl::CmdReportDeadBody(PlayerControl target) {
    if(isUsable() && target.isUsable())
        PF_CmdReportDeadBody(getPointer(), target.getPointer());
}