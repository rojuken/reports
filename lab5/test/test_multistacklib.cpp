
#include <gtest.h>
#include <Multistack.h>

TEST(Multistack, can_create_multistack)
{
    ASSERT_NO_THROW(TMultiStack<int> s(9,3));
}

TEST(Multistack, cant_create_stack_with_zero_capacity)
{
    ASSERT_ANY_THROW(TMultiStack<int> s(0,0));
}

TEST(Multistack, can_pop_item_from_stack)
{
  TMultiStack<int> s(9, 3);
  s.push(1, 0);
  s.push(2, 1);
  s.push(3, 2);

  int number = s.pop(0);
  EXPECT_EQ(number, 1);

  number = s.pop(1);
  EXPECT_EQ(number, 2);

  number = s.pop(2);
  EXPECT_EQ(number, 3);
}

TEST(Multistack, can_push_item_to_stack)
{
    TMultiStack<int> s(9,3);
    ASSERT_NO_THROW(s.push(1, 0));
    ASSERT_NO_THROW(s.push(2, 1));
    ASSERT_NO_THROW(s.push(3, 2));
}
