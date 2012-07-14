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

TEST(BoostUUID, emptyURL)
{
  std::string s;
  uuid x = fromURL(s);
  uuid y = fromURL(s);
  EXPECT_EQ(x, y);
  // MUST yield same result as Python fromURL() function:
  EXPECT_EQ(toString(x), "1b4db7eb-4057-5ddf-91e0-36dec72071f5");
}

TEST(BoostUUID, sameURL)
{
  std::string s("http://openstreetmap.org/node/1");
  uuid x = fromURL(s);
  uuid y = fromURL(s);
  EXPECT_EQ(x, y);
  // MUST yield same result as Python fromURL() function:
  EXPECT_EQ(toString(x), "ef362ac8-9659-5481-b954-88e9b741c8f9");
}

TEST(BoostUUID, differentOsmNamespace)
{
  uuid x = fromURL("http://openstreetmap.org/node/1");
  uuid y = fromURL("http://openstreetmap.org/way/1");
  EXPECT_NE(x, y);
  // MUST yield same result as Python fromURL() function:
  EXPECT_EQ(toString(y), "b3180681-b125-5e41-bd04-3c8b046175b4");
}

TEST(BoostUUID, actualOsmNode)
{
  uuid x = fromURL("http://openstreetmap.org/node/1");
  uuid y = fromURL("http://openstreetmap.org/node/152370223");
  EXPECT_NE(x, y);
  // MUST yield same result as Python fromURL() function:
  EXPECT_EQ(toString(y), "8e0b7d8a-c433-5c42-be2e-fbd97ddff9ac");
}

TEST(BoostUUID, fromString)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  std::string r = toString(fromString(s));
  EXPECT_EQ(s, r);
}

TEST(BoostUUID, fromHexString)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  std::string s_hex("da7c242f2efe5175996149cc621b80b9");
  std::string r = toString(fromString(s_hex));
  EXPECT_EQ(s, r);
}

TEST(BoostUUID, fromBracesString)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  std::string s_braces = "{" + s + "}";
  std::string r = toString(fromString(s_braces));
  EXPECT_EQ(s, r);
}

TEST(BoostUUID, fromUrnString)
{
  // This documents boost 1.46.1 behavior, but is an undefined
  // fromString() input, not really a valid test case.
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  std::string s_urn = "urn:uuid:" + s;
  std::string r = toString(fromString(s_urn));
  EXPECT_NE(s, r);
}

TEST(BoostUUID, fromTooLongString)
{
  // This documents boost 1.46.1 behavior, but is an undefined
  // fromString() input, not really a valid test case.
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  std::string s_too_long = s + "-0001";
  std::string r = toString(fromString(s_too_long));
  EXPECT_EQ(s, r);
}

TEST(BoostUUID, fromTooShortString)
{
  // This documents boost 1.46.1 behavior, but is an undefined
  // fromString() input, not really a valid test case.
  std::string s("da7c242f-2efe-5175-9961-49cc621b80");
  try
    {
      uuid x = fromString(s);
      FAIL();                           // expected exception not thrown
      EXPECT_NE(toString(x), s);
    }
  catch (std::runtime_error &e)
    {
      EXPECT_EQ(e.what(), std::string("invalid uuid string"));
    }
  catch (...)
    {
      FAIL();                           // unexpected exception
    }
}

TEST(BoostUUID, fromBogusString)
{
  // This documents boost 1.46.1 behavior, but is an undefined
  // fromString() input, not really a valid test case.
  std::string s("Invalid UUID string");
  try
    {
      uuid x = fromString(s);
      FAIL();                           // expected exception not thrown
      EXPECT_NE(toString(x), s);
    }
  catch (std::runtime_error &e)
    {
      EXPECT_EQ(e.what(), std::string("invalid uuid string"));
    }
  catch (...)
    {
      FAIL();                           // unexpected exception
    }
}

TEST(UniqueID, nilMessage)
{
  UniqueID x;
  UniqueID y = toMsg(uuid());
  EXPECT_EQ(x.uuid, y.uuid);
}

TEST(UniqueID, randomMessage)
{
  UniqueID x;
  UniqueID y = toMsg(fromRandom());
  EXPECT_NE(x.uuid, y.uuid);
}

TEST(UniqueID, equivalentMessages)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  UniqueID x = toMsg(fromString(s));
  UniqueID y = toMsg(fromString(s));
  EXPECT_EQ(x.uuid, y.uuid);
  EXPECT_EQ(s, toString(y));
}

TEST(UniqueID, toAndFromMessage)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  uuid x = uuid(fromString(s));
  uuid y = fromMsg(toMsg(x));
  EXPECT_EQ(x, y);
}

TEST(UniqueID, messageToString)
{
  std::string s("da7c242f-2efe-5175-9961-49cc621b80b9");
  UniqueID x = toMsg(fromString(s));
  std::string y = toString(x);
  EXPECT_EQ(s, y);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
