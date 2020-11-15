//
//  KittyMemory.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include <src/Includes/obfuscate.h>
#include "KittyMemory.h"
#include <iostream>
#include <string>
#include <src/Includes/Logger.h>
#include <fstream>
#include <src/Includes/Utils.h>

using KittyMemory::Memory_Status;
using KittyMemory::ProcMap;


struct mapsCache {
    std::string identifier;
    ProcMap map;
};

static std::vector<ProcMap> _procMaps;
static std::vector<mapsCache> __mapsCache;
static ProcMap findMapInCache(std::string id){
    ProcMap ret;
    for(int i = 0; i < __mapsCache.size(); i++){
        if(__mapsCache[i].identifier.compare(id) == 0){
            ret = __mapsCache[i].map;
            break;
        }
    }
    return ret;
}


bool KittyMemory::ProtectAddr(void *addr, size_t length, int protection) {
   uintptr_t pageStart = _PAGE_START_OF_(addr);
   uintptr_t pageLen   = _PAGE_LEN_OF_(addr, length);
   return (
     mprotect(reinterpret_cast<void *>(pageStart), pageLen, protection) != -1
 );
}


Memory_Status KittyMemory::memWrite(void *addr, const void *buffer, size_t len) {
    if (addr == NULL)
        return INV_ADDR;

    if (buffer == NULL)
        return INV_BUF;

    if (len < 1 || len > INT_MAX)
        return INV_LEN;

    if (!ProtectAddr(addr, len, _PROT_RWX_))
        return INV_PROT;

    if (memcpy(addr, buffer, len) != NULL && ProtectAddr(addr, len, _PROT_RX_))
        return SUCCESS;

    return FAILED;
}


Memory_Status KittyMemory::memRead(void *buffer, const void *addr, size_t len) {
    if (addr == NULL)
        return INV_ADDR;

    if (buffer == NULL)
        return INV_BUF;

    if (len < 1 || len > INT_MAX)
        return INV_LEN;

    if (memcpy(buffer, addr, len) != NULL)
        return SUCCESS;

    return FAILED;
}

std::vector<uintptr_t>
scanMem(uintptr_t startAddr, uintptr_t endAddr, unsigned char *val, size_t len) {
    std::vector<uintptr_t> result;
    unsigned __int64 targetAddr = startAddr;
    unsigned char readBuffer[4096] = {0,};
    size_t readLen = sizeof(readBuffer);

    LOGI(stringFormat("scanMem: Start: %p, End: %p, Length: %p", startAddr, endAddr,
                      endAddr - startAddr).c_str());


    while (true) {
        if (targetAddr + readLen > endAddr)
            readLen = endAddr - targetAddr;

        if (KittyMemory::memRead(readBuffer, (const void *) targetAddr, readLen) ==
            KittyMemory::SUCCESS) {
            for (int i = 0; i < readLen; ++i) {
                if (readBuffer[i] != val[0])
                    continue;

                if (readLen - i <= len)
                    break;

                if (memcmp(readBuffer + i, val, len) == 0) {
                    result.push_back(targetAddr + i);
                    i += len;
                }
            }
        }

        std::memset(readBuffer, 0, sizeof(readBuffer));
        targetAddr += readLen;

        if (targetAddr >= endAddr)
            break;
        usleep(10);
    }

    LOGI("scanMem: end");
    return result;
}

std::string KittyMemory::read2HexStr(const void *addr, size_t len) {
    char temp[len];
    memset(temp, 0, len);

    const size_t bufferLen = len * 2 + 1;
    char buffer[bufferLen];
    memset(buffer, 0, bufferLen);

    std::string ret;

    if (memRead(temp, addr, len) != SUCCESS)
        return ret;

    for (int i = 0; i < len; i++) {
        sprintf(&buffer[i * 2], "%02X", (unsigned char) temp[i]);
    }

    ret += buffer;
    return ret;
}

