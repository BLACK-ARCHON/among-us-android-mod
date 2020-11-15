#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <jni.h>
#include <list>
#include <pthread.h>
#include <unistd.h>

#include "src/Includes/base64.hpp"
#include "src/KittyMemory/MemoryPatch.h"
#include "src/Includes/Logger.h"
#include "src/Includes/Utils.h"
#include "src/Includes/obfuscate.h"

#include "Menu/Sounds.h"
#include "Menu/Menu.h"
#include "Toast.h"


#include <src/And64InlineHook/And64InlineHook.hpp>
#include "src/Runtime/ChatController.hpp"
#include "src/Runtime/InfectedOverlay.hpp"
#include "src/Runtime/InnerNetClient.hpp"
#include "src/Runtime/KillButtonManager.hpp"
#include "src/Runtime/MapRoom.hpp"
#include "src/Runtime/MeetingHud.hpp"
#include "src/Runtime/NoShadowBehaviour.hpp"
#include "src/Runtime/PlayerControl.hpp"
#include "src/Runtime/PlayerInfo.hpp"
#include "src/Runtime/SabotageSystemType.hpp"
#include "src/Runtime/SaveManager.hpp"
#include "src/Runtime/ShhhBehaviour.hpp"
#include "src/Runtime/StatsManager.hpp"
#include "src/Runtime/UnityEngine.hpp"

