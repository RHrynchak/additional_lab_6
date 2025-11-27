#pragma once

#include "coroutineA.h"

class Generator{
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    int operator()()
    {
        coro.resume();
        return coro.promise().value;
    }
    class Awaiter;

    struct promise_type{
        int value;
        CoroutineA* coro_A;
        promise_type(CoroutineA* coro) : coro_A(coro) {}
        void unhandled_exception() { std::terminate(); }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_void() {}
        auto get_return_object()
        {
            return Generator{handle::from_promise(*this)};
        }
        auto yield_value(int val)
        {
            value = val;
            return std::suspend_always{};
        }
        Awaiter await_transform(int value) 
        { 
            this->value = value;
            return {*this}; 
        }
    };

    class Awaiter{
    private:
        promise_type& p;
    public:
        Awaiter(promise_type &p) : p(p) {}
        bool await_ready() const { 
            return (p.value % 2 != 0);
        }
        bool await_suspend(std::coroutine_handle<> h) noexcept
        {
            if ( p.coro_A ) {
                p.coro_A->process(p.value);
            } 
            return false;
        }
        void await_resume() noexcept {}
    };
    
    Generator(Generator const&) = delete;
    Generator& operator=(Generator const&) = delete;
    Generator(Generator&& other) : coro(other.coro) 
    { 
        other.coro = nullptr; 
    }
    
    ~Generator()
    {
        if (coro)
            coro.destroy();
    }

private:
    Generator(handle h) : coro(h) {}
    handle coro;
};