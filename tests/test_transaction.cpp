#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Account.h>
#include <Transaction.h>

class MockAccount: public Account {
public:
  MockAccount(int id, int balance): Account(id, balance){}
  MOCK_CONST_METHOD0(id, int());
  MOCK_CONST_METHOD0(GetBalance, int());
  MOCK_METHOD1(ChangeBalance, void(int));
  MOCK_METHOD0(Lock, void());
  MOCK_METHOD0(Unlock, void());
};

class MockTransaction : public Transaction {
public:
  MockTransaction() : Transaction(){};
  MOCK_METHOD3(Make, bool(Account&, Account&, int));
  MOCK_CONST_METHOD0(fee, int());
  MOCK_METHOD1(set_fee, void(int));
};

TEST(Transaction, Init) {
  MockTransaction tr;
  EXPECT_CALL(tr, fee());
  EXPECT_EQ(tr.fee(), 1);
}

TEST(Transaction, Make) {
  MockTransaction tr;
  MockAccount ac_from(1, 1000);
  MockAccount ac_to(2, 1000);
  EXPECT_CALL(tr, set_fee(testing::_)).Times(4);
  EXPECT_CALL(tr, fee()).Times(2);
  EXPECT_CALL(tr, Make(testing::_, testing::_, testing::_)).Times(7);
  EXPECT_CALL(ac_from, Lock()).Times(2);
  EXPECT_CALL(ac_from, ChangeBalance(testing::_)).Times(2);
  EXPECT_CALL(ac_from, Unlock()).Times(2);
  EXPECT_CALL(ac_from, GetBalance()).Times(2);
  EXPECT_CALL(ac_to, GetBalance()).Times(2);
  tr.set_fee(10);
  EXPECT_EQ(tr.fee(), 10);
  tr.set_fee(1);
  EXPECT_THROW(tr.Make(ac_from, ac_from, 150), std::logic_error);
  EXPECT_THROW(tr.Make(ac_from, ac_to, -50), std::invalid_argument);
  EXPECT_THROW(tr.Make(ac_from, ac_to, 50), std::logic_error);
  tr.set_fee(50);
  EXPECT_EQ(tr.Make(ac_from, ac_to, 98), false);
  tr.set_fee(1);
  ac_from.Lock();
  EXPECT_THROW(tr.Make(ac_from, ac_to, 150), std::runtime_error);
  ac_from.ChangeBalance(-900);
  ac_from.Unlock();
  EXPECT_EQ(tr.Make(ac_from, ac_to, 150), false);
  EXPECT_EQ(ac_from.GetBalance(), 100);
  EXPECT_EQ(ac_to.GetBalance(), 1000);
  ac_from.Lock();
  ac_from.ChangeBalance(900);
  ac_from.Unlock();
  EXPECT_EQ(tr.Make(ac_from, ac_to, 150), true);
  EXPECT_EQ(ac_from.GetBalance(), 1000 - tr.fee() - 150);
  EXPECT_EQ(ac_to.GetBalance(), 1000 + 150);
}
  
  
  
  
  
  
  
  
