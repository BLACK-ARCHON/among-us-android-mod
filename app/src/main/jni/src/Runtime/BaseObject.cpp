#include <unistd.h>
#include "BaseObject.hpp"

BaseObject::BaseObject(uintptr_t pointer) {
    this->_pointer = pointer;
}

BaseObject::~BaseObject() = default;

uintptr_t BaseObject::getPointer() {
    return this->_pointer;
}

void BaseObject::setPointer(uintptr_t pointer) {
    this->_pointer = pointer;
}
