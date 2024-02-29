#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>
#include <limits>
#undef min
#undef max
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <typeindex>

template <typename T_ret, typename... T_args>
struct Proc : public std::function<T_ret(T_args...)> {

    using _function = std::function<T_ret(T_args...)>;
    using _function::function;

    template <typename T2_ret, typename... T2_args>
    Proc(std::function<T2_ret(T2_args...)> fnc) :
        _function(
            [fnc](T_args... args) -> T_ret {
                return (T_ret)fnc((T2_args)args...);
            }
        ) {}

    template <typename T2_ret, typename... T2_args>
    Proc(std::function<T2_ret(T2_args...)>* fnc) : 
        _function(
            [fnc](T_args... args) -> T_ret {
                return (T_ret)(*fnc)((T2_args)args...);
            }
        ) {}

};

/*
En listener tar in en funktion för varje unikt event, och läggs till i någon map för window messages.
Om en funktion tar in ett mer generellt event sätts den helt enkelt som funktion för flera unika events.
Sen har man någon "wrapper" lambda som kör en function pointer (om ej nullptr) som kan ändras. Wow wow wow. 
Tänker då att endast listeners kan "tas bort," snarare än "event handlers" som lika gärna kan sättas till nullptr 
om de inte ska köras.
*/

int main() {

    Proc<void> p = nullptr;
    p();

    return 0;
}