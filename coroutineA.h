#pragma once

#include <coroutine>
#include <memory>

class CoroutineA{
public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    void process(int value)
    {
        *coro.promise().value = value;
        coro.resume();
    }
    struct promise_type{
        std::shared_ptr<int> value;
        promise_type(std::shared_ptr<int> val) : value(val) {}
        void unhandled_exception() { std::terminate(); }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_void() {}
        auto get_return_object()
        {
            return CoroutineA{handle::from_promise(*this)};
        }
    };
    CoroutineA(CoroutineA const&) = delete;
    CoroutineA& operator=(CoroutineA const&) = delete;
    CoroutineA(CoroutineA&& other) : coro(other.coro) 
    { 
        other.coro = nullptr; 
    }
    ~CoroutineA()
    {
        if (coro)
            coro.destroy();
    }

private:
    CoroutineA(handle h) : coro(h) {}
    handle coro;
};