#ifndef TGCPP_H
#define TGCPP_H

#include <cstddef>
#include <new>

namespace tgc {

void start(void *stk);
void stop();

void run();

void pause();
void resume();

void *alloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

void set_dtor(void *ptr, void (*dtor)(void *));

} // namespace tgc

template <typename Tp, typename... Targs> Tp *tgc_new(Targs... args) {
    void *mem = tgc::alloc(sizeof(Tp));
    void *ptr = new (mem) Tp(args...);
    tgc::set_dtor(ptr, [](void *self) noexcept { static_cast<Tp *>(self)->~Tp(); });
    return static_cast<Tp *>(ptr);
}

#endif