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