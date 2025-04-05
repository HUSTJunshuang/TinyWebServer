/*
 * Copyright (c) 2025 HUSTJunshuang
 * 
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.
 * 
 * Portions of this file are derived from [markparticle/WebServer](https://github.com/markparticle/WebServer),
 * which is licensed under the Apache License, Version 2.0.
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <queue>
// for ITC(Inter-Thread Communication)
#include <mutex>
#include <condition_variable>
// function package
#include <functional>
#include <future>
// for smart pointer
#include <memory>
// for exception
#include <stdexcept>

class ThreadPool {
private:
    struct TaskQue {
        std::mutex mtx;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> tasks;
    };
    std::shared_ptr<TaskQue> taskque_;
public:
    // TODO why not delete
    ThreadPool() = default;
    explicit ThreadPool(size_t threadCount = 8);
    ThreadPool(ThreadPool &&) = default;
    ~ThreadPool();
    // add task to taskque_
    template<class F, class... Args>
    auto enqueue(F &&task, Args &&...args)
    #if __cplusplus >= 201703L
        -> std::future<typename std::invoke_result<F, Args...>::type>;
    #else
        -> std::future<typename std::result_of<F(Args...)>::type>;
    #endif
};

inline ThreadPool::ThreadPool(size_t threadCount) : taskque_(std::make_shared<TaskQue>()) {
    // assert(threadCount > 0);
    for (size_t i = 0; i < threadCount; ++i) {
        std::thread([taskque = taskque_] {
            std::unique_lock<std::mutex> locker(taskque->mtx);  // lock, wakeup while get lock
            while(true) {
                // if taskque is closed, child thread exit
                if (taskque->isClosed)  break;
                if (!taskque->tasks.empty()) {
                    // has available task
                    auto task = std::move(taskque->tasks.front());
                    taskque->tasks.pop();
                    locker.unlock();                            // unlock and go to work
                    task();
                    locker.lock();                              // lock, wakeup while get lock
                }
                else    taskque->cond.wait(locker);             // unlock until notified, only one thread can lock
            }
        }).detach();
    }
}

inline ThreadPool::~ThreadPool() {
    if (static_cast<bool>(taskque_)) {
        {
            std::lock_guard<std::mutex> locker(taskque_->mtx);  // lock, wakeup while get lock(all tasks finished)
            taskque_->isClosed = true;
        }
        taskque_->cond.notify_all();
    }
}

template<class F, class... Args>
auto ThreadPool::enqueue(F &&task, Args &&...args)
#if __cplusplus >= 201703L
    -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;
#else
    -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
#endif
    // pack the task function
    auto packed_task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(task), std::forward<Args>(args)...)
    );
    std::future<return_type> res = packed_task->get_future();
    // critical section
    {
        std::lock_guard<std::mutex> locker(taskque_->mtx);
        // can't enqueue after the close of threadpool
        if (taskque_->isClosed) {
            throw std::runtime_error("enqueue on closed ThreadPool");
        }
        taskque_->tasks.emplace([packed_task](){ (*packed_task)(); });
    }
    taskque_->cond.notify_one();
    return res;
}

#endif
