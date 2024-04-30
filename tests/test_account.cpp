#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <Account.h>

class MockAccount: public Account {
public:
  MockAccount(int id, int balance): Account(id, balance){}
  MOCK_METHOD(int, id, (), (const, override));
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};

TEST(Account, Init) {
  MockAccount ac(12, 100);
  EXPECT_CALL(ac, GetBalance());
  EXPECT_CALL(ac, id());
  EXPECT_EQ(ac.GetBalance(), 100);
  EXPECT_EQ(ac.id(), 12);
}

TEST(Account, ChangeBalance) {
  MockAccount ac(12, 100);
  EXPECT_THROW(ac.ChangeBalance(20), std::runtime_error);
  ac.Lock();
  ac.ChangeBalance(20);
  EXPECT_EQ(ac.GetBalance(), 120);
  ac.ChangeBalance(-30);
  EXPECT_EQ(ac.GetBalance(), 90);
  ac.Unlock();
  EXPECT_THROW(ac.ChangeBalance(20), std::runtime_error);
}

TEST(Account, Lock) {
  MockAccount ac(12, 100);
  ac.Unlock();
  ac.Lock();
  EXPECT_THROW(ac.Lock(), std::runtime_error);
}






