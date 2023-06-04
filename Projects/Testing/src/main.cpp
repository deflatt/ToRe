#include <functional>
#include <iostream>

template <typename... Ts>
struct Foo {
    std::function<void(Ts...)> fnc = {};
    Foo() {}
    Foo(const Foo&) : fnc(
        [](Ts... ts) {
            bool test = ([&ts]() { return true; }() && ...);
            std::cout << test << std::endl;
        }
    ) {}
};
int main() {
    Foo<int> f;
    Foo<int> f2 = f;
    return 0;
}