extern "C" JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;
    featureListValid = true;

    // Note: Do not translate the first text
    // Usage:
    // Category_(text)
    // Toggle_[feature name]
    // SeekBar_[feature name]_[min value]_[max value]
    // Spinner_[feature name]_[Items e.g. item1_item2_item3]
    // Button_[feature name]
    // Button_OnOff_[feature name]
    // InputValue_[feature name]
    const char *features[] = {
            "Category_PLAYER",
            "Button_BE ALIVE", //2
            "Button_BE DEAD", //3
            "Button_OnOff_SUPER VISION", //4
            "Button_OnOff_COUNTER IMPOSTOR", //5
            "Button_OnOff_CAN SEE GHOSTS CHAT", //6
            "SeekBar_COLOR_0_9", //7

            "Category_CREWMATE",
            "Button_BE IMPOSTOR", //9
            "Button_OnOff_CREWMATE CAN SEE IMPOSTOR", //10

            "Category_IMPOSTOR",
            "Button_BE CREWMATE", //12
            "Button_OnOff_NO KILL COOLDOWN", //13
            "Button_OnOff_IMPOSTOR CAN KILL IMPOSTOR", //14

            "Category_MOVEMENT",
            "Button_OnOff_CAN MOVE IN VENT", //16
            "Button_OnOff_NOCLIP", //17

            "Category_SABOTAGE",
            "Button_OnOff_NO SABOTAGE COOLDOWN", //19
            "Button_OnOff_KILL BY OTHER HAND", //20
            "Button_EMPTY NAME", //21
            "Button_REPORT SOMEONE", //22

            "Category_VOTE",
            "Button_OnOff_ONE SHOT ONE EXILE", //24
            "Button_OnOff_COUNTER POLITICS", //25

            "Category_MISC",
            "Button_OnOff_NO ADS", //27
            "Button_OnOff_NO LEAVE PENALTY" //28
    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you don't have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

extern "C" JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint feature, jint value, jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(str, 0);
    feature += 1;

    LOGD(OBFUSCATE("Feature name: [%d] %s | Value: = %d | Bool: = %d"), feature, featureName, value,
         boolean);

    switch(feature) {
        case 2: {
            PlayerControl::getLocalPlayer().getPlayerInfo()
            .set<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset::FieldOffset_IsDead>>(PlayerInfo::FieldOffset::FieldOffset_IsDead, false);
            break;
        }

        case 3: {
            PlayerControl::getLocalPlayer().getPlayerInfo()
            .set<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset::FieldOffset_IsDead>>(PlayerInfo::FieldOffset::FieldOffset_IsDead, true);
            break;
        }

        case 4: {
            //TODO(SUPER VISION);
            break;
        }

        case 5: {
            if(boolean)
                PlayerControl::EnableCounterImpostor();
            else
                PlayerControl::DisableCounterImpostor();
            break;
        }

        case 6: {
            if(boolean)
                ChatController::EnableCanSeeGhostsChat();
            else
                ChatController::DisableCanSeeGhostsChat();
            break;
        }

        case 7: {
            PlayerControl::getLocalPlayer().CmdCheckColor(value);
            usleep(300000);
            break;
        }

        case 9: {
            //TODO(BE IMPOSTOR, CALL SetInfected);
            PlayerControl::getLocalPlayer().getPlayerInfo()
            .set<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset_IsImpostor>>(PlayerInfo::FieldOffset_IsImpostor, true);
            PlayerControl::getLocalPlayer()
            .set<PlayerControl::DetermineFieldType<PlayerControl::FieldOffset_infectedSet>>(PlayerControl::FieldOffset_infectedSet, true);

            break;
        }

        case 10: {
            //TODO(CREWMATE CAN SEE IMPOSTOR)
            if(boolean) {
                ChatController::EnableCrewmateCanSeeImpostor();
                MeetingHud::EnableCrewmateCanSeeImpostor();
            }
            else {
                ChatController::DisableCrewmateCanSeeImpostor();
                MeetingHud::DisableCremwateCanSeeImpostor();
            }
            break;
        }

        case 12: {
            //TODO(BE CREWMATE, CALL RemoveInfected)
            PlayerControl::getLocalPlayer().getPlayerInfo()
                    .set<PlayerInfo::DetermineFieldType<PlayerInfo::FieldOffset_IsImpostor>>(PlayerInfo::FieldOffset_IsImpostor, false);
            PlayerControl::getLocalPlayer()
                    .set<PlayerControl::DetermineFieldType<PlayerControl::FieldOffset_infectedSet>>(PlayerControl::FieldOffset_infectedSet, false);
            break;
        }

        case 13: {
            if(boolean)
                KillButtonManager::EnableNoKillCooldown();
            else
                KillButtonManager::DisableNoKillCooldown();
            break;
        }

        case 14: {
            if(boolean)
                PlayerControl::EnableImpostorCanKillImpostor();
            else
                PlayerControl::DisableImpostorCanKillImpostor();
            break;
        }

        case 16: {
            if(boolean)
                PlayerControl::EnableCanMoveInVent();
            else
                PlayerControl::DisableCanMoveInVent();
            break;
        }

        case 17: {
            if(boolean)
                UnityEngine::EnableNoclip();
            else
                UnityEngine::DisableNoclip();
            break;
        }

        case 19: {
            if(boolean) {
                InfectedOverlay::EnableNoSabotageCooldown();
                MapRoom::EnableNoSabotageCooldown();
                SabotageSystemType::EnableNoSabotageCooldown();
            }
            else {
                InfectedOverlay::DisableNoSabotageCooldown();
                MapRoom::DisableNoSabotageCooldown();
                SabotageSystemType::DisableNoSabotageCooldown();
            }
            break;
        }

        case 20: {
            if(boolean)
                PlayerControl::EnableKillByOtherHand();
            else
                PlayerControl::DisableKillByOtherHand();
            break;
        }

        case 21: {
            PlayerControl::EmptyName();
            break;
        }

        case 22: {
            PlayerControl::ReportSomeone();
            break;
        }

        case 24: {
            if(boolean)
                MeetingHud::EnableOneShotOneExile();
            else
                MeetingHud::DisableOneShotOneExile();
            break;
        }

        case 25: {
            //TODO(COUNTER-POLITICS)
            break;
        }

        case 27: {
            if(boolean)
                SaveManager::EnableNoAds();
            else
                SaveManager::DisableNoAds();
            break;
        }

        case 28: {
            if(boolean)
                StatsManager::EnableNoLeavePenalty();
            else
                StatsManager::DisableNoLeavePenalty();
            break;
        }
    }
}

void *doHack(void *) {
    do {
        sleep(1);
    } while (!isLibraryLoaded(libName));

    LOGI(OBFUSCATE("Lib %s has been loaded"), libName);

    //For singleton initialization
    UnityEngine::getInstance().init();
    ChatController::getInstance().init();
    InfectedOverlay::getInstance().init();
    InnerNetClient::getInstance().init();
    KillButtonManager::getInstance().init();
    MapRoom::getInstance().init();
    MeetingHud::getInstance().init();
    NoShadowBehaviour::getInstance().init();
    SabotageSystemType::getInstance().init();
    SaveManager::getInstance().init();
    ShhhBehaviour::getInstance().init();
    StatsManager::getInstance().init();
    PlayerControl::getInstance().init();

    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, doHack, NULL);

    //Run anti-leech
    pthread_t p;
    pthread_create(&p, NULL, antiLeech, NULL);
}