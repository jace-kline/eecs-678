#include "Maybe.h"

template <typename T>
Maybe<T>::Maybe() : nothing(true) {}

template <typename T>
Maybe<T>::Maybe(const T& obj) : nothing(false) {
    item = obj;
}

template <typename T>
Maybe<T>::~Maybe() {}

template <typename T>
bool Maybe<T>::isNothing() const {return nothing;}

template <typename T>
T Maybe<T>::getItem() const {return item;}