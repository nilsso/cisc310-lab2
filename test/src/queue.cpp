#include "gtest/gtest.h"
#include "queue.hpp"

namespace QueueTests {
    namespace EmptyTests {
        TEST(QueueTest, EmptyTest1) {
            queue<uint8_t> q;
            ASSERT_TRUE(q.empty());
        }

        TEST(QueueTest, EmptyTest2) {
            queue<uint8_t> q = { 1 };
            ASSERT_TRUE(!q.empty());
        }

        TEST(QueueTest, EmptyTest3) {
            queue<uint8_t> q = { 1 };
            q.dequeue();
            ASSERT_TRUE(q.empty());
        }
    }

    namespace SizeTests {
        TEST(QueueTest, SizeTest1) {
            queue<uint8_t> q;
            ASSERT_EQ(q.size(), 0);
        }

        TEST(QueueTest, SizeTest2) {
            queue<uint8_t> q = { 1, 2, 3, 4, 5 };
            ASSERT_EQ(q.size(), 5);
        }

        TEST(QueueTest, SizeTest3) {
            queue<uint8_t> q = { 1, 2, 3, 4, 5 };
            q.dequeue();
            ASSERT_EQ(q.size(), 4);
        }
    }

    namespace TakeNextTests {
        TEST(QueueTest, TakeNextTest1) {
            queue<uint8_t> q = { 1 };
            ASSERT_EQ(q.dequeue(), 1);
        }

        //TEST(QueueTest, TakeNextTest1) {
            //queue<uint8_t> q = { 1 };
            //ASSERT_EQ(q.dequeue(), 1);
        //}
    }
}
