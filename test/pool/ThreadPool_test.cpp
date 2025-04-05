/*
 * Copyright (c) 2025 HUSTJunshuang
 * 
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.
 */

#include <pool/threadpool.h>
#include <gtest/gtest.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <random>

// run single thread
TEST(ThreadPoolTest, SingleTask) {
    ThreadPool pool(4);
    auto result = pool.enqueue([]() { return 1; });
    EXPECT_EQ(result.get(), 1);
}

// run multiple threads
TEST(ThreadPoolTest, MultiTasks) {
    ThreadPool pool(4);
    auto result1 = pool.enqueue([]() { return 1; });
    auto result2 = pool.enqueue([]() { return 2; });
    auto result3 = pool.enqueue([]() { return 3; });
    auto result4 = pool.enqueue([]() { return 4; });
    EXPECT_EQ(result1.get(), 1);
    EXPECT_EQ(result2.get(), 2);
    EXPECT_EQ(result3.get(), 3);
    EXPECT_EQ(result4.get(), 4);
}

// run multiple threads, Out of Order
TEST(ThreadPoolTest, OoOMultiTasks) {
    ThreadPool pool(4);
    std::srand(std::time(nullptr));
    auto result1 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
        return 1;
    });
    auto result2 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
        return 2;
    });
    auto result3 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
        return 3;
    });
    auto result4 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
        return 4;
    });
    EXPECT_EQ(result1.get(), 1);
    EXPECT_EQ(result2.get(), 2);
    EXPECT_EQ(result3.get(), 3);
    EXPECT_EQ(result4.get(), 4);
}

// run massive threads, Out of Order
TEST(ThreadPoolTest, OoOMassiveTasks) {
    ThreadPool pool(8);
    std::srand(time(nullptr));
    std::vector<std::future<int>> results;

    for (int i = 0; i < 100; ++i) {
        results.emplace_back(pool.enqueue([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
            return i;
        }));
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(results[i].get(), i);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}