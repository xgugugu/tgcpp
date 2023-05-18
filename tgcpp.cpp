#include "tgcpp.h"

extern "C"
{
#include "tgc/tgc.h"
}

namespace tgc_private {
tgc_t gc;
}

namespace tgc {
using namespace tgc_private;

void start(void *stk) {
    tgc_start(&gc, stk);
}
void stop() {
    tgc_stop(&gc);
}

void run() {
    tgc_run(&gc);
}

void pause() {
    tgc_pause(&gc);
}
void resume() {
    tgc_resume(&gc);
}

void *alloc(size_t size) {
    return tgc_alloc(&gc, size);
}
void *calloc(size_t num, size_t size) {
    return tgc_calloc(&gc, num, size);
}
void *realloc(void *ptr, size_t size) {
    return tgc_realloc(&gc, ptr, size);
}
void free(void *ptr) {
    tgc_free(&gc, ptr);
}

void set_dtor(void *ptr, void (*dtor)(void *)) {
    tgc_set_dtor(&gc, ptr, dtor);
}

} // namespace tgc
