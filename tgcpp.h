/* tgcpp - a C++ wrapper for tiny garbage collector
 *
 * Copyright (C) 2023 xgugugu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TGCPP_H
#define TGCPP_H

#include <cstddef>
#include <memory>
#include <new>

class tgc {
  public:
    tgc() noexcept {};
    virtual ~tgc() noexcept {};
    /*
     * Run an iteration of the garbage collector, freeing any unreachable memory.
     */
    virtual void run() = 0;
    /*
     * Pause the garbage collector.
     * While paused the garbage collector will not run during any allocations made.
     */
    virtual void pause() = 0;
    /*
     * Resume the garbage collector.
     */
    virtual void resume() = 0;
    /*
     * Allocate memory via the garbage collector to be automatically freed once it becomes unreachable.
     */
    virtual void *alloc(size_t size) = 0;
    /*
     * Allocate memory via the garbage collector and initalise it to zero.
     */
    virtual void *calloc(size_t num, size_t size) = 0;
    /*
     * Reallocate memory allocated by the garbage collector.
     */
    virtual void *realloc(void *ptr, size_t size) = 0;
    /*
     * Manually free an allocation made by the garbage collector.
     * Runs any destructor if registered.
     */
    virtual void free(void *ptr) = 0;
    /*
     * Register a destructor function to be called after the memory allocation ptr becomes unreachable,
     * and just before it is freed by the garbage collector.
     */
    virtual void set_dtor(void *ptr, void (*dtor)(void *)) = 0;

    /*
     * Global garbage collector.
     */
    static tgc *global;
    /*
     * Initialize and start the global garbage collector on the current thread,
     * beginning at the stack location given by the stk variable.
     * Usually this can be found using the address of any local variable,
     * and then the garbage collector will cover all memory at least one function call deeper.
     * @throws std::runtime_error
     */
    static void init(void *stk);
    /*
     * Run an iteration of the global garbage collector, freeing any unreachable memory.
     * @throws std::runtime_error
     */
    static void gc();
    /*
     * Stop the global garbage collector and free its internal memory.
     * @throws std::runtime_error
     */
    static void stop();
    /*
     * Create and start a garbage collector on the current thread,
     * beginning at the stack location given by the stk variable.
     * Usually this can be found using the address of any local variable,
     * and then the garbage collector will cover all memory at least one function call deeper.
     * @return std::unique_ptr<tgc>
     */
    static std::unique_ptr<tgc> create(void *stk) noexcept;

    tgc &operator=(const tgc &) = delete;
    tgc &operator=(const tgc &&) = delete;
};

/*
 * Creates and initializes objects via the garbage collector to be automatically freed once it becomes unreachable.
 * @throws std::bad_alloc
 */
template <typename Tp, typename... Targs> Tp *tgc_new(Targs... args) {
    void *mem = tgc::global->alloc(sizeof(Tp));
    if (mem == nullptr) {
        throw std::bad_alloc();
    }
    void *ptr = new (mem) Tp(args...);
    tgc::global->set_dtor(ptr, [](void *self) noexcept { static_cast<Tp *>(self)->~Tp(); });
    return static_cast<Tp *>(ptr);
}

#endif