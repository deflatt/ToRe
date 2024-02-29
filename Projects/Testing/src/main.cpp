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
En listener tar in en funktion f�r varje unikt event, och l�ggs till i n�gon map f�r window messages.
Om en funktion tar in ett mer generellt event s�tts den helt enkelt som funktion f�r flera unika events.
Sen har man n�gon "wrapper" lambda som k�r en function pointer (om ej nullptr) som kan �ndras. Wow wow wow. 
T�nker d� att endast listeners kan "tas bort," snarare �n "event handlers" som lika g�rna kan s�ttas till nullptr 
om de inte ska k�ras.
*/

int main() {

    Proc<void> p = nullptr;
    p();

    return 0;
}