#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "types.h"

class ThreadPool
{
public:
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);

    ThreadPool(unsigned size) :
        stop { false } {
        for(size_t ii = 0; ii < size; ++ii) {
            threads.emplace_back
            (
                [&]() {
                    for(;;) {
                        std::unique_lock<std::mutex> lck(mtx);
                        cv.wait(lck, [&]{ return stop || !tasks.empty(); });
                        if(stop && tasks.empty()) { return; }
                        auto task = std::move(tasks.front()); tasks.pop();
                        lck.unlock();
                        task();
                    }
                }
            );
        }
    }

    template <typename Func, typename ... Arg>
    auto execute(Func&& func, Arg&& ... arg) {
        using Ret = typename std::result_of<Func(Arg...)>::type;
        auto task = std::make_shared<std::packaged_task<Ret()>> (
                std::bind(std::forward<Func>(func), std::forward<Arg>(arg)...)
            );
        std::future<Ret> fut = task->get_future();
        std::unique_lock<std::mutex> lck(mtx);
        assert(!stop);
        tasks.emplace([task]() { (*task)(); });
        cv.notify_one();
        return fut;
    }

    ~ThreadPool() noexcept {
        stop = true;
        cv.notify_all();
        for(auto& thr : threads) {
            assert(thr.joinable());
            thr.join();
        }
    }

private:
    std::atomic<bool> stop;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
};

#endif // _THREADPOOL_H_
