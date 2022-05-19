[![Coverage Status](https://coveralls.io/repos/github/Yaros09/lab-05/badge.svg)](https://coveralls.io/github/Yaros09/lab-05)

Задание 1
CMakeLists.txt для banking:
```
cmake_minimum_required(VERSION 3.4)

project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(banking STATIC Account.cpp  Account.h Transaction.cpp Transaction.h)
```
![Снимок экрана от 2022-05-14 14-17-24](https://user-images.githubusercontent.com/91633974/169373680-f42e48dd-a886-4661-9af7-dadb91bc437b.png)

Testing.cpp
```
#include<iostream>
#include<Account.h>
#include<Transaction.h>
#include<gtest/gtest.h>
#include<gmock/gmock.h>

struct MockAccount : public Account {
  MockAccount(int id, int balance) : Account(id, balance) {}
  MOCK_METHOD1(ChangeBalance, void(int diff));
  MOCK_METHOD0(GetBalance, int());
  MOCK_METHOD0(id, int());
  MOCK_METHOD0(Lock, void());
  MOCK_METHOD0(Unlock, void());
};
struct MockTransaction : public Transaction {
  MOCK_METHOD3(Make, bool(Account& from, Account& to, int sum));
  MOCK_METHOD1(set_fee, void(int fee));
  MOCK_METHOD0(fee, int());
 };

TEST(Account, Testmockac) {
  MockAccount ob(2, 150000);
  EXPECT_CALL(ob, ChangeBalance(testing::_ )).Times(2);
  EXPECT_CALL(ob, GetBalance()).Times(3);
  EXPECT_CALL(ob, id()).Times(1);
  EXPECT_CALL(ob, Lock()).Times(1);
  EXPECT_CALL(ob, Unlock()).Times(1);
  ob.Unlock();
  ob.id();
  ob.GetBalance();
  ob.ChangeBalance(-60000);
  ob.GetBalance();
  ob.ChangeBalance(-90000);
  ob.GetBalance();
  ob.Lock();
}

TEST(Account, Lock) {
  Account ob(198, 87000);
  ob.Lock();

  ASSERT_NO_THROW(ob.ChangeBalance(-7000));
  EXPECT_EQ(ob.GetBalance(), 80000);
  ASSERT_NO_THROW(ob.ChangeBalance(7000));
  EXPECT_EQ(ob.GetBalance(), 87000);

  ob.Unlock();
  ASSERT_ANY_THROW(ob.ChangeBalance(65000));
  ASSERT_NO_THROW(ob.GetBalance());
}

TEST(Account, ChangeBalance) {
  Account ob1(74, 50000);
  Account ob2(35, 90000);

  ob1.Lock();
  ob2.Lock();

  EXPECT_EQ(ob1.GetBalance(), 50000);
  EXPECT_EQ(ob1.id(), 74);
  ASSERT_NO_THROW(ob1.ChangeBalance(8700));
  EXPECT_EQ(ob1.GetBalance(), 58700);

  ob2.ChangeBalance(-90000);
  EXPECT_EQ(ob2.GetBalance(), 0);
  EXPECT_EQ(ob2.id(), 35);
  ob1.Unlock();
  ob2.Unlock();
}

TEST(Transaction, Transmocktest) {
  MockTransaction trans_1;
  MockAccount ob1(1, 300000);
  MockAccount ob2(2, 200000);
  EXPECT_CALL(trans_1, set_fee(testing::_)).Times(1);
  EXPECT_CALL(trans_1, Make(testing::_, testing::_, testing::_)).Times(1);
  EXPECT_CALL(trans_1, fee()).Times(1);
  EXPECT_CALL(ob1, GetBalance()).Times(2);
  EXPECT_CALL(ob2, GetBalance()).Times(2);
  EXPECT_CALL(ob1, Unlock()).Times(1);
  EXPECT_CALL(ob1, Lock()).Times(1);
  EXPECT_CALL(ob2, Unlock()).Times(1);
  EXPECT_CALL(ob2, Lock()).Times(1);
  ob1.Unlock();
  ob2.Unlock();
  ob1.GetBalance();
  ob2.GetBalance();
  trans_1.set_fee(100);
  trans_1.Make(ob1, ob2, 150000);
  trans_1.fee();
  ob1.GetBalance();
  ob2.GetBalance();
  ob1.Lock();
  ob2.Lock();
}

TEST(Transaction, Throws) {
  Transaction trans;

  Account ob1(1, 1500);
  Account ob2(2, 2000);
  Account ob3(3, 10);
  Account ob4(777, 9000000);

  ASSERT_ANY_THROW(trans.Make(ob1, ob1, 500));
  ASSERT_ANY_THROW(trans.Make(ob4, ob3, -100000));
  ASSERT_ANY_THROW(trans.Make(ob4, ob3, 10));
  ASSERT_NO_THROW(trans.Make(ob1, ob2, 1000));
}

TEST(Transaction, Make) {
  Transaction trans;

  Account ob1(1, 15000);
  Account ob2(2, 3000);


  EXPECT_EQ(trans.fee(), 1);
  trans.set_fee(0);
  ASSERT_NO_THROW(trans.Make(ob1, ob2, 10000));
  ASSERT_NO_THROW(trans.Make(ob2, ob1, 10000));
  trans.set_fee(1);
  EXPECT_EQ(trans.fee(), 1);
  ASSERT_EQ(trans.Make(ob2, ob1, 1000), true);
  EXPECT_EQ(ob1.GetBalance(), 16000);
  EXPECT_EQ(ob2.GetBalance(), 1999);

  ASSERT_EQ(trans.Make(ob1, ob2, 100000), false);
}

TEST(Transaction, Set_fee) {
  Transaction trans;

  Account ob1(1, 25000);
  Account ob2(2, 2500000);

  EXPECT_EQ(trans.fee(), 1);
  trans.set_fee(1000);
  ASSERT_EQ(trans.Make(ob2, ob1, 4300), true);
  EXPECT_EQ(ob2.GetBalance(), 2494700);
  EXPECT_EQ(ob1.GetBalance(), 29300);

  trans.set_fee(0);
  ASSERT_EQ(trans.Make(ob2, ob1, 10000), true);
  EXPECT_EQ(ob2.GetBalance(), 2484700);
  EXPECT_EQ(ob1.GetBalance(), 39300);

}
```
```
cmake_minimum_required(VERSION 3.4)
SET(COVERAGE OFF CACHE BOOL "Coverage")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(Work_Gtest)

add_subdirectory(
	"${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest" 
	"gtest"
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/banking
)

add_executable(
	testing 
	${CMAKE_CURRENT_SOURCE_DIR}/tests/testing.cpp
)

target_link_libraries(
	testing PRIVATE
	gtest gtest_main gmock_main
)

if(COVERAGE)
	target_compile_options(testing PRIVATE --coverage)
	target_link_libraries(testing PRIVATE --coverage)
endif()

target_include_directories(
	testing PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/banking
)

target_link_libraries(
	testing PRIVATE
	banking
)

target_compile_options(
	testing PRIVATE
	-Wall -Wextra -pedantic -Werror
)
```
![Снимок экрана от 2022-05-14 15-08-41](https://user-images.githubusercontent.com/91633974/169374627-62c7984a-de05-49be-87a1-53addc47e406.png)

```
name: banking

on:

  push:
    branches: master
    
  workflow_dispatch:


jobs:
  build_lib:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v3

      - name: build_lib 
        run: |
          cd lab-05/banking
          cmake -H. -B_build
          cd _build
          cmake --build .
  Test_banking:
    runs-on: ubuntu-latest
    
    steps: 
      - uses: actions/checkout@v3
      - name: preparing 
        run: |
              sudo apt install git && git submodule update --init
              sudo apt install lcov
              sudo apt install g++-7 
      - name: Test
        run: | 
          cd lab-05
          mkdir _build && cd _build
          CXX=/usr/bin/g++-7 cmake -DCOVERAGE=1 ..
          cmake --build .
          ./testing
          lcov -t "banking" -o lcov.info -c -d .
          ls
          
      - name: Coveralls Parallel
        uses: coverallsapp/github-action@master
        with:
          github-token: ${{ secrets.github_token }}
          parallel: true
          path-to-lcov: ./lab-05/_build/lcov.info
          coveralls-endpoint: https://coveralls.io

      - name: Coveralls Finished
        uses: coverallsapp/github-action@master
        with:
          github-token: ${{ secrets.github_token }}
          parallel-finished: true
```
