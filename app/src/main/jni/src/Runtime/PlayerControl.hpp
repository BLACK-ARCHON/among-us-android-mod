#pragma once
#include "BaseObject.hpp"

class PlayerInfo;

class PlayerControl : public BaseObject {
private:
public:
    enum MethodOffset : uintptr_t {
        MethodOffset_Constructor = 0x693C34,
        MethodOffset_get_CanMove = 0x689914,
        MethodOffset_CmdCheckColor = 0x68F3B0,
        MethodOffset_CmdCheckName = 0x68E948,
        MethodOffset_CmdReportDeadBody = 0x68FAE8,
        MethodOffset_FindClosestTarget_IsImpostor = 0x69265C,
        MethodOffset_MurderPlayer = 0x690020,
        MethodOffset_RpcMurderPlayer = 0x68FF50,
        MethodOffset_RpcSetName = 0x68F1D8
    };

    enum FieldOffset : uintptr_t {
        FieldOffset_GameOptions = 0x08,
        FieldOffset_AllPlayerControls = 0x10,
        FieldOffset_SpawnId = 0x18,
        FieldOffset_NetId = 0x1C,
        FieldOffset_DirtyBits = 0x20,
        FieldOffset_SpawnFlags = 0x24,
        FieldOffset_sendMode = 0x25,
        FieldOffset_OwnerId = 0x28,
        FieldOffset_DespawnOnDestroy = 0x2C,
        FieldOffset_LastStartCounter = 0x30,
        FieldOffset_PlayerId = 0x34,
        FieldOffset_MaxReportDistance = 0x38,
        FieldOffset_moveable = 0x3C,
        FieldOffset_inVent = 0x3D,
        FieldOffset__cachedData = 0x40,
        FieldOffset_FootSteps = 0x48,
        FieldOffset_KillSfx = 0x50,
        FieldOffset_KillAnimations = 0x58,
        FieldOffset_killTimer = 0x60,
        FieldOffset_RemainingEmergencies = 0x64,
        FieldOffset_nameText = 0x68,
        FieldOffset_LightPrefab = 0x70,
        FieldOffset_myLight = 0x78,
        FieldOffset_Collider = 0x80,
        FieldOffset_MyPhysics = 0x88,
        FieldOffset_NetTransform = 0x90,
        FieldOffset_CurrentPet = 0x98,
        FieldOffset_HatRenderer = 0xA0,
        FieldOffset_myRend = 0xA8,
        FieldOffset_hitBuffer = 0xB0,
        FieldOffset_myTasks = 0xB8,
        FieldOffset_ScannerAnims = 0xC0,
        FieldOffset_ScannersImages = 0xC8,
        FieldOffset_closest = 0xD0,
        FieldOffset_isNew = 0xD8,
        FieldOffset_cache = 0xE0,
        FieldOffset_itemsInRange = 0xE8,
        FieldOffset_newItemsInRange = 0xF0,
        FieldOffset_scannerCount = 0xF8,
        FieldOffset_infectedSet = 0xF9
    };

    template<FieldOffset Offset>
    using DetermineFieldType =
    typename std::conditional<Offset == FieldOffset_SpawnId || Offset == FieldOffset_NetId || Offset == FieldOffset_DirtyBits, unsigned int,
    typename std::conditional<Offset == FieldOffset_SpawnFlags || Offset == FieldOffset_sendMode
            || Offset == FieldOffset_PlayerId || Offset == FieldOffset_scannerCount, unsigned char,
    typename std::conditional<Offset == FieldOffset_DespawnOnDestroy || Offset == FieldOffset_moveable || Offset == FieldOffset_inVent
            || Offset == FieldOffset_isNew || Offset == FieldOffset_infectedSet, bool,
    typename std::conditional<Offset == FieldOffset_OwnerId || Offset == FieldOffset_LastStartCounter || Offset == FieldOffset_RemainingEmergencies, signed int,
    typename std::conditional<Offset == FieldOffset_MaxReportDistance || Offset == FieldOffset_killTimer, float, uintptr_t>::type>::type>::type>::type>::type;

    static PlayerControl& getInstance() {
        static PlayerControl* instance = new PlayerControl(NULL);
        return *instance;
    }

    static void addInstance(uintptr_t pointer);
    static void clearInstances();
    static size_t removeDisposedInstances();

    static std::vector<PlayerControl> getInstances();
    static PlayerControl getLocalPlayer();

    static void EnableCounterImpostor();
    static void DisableCounterImpostor();

    static void EnableLockdownImpostor();
    static void DisableLockdownImpostor();

    static void EnableKillByOtherHand();
    static void DisableKillByOtherHand();
    static void EnableImpostorCanKillImpostor();
    static void DisableImpostorCanKillImpostor();

    static void EnableCanMoveInVent();
    static void DisableCanMoveInVent();

    static void EnableEmptyName();
    static void DisableEmptyName();

    static void EmptyName();

    static void ReportSomeone();

    PlayerControl(uintptr_t pointer);
    ~PlayerControl();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;

    bool isLocalPlayer();

    PlayerInfo getPlayerInfo();

    void CmdCheckColor(unsigned int colorId);
    void CmdReportDeadBody(PlayerControl target);
};