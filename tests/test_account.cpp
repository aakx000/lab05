#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <Account.h>

class MockAccount: public Account {
public:
  MockAccount(int id, int balance): Account(id, balance){}
  MOCK_CONST_METHOD0(id, int());
  MOCK_CONST_METHOD0(GetBalance, int());
  MOCK_METHOD1(ChangeBalance, void(int));
  MOCK_METHOD0(Lock, void());
  MOCK_METHOD0(Unlock, void());
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
  EXPECT_CALL(ac, ChangeBalance(testing::_)).Times(4);
  EXPECT_CALL(ac, Lock()).Times(4);
  EXPECT_CALL(ac, GetBalance()).Times(2);
  EXPECT_CALL(ac, Unlock());
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
  EXPECT_CALL(ac, Unlock());
  EXPECT_CALL(ac, Lock()).Times(2);
  ac.Unlock();
  ac.Lock();
  EXPECT_THROW(ac.Lock(), std::runtime_error);
}






