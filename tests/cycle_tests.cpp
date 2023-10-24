#include "./lib/DynamicBuffer/DynamicBuffer.hpp"
#include "./lib/StaticBuffer/StaticBuffer.hpp"
#include <gtest/gtest.h>

TEST(DynamicBufferTests, ConstructorTest1){
    DynamicBuffer<int> buf(3, 5);
    ASSERT_TRUE(buf.size() == 3);
}

TEST(DynamicBufferTests, ConstructorTest2){
    DynamicBuffer<int> buf(3, 5);
    ASSERT_TRUE(buf[1] == 5 && buf[0] == 5 && buf[2] == 5);
}

TEST(DynamicBufferTests, ConstructorTest3){
    DynamicBuffer<int> buf;
    ASSERT_TRUE(buf.empty());
}

TEST(DynamicBufferTests, ResizeTest){
    DynamicBuffer<int> buf(3, 5);
    buf.resize(5);
    ASSERT_TRUE(buf.size() == 5);
}

TEST(DynamicBufferTests, ReserveTest){
    DynamicBuffer<int> buf(3, 5);
    buf.reserve(5);
    ASSERT_TRUE(buf.capacity() == 5);
}


TEST(DynamicBufferTests, ClearTest){
    DynamicBuffer<int> buf(3, 5);
    buf.clear();
    ASSERT_TRUE(buf.empty());
}

TEST(DynamicBufferTests, PushPopTest){
    DynamicBuffer<int> buf(5);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
        buf.pop_front();
        buf.push_front(i);
        buf.pop_back();
    }
    ASSERT_TRUE(buf.empty());
}

TEST(DynamicBufferTests, ReallocateTest){
    DynamicBuffer<int> buf;
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
        buf.push_front(i);
    }
    ASSERT_TRUE(buf.size() == 10);
}

TEST(DynamicBufferTests, IteratorTest1){
    DynamicBuffer<int> buf;
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 2 3 4 ");
}

TEST(DynamicBufferTests, CirculatedTest){
    DynamicBuffer<int> buf(3, 5);

    ASSERT_TRUE(buf.begin() == buf.begin() + 12);
}

TEST(DynamicBufferTests, EraseTest0){
    DynamicBuffer<int> buf(10, 5);
    buf.erase(buf.begin() + 5);

    ASSERT_TRUE(buf.size() == 9);
}

TEST(DynamicBufferTests, EraseTest1){
    DynamicBuffer<int> buf(10, 5);
    buf.erase(buf.begin() + 5, buf.begin() + 8);

    ASSERT_TRUE(buf.size() == 7);
}

TEST(DynamicBufferTests, AssignTest0){
    DynamicBuffer<int> buf;
    std::vector<int> a{1, 2, 3};
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    buf.assign(a.begin(), a.end());
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "1 2 3 ");
}

TEST(DynamicBufferTests, AssignTest1){
    DynamicBuffer<int> buf;
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    buf.assign({1, 2, 3});
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "1 2 3 ");
}

TEST(DynamicBufferTests, AssignTest2){
    DynamicBuffer<int> buf;
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    size_t a = 5;
    buf.assign(a, 10);
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "10 10 10 10 10 ");
}

TEST(DynamicBufferTests, InsertTest0) {
    DynamicBuffer<int> buf;
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, 10);
    ASSERT_TRUE(*(buf.begin() + 2) == 10);
}

TEST(DynamicBufferTests, InsertTest1) {
    DynamicBuffer<int> buf;
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, {1,2,3});
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 1 2 3 2 3 4 ");
}

TEST(DynamicBufferTests, InsertTest2) {
    DynamicBuffer<int> buf;
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, size_t(3), 7);
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 7 7 7 2 3 4 ");
}

TEST(DynamicBufferTests, SortTest) {
    DynamicBuffer<int> buf;
    for (int i = 0; i < 3; i++) {
        buf.push_back(i);
        buf.push_back(10 - i);
    }
    std::sort(buf.begin(), buf.end());
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 2 8 9 10 ");
}

TEST(StaticBufferTests, ConstructorTest1){
    StaticBuffer<int> buf(3, 5);
    ASSERT_TRUE(buf.size() == 3);
}

TEST(StaticBufferTests, ConstructorTest2){
    StaticBuffer<int> buf(3, 5);
    ASSERT_TRUE(buf[1] == 5 && buf[0] == 5 && buf[2] == 5);
}

TEST(StaticBufferTests, ConstructorTest3){
    StaticBuffer<int> buf(20);
    ASSERT_TRUE(buf.empty());
}


TEST(StaticBufferTests, ClearTest){
    StaticBuffer<int> buf(3, 5);
    buf.clear();
    ASSERT_TRUE(buf.empty());
}

TEST(StaticBufferTests, PushPopTest){
    StaticBuffer<int> buf(20);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
        buf.pop_front();
        buf.push_front(i);
        buf.pop_back();
    }
    ASSERT_TRUE(buf.empty());
}

TEST(StaticBufferTests, ReallocateTest){
    StaticBuffer<int> buf(20);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
        buf.push_front(i);
    }
    ASSERT_TRUE(buf.size() == 10);
}

TEST(StaticBufferTests, IteratorTest1){
    StaticBuffer<int> buf(20);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 2 3 4 ");
}

TEST(StaticBufferTests, CirculatedTest){
    StaticBuffer<int> buf(3, 5);

    ASSERT_TRUE(buf.begin() == buf.begin() + 12);
}

TEST(StaticBufferTests, EraseTest0){
    StaticBuffer<int> buf(10, 5);
    buf.erase(buf.begin() + 5);

    ASSERT_TRUE(buf.size() == 9);
}

TEST(StaticBufferTests, EraseTest1){
    StaticBuffer<int> buf(10, 5);
    buf.erase(buf.begin() + 5, buf.begin() + 8);

    ASSERT_TRUE(buf.size() == 7);
}

TEST(StaticBufferTests, AssignTest0){
    StaticBuffer<int> buf(20);
    std::vector<int> a{1, 2, 3};
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    buf.assign(a.begin(), a.end());
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "1 2 3 ");
}

TEST(StaticBufferTests, AssignTest1){
    StaticBuffer<int> buf(20);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    buf.assign({1, 2, 3});
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "1 2 3 ");
}

TEST(StaticBufferTests, AssignTest2){
    StaticBuffer<int> buf(20);
    for(int i = 0; i < 5; i++){
        buf.push_back(i);
    }
    size_t a = 5;
    buf.assign(a, 10);
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "10 10 10 10 10 ");
}

TEST(StaticBufferTests, InsertTest0) {
    StaticBuffer<int> buf(20);
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, 10);
    ASSERT_TRUE(*(buf.begin() + 2) == 10);
}

TEST(StaticBufferTests, InsertTest1) {
    StaticBuffer<int> buf(20);
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, {1,2,3});
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 1 2 3 2 3 4 ");
}

TEST(StaticBufferTests, InsertTest2) {
    StaticBuffer<int> buf(20);
    for (int i = 0; i < 5; i++) {
        buf.push_back(i);
    }
    buf.insert(buf.begin() + 2, size_t(3), 7);
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 7 7 7 2 3 4 ");
}

TEST(StaticBufferTests, SortTest) {
    StaticBuffer<int> buf(20);
    for (int i = 0; i < 3; i++) {
        buf.push_back(i);
        buf.push_back(10 - i);
    }
    std::sort(buf.begin(), buf.end());
    std::string ans;
    for(auto i : buf){
        ans += std::to_string(i) + " ";
    }
    ASSERT_TRUE(ans == "0 1 2 8 9 10 ");
}