std::vector<ProcMap> KittyMemory::getMaps() {
    char line[512] = { 0, };
    FILE *fp = fopen("/proc/self/maps", "rt");

    if(fp != nullptr) {
        _procMaps.clear();

        while(fgets(line, sizeof(line), fp)) {
            ProcMap tmpMap;
            char tmpPerms[5] = {0}, tmpDev[12] = {0}, tmpPathname[444] = {0};

            sscanf(line, "%llx-%llx %s %ld %s %d %s",
                   (long long unsigned *) &tmpMap.startAddr,
                   (long long unsigned *) &tmpMap.endAddr,
                   tmpPerms, &tmpMap.offset, tmpDev, &tmpMap.inode, tmpPathname);

            tmpMap.length = (uintptr_t) tmpMap.endAddr - (uintptr_t) tmpMap.startAddr;
            tmpMap.perms = tmpPerms;
            tmpMap.dev = tmpDev;
            tmpMap.pathname = tmpPathname;

            _procMaps.push_back(tmpMap);
        }

        fclose(fp);
    }

    return _procMaps;
}

std::vector<ProcMap> KittyMemory::getMaps(const char *name) {
    std::vector<ProcMap> maps;
    char line[512] = { 0, };
    FILE *fp = fopen("/proc/self/maps", "rt");

    if(fp != nullptr) {
        _procMaps.clear();

        while(fgets(line, sizeof(line), fp)) {
            if(strstr(line, name)) {
                ProcMap tmpMap;
                char tmpPerms[5] = {0}, tmpDev[12] = {0}, tmpPathname[444] = {0};

                sscanf(line, "%llx-%llx %s %ld %s %d %s",
                       (long long unsigned *) &tmpMap.startAddr,
                       (long long unsigned *) &tmpMap.endAddr,
                       tmpPerms, &tmpMap.offset, tmpDev, &tmpMap.inode, tmpPathname);

                tmpMap.length = (uintptr_t) tmpMap.endAddr - (uintptr_t) tmpMap.startAddr;
                tmpMap.perms = tmpPerms;
                tmpMap.dev = tmpDev;
                tmpMap.pathname = tmpPathname;

                maps.push_back(tmpMap);
            }
        }

        fclose(fp);
    }

    return maps;
}

ProcMap KittyMemory::getRegionMap(uintptr_t adr) {
    ProcMap map = { 0, };

    for(ProcMap map: KittyMemory::getMaps()) {
        if(reinterpret_cast<uintptr_t>(map.startAddr) <= adr && reinterpret_cast<uintptr_t>(map.endAddr) >= adr)
            return map;
    }

    return map;
}

ProcMap KittyMemory::getLibraryMap(const char *libraryName) {
    ProcMap retMap;
    char line[512] = {0};

    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, libraryName)) {
                char tmpPerms[5] = {0}, tmpDev[12] = {0}, tmpPathname[444] = {0};
                // parse a line in maps file
                // (format) startAddress-endAddress perms offset dev inode pathname
                sscanf(line, "%llx-%llx %s %ld %s %d %s",
                       (long long unsigned *) &retMap.startAddr,
                       (long long unsigned *) &retMap.endAddr,
                       tmpPerms, &retMap.offset, tmpDev, &retMap.inode, tmpPathname);

                retMap.length = (uintptr_t) retMap.endAddr - (uintptr_t) retMap.startAddr;
                retMap.perms = tmpPerms;
                retMap.dev = tmpDev;
                retMap.pathname = tmpPathname;

                break;
            }
        }
        fclose(fp);
    }
    return retMap;
}

uintptr_t KittyMemory::getAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr, bool useCache) {
    ProcMap libMap;

    if(useCache){
        libMap = findMapInCache(libraryName);
        if(libMap.isValid())
        return (reinterpret_cast<uintptr_t>(libMap.startAddr) + relativeAddr);
    }

    libMap = getLibraryMap(libraryName);
    if (!libMap.isValid())
        return 0;

    if(useCache){
        mapsCache cachedMap;
        cachedMap.identifier = libraryName;
        cachedMap.map        = libMap;
        __mapsCache.push_back(cachedMap);
    }

    return (reinterpret_cast<uintptr_t>(libMap.startAddr) + relativeAddr);
}
