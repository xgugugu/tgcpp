#include "tgcpp.h"

#include <stdexcept>

extern "C"
{
#include "tgc/tgc.h"
}

namespace tgc_private {
class gc final : public tgc {
  public:
    gc(void *stk) noexcept;
    ~gc() noexcept override;
    void run() override;
    void pause() override;
    void resume() override;
    void *alloc(size_t size) override;
    void *calloc(size_t num, size_t size) override;
    void *realloc(void *ptr, size_t size) override;
    void free(void *ptr) override;
    void set_dtor(void *ptr, void (*dtor)(void *)) override;

  private:
    tgc_t tgc;
};

gc::gc(void *stk) noexcept {
    tgc_start(&tgc, stk);
}
gc::~gc() noexcept {
    tgc_stop(&tgc);
}
void gc::run() {
    tgc_run(&tgc);
}
void gc::pause() {
    tgc_pause(&tgc);
}
void gc::resume() {
    tgc_resume(&tgc);
}
void *gc::alloc(size_t size) {
    return tgc_alloc(&tgc, size);
}
void *gc::calloc(size_t num, size_t size) {
    return tgc_calloc(&tgc, num, size);
}
void *gc::realloc(void *ptr, size_t size) {
    return tgc_realloc(&tgc, ptr, size);
}
void gc::free(void *ptr) {
    tgc_free(&tgc, ptr);
}
void gc::set_dtor(void *ptr, void (*dtor)(void *)) {
    tgc_set_dtor(&tgc, ptr, dtor);
}
} // namespace tgc_private

tgc *tgc::global = nullptr;
void tgc::init(void *stk) {
    if (global != nullptr) {
        throw std::runtime_error("In tgc::init(), tgc::global != nullptr");
    }
    global = new tgc_private::gc(stk);
}
void tgc::gc() {
    if (global == nullptr) {
        throw std::runtime_error("In tgc::gc(), tgc::global == nullptr");
    }
    global->run();
}
void tgc::stop() {
    if (global == nullptr) {
        throw std::runtime_error("In tgc::stop(), tgc::global == nullptr");
    }
    delete global;
    global = nullptr;
}
std::unique_ptr<tgc> tgc::create(void *stk) noexcept {
    return std::unique_ptr<tgc_private::gc>(new tgc_private::gc(stk));
}