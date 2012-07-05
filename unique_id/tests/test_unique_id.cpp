//
// C++ unit tests for unique_id interface.
//

#include <gtest/gtest.h>

#include <unique_id/unique_id.h>
using namespace unique_id;
typedef boost::uuids::uuid uuid;
typedef uuid_msgs::UniqueID UniqueID;

///////////////////////////////////////////////////////////////
// Test cases
///////////////////////////////////////////////////////////////

// Test random generator
TEST(BoostUUID, fromRandom)
{
  static const int N = 1000;
  uuid uu[N];
  for (int i = 0; i < N; ++i)
    {
      uu[i] = fromRandom();
      for (int j = i-1; j >= 0; --j)
        {
          EXPECT_NE(uu[i], uu[j]);
        }
    }
}

TEST(UniqueID, nilMessage)
{
  UniqueID x;
  UniqueID y = toMsg(uuid());
  EXPECT_EQ(x.uuid, y.uuid);
}

TEST(UniqueID, fromRandom)
{
  UniqueID x;
  UniqueID y = toMsg(fromRandom());
  EXPECT_NE(x.uuid, y.uuid);
}

TEST(UniqueID, equivalentMessages)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  boost::uuids::string_generator gen;
  UniqueID x = toMsg(uuid(gen(s)));
  UniqueID y = toMsg(uuid(gen(s)));
  EXPECT_EQ(x.uuid, y.uuid);
  EXPECT_EQ(s, toString(y));
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
