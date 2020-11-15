#pragma once

#include <src/Includes/Utils.h>

class BaseObject {
private:
    uintptr_t _pointer = NULL;

public:
    BaseObject(uintptr_t pointer);
    ~BaseObject();

    virtual bool isInitialized() = 0;
    virtual bool isUsable() = 0;
    virtual void init() = 0;

    uintptr_t getPointer();
    void setPointer(uintptr_t pointer);

    template<typename T>
    T get(uintptr_t offset) {
        if(isInitialized() && getPointer() != NULL)
            return *(T*)(this->_pointer + offset);
        return T(NULL);
    }

    template<typename T>
    bool set(uintptr_t offset, T value) {
        if(isInitialized() && getPointer() != NULL) {
            *(T*)(this->_pointer + offset) = value;
            return true;
        }
        return false;
    }
};