tgcpp
=====

C++ Wrapper for [Tiny Garbage Collector](https://github.com/orangeduck/tgc)

**Usage**

```cpp
#include "tgcpp.h"
#include <cstdio>

class A {
  public:
    virtual ~A() noexcept {
        printf("A dtor\n");
    }
};
class B : public A {
  public:
    virtual ~B() noexcept override {
        printf("B dtor\n");
    }
};

int main(int argc, char *argv[]) {
    tgc::init(&argc);
    try {
        A *ptr0 = tgc_new<A>();
        A *ptr1 = tgc_new<B>();
        throw "exception";
        A *ptr2 = tgc_new<A>();
    } catch (...) {
        printf("exception!\n");
    }
    tgc::stop();
    return 0;
}
```
