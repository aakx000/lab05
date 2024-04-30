#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Account.h>
#include <Transaction.h>

class MockAccount: public Account {
public:
  MockAccount(int id, int balance): Account(id, balance){}
  MOCK_METHOD(int, id, (), (const, override));
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction {
public:
  MockTransaction() : Transaction(){};
  MOCK_METHOD(bool, Make, (Account&, Account&, int), (override));
  MOCK_METHOD(int, fee, (), (const, override));
  MOCK_METHOD(void, set_fee, (int), (override));
}

TEST(Transaction, Init) {
  MockTransaction tr;
  EXPECT_EQ(tr.fee(), 1);
}

TEST(Transaction, Make) {
  MockTransaction tr;
  MockAccount ac_from;
  MockAccount ac_to;
  tr.set_fee(10);
  EXPECT_EQ(tr.fee(), 10);
  tr.set_fee(1);
  EXPECT_THROW(tr.Make(ac_from(1, 1000), ac_to(1, 1000), 150), std::logic_error);
  EXPECT_THROW(tr.Make(ac_from(1, 1000), ac_to(2, 1000), -50), std::invalid_argument);
  EXPECT_THROW(tr.Make(ac_from(1, 1000), ac_to(2, 1000), 50), std::logic_error);
  tr.set_fee(50);
  EXPECT_EQ(tr.make(ac_from(1, 1000), ac_to(2, 1000), 98), false);
  tr.set_fee(1);
  ac_from.Lock();
  EXPECT_THROW(tr.Make(ac_from(1, 1000), ac_to(2, 1000), 150), std::runtime_error);
  ac_from.Unlock();
  EXPECT_EQ(tr.make(ac_from(1, 100), ac_to(2, 1000), 150), false);
  EXPECT_EQ(ac_from.GetBalance(), 100);
  EXPECT_EQ(ac_to.GetBalance(), 1000);
  EXPECT_EQ(tr.make(ac_from(1, 1000), ac_to(2, 1000), 150), true);
  EXPECT_EQ(ac_from.GetBalance(), 1000 - tr.fee() - 150);
  EXPECT_EQ(ac_to.GetBalance(), 1000 + 150);
}
  
  
  
  
  
  
  
  
