#ifndef TGCPP_H
#define TGCPP_H

#include <cstddef>
#include <memory>
#include <new>

class tgc {
  public:
    tgc() noexcept {};
    virtual ~tgc() noexcept {};
    virtual void run() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void *alloc(size_t size) = 0;
    virtual void *calloc(size_t num, size_t size) = 0;
    virtual void *realloc(void *ptr, size_t size) = 0;
    virtual void free(void *ptr) = 0;
    virtual void set_dtor(void *ptr, void (*dtor)(void *)) = 0;

    static tgc *global;
    static void init(void *stk);
    static void gc();
    static void stop();
    static std::unique_ptr<tgc> create(void *stk) noexcept;

    tgc &operator=(const tgc &) = delete;
    tgc &operator=(const tgc &&) = delete;
};

template <typename Tp, typename... Targs> Tp *tgc_new(Targs... args) {
    void *mem = tgc::global->alloc(sizeof(Tp));
    void *ptr = new (mem) Tp(args...);
    tgc::global->set_dtor(ptr, [](void *self) noexcept { static_cast<Tp *>(self)->~Tp(); });
    return static_cast<Tp *>(ptr);
}

#endif