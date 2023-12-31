#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"

#include "uint256.h"

typedef struct {
  UInt256 zero; // the value equal to 0
  UInt256 one;  // the value equal to 1
  UInt256 two; // teh value equal to 2
  UInt256 eighteen; // the value equal to 18
  UInt256 max;  // the value equal to (2^256)-1
  UInt256 msb_set; // the value equal to 2^255
  UInt256 rot; // value used to test rotations
} TestObjs;

// Helper functions for implementing tests
void set_all(UInt256 *val, uint32_t wordval);

#define ASSERT_SAME(expected, actual) \
do { \
  ASSERT(expected.data[0] == actual.data[0]); \
  ASSERT(expected.data[1] == actual.data[1]); \
  ASSERT(expected.data[2] == actual.data[2]); \
  ASSERT(expected.data[3] == actual.data[3]); \
  ASSERT(expected.data[4] == actual.data[4]); \
  ASSERT(expected.data[5] == actual.data[5]); \
  ASSERT(expected.data[6] == actual.data[6]); \
  ASSERT(expected.data[7] == actual.data[7]); \
} while (0)

#define INIT_FROM_ARR(val, arr) \
do { \
  for (unsigned i = 0; i < 8; ++i) \
    val.data[i] = arr[i]; \
} while (0)

// Functions to create and cleanup the test fixture object
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// Declarations of test functions
void test_get_bits(TestObjs *objs);
void test_create_from_u32(TestObjs *objs);
void test_create(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_rotate_left(TestObjs *objs);
void test_rotate_right(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_get_bits);
  TEST(test_create_from_u32);
  TEST(test_create);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_negate);
  TEST(test_rotate_left);
  TEST(test_rotate_right);

  TEST_FINI();
}

// Set all of the "words" of a UInt256 to a specific initial value
void set_all(UInt256 *val, uint32_t wordval) {
  for (unsigned i = 0; i < 8; ++i) {
    val->data[i] = wordval;
  }
}

TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *) malloc(sizeof(TestObjs));

  // initialize several UInt256 values "manually"
  set_all(&objs->zero, 0);
  set_all(&objs->one, 0);
  objs->one.data[0] = 1U;
  set_all(&objs->two, 0);
  objs->two.data[0] = 2U;
  set_all(&objs->eighteen, 0);
  objs->eighteen.data[0] = 0x12U;
  set_all(&objs->max, 0xFFFFFFFFU);

  // create a value with only the most-significant bit set
  uint32_t msb_set_data[8] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0x80000000U };
  INIT_FROM_ARR(objs->msb_set, msb_set_data);

  // value with nonzero values in least significant and most significant words
  // Note that the hex representation of this value is
  //   CD000000 00000000 00000000 00000000 00000000 00000000 00000000 000000AB
  uint32_t rot_data[8] = { 0x000000ABU, 0U, 0U, 0U, 0U, 0U, 0U, 0xCD000000U };
  INIT_FROM_ARR(objs->rot, rot_data);

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_get_bits(TestObjs *objs) {
  ASSERT(0U == uint256_get_bits(objs->zero, 0));
  ASSERT(0U == uint256_get_bits(objs->zero, 1));
  ASSERT(0U == uint256_get_bits(objs->zero, 2));
  ASSERT(0U == uint256_get_bits(objs->zero, 3));
  ASSERT(0U == uint256_get_bits(objs->zero, 4));
  ASSERT(0U == uint256_get_bits(objs->zero, 5));
  ASSERT(0U == uint256_get_bits(objs->zero, 6));
  ASSERT(0U == uint256_get_bits(objs->zero, 7));

  ASSERT(1U == uint256_get_bits(objs->one, 0));
  ASSERT(0U == uint256_get_bits(objs->one, 1));
  ASSERT(0U == uint256_get_bits(objs->one, 2));
  ASSERT(0U == uint256_get_bits(objs->one, 3));
  ASSERT(0U == uint256_get_bits(objs->one, 4));
  ASSERT(0U == uint256_get_bits(objs->one, 5));
  ASSERT(0U == uint256_get_bits(objs->one, 6));
  ASSERT(0U == uint256_get_bits(objs->one, 7));

  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 0));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 1));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 2));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 3));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 4));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 5));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 6));
  ASSERT(0xFFFFFFFFU == uint256_get_bits(objs->max, 7));
}

void test_create_from_u32(TestObjs *objs) {
  UInt256 zero = uint256_create_from_u32(0U);
  UInt256 one = uint256_create_from_u32(1U);

  ASSERT_SAME(objs->zero, zero);
  ASSERT_SAME(objs->one, one);

  // my tests

  UInt256 two = uint256_create_from_u32(2U);
  UInt256 five = uint256_create_from_u32(5U);
  UInt256 eighteen = uint256_create_from_u32(18U);

  int32_t five_data[8] = { 5U };
  UInt256 five_test;
  INIT_FROM_ARR(five_test, five_data);

  ASSERT_SAME(objs->two, two);
  ASSERT_SAME(five_test, five);
  ASSERT_SAME(objs->eighteen, eighteen);
}

void test_create(TestObjs *objs) {
  (void) objs;

  uint32_t data1[8] = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U };
  UInt256 val1 = uint256_create(data1);
  ASSERT(1U == val1.data[0]);
  ASSERT(2U == val1.data[1]);
  ASSERT(3U == val1.data[2]);
  ASSERT(4U == val1.data[3]);
  ASSERT(5U == val1.data[4]);
  ASSERT(6U == val1.data[5]);
  ASSERT(7U == val1.data[6]);
  ASSERT(8U == val1.data[7]);

  uint32_t data2[8] = { 0xCU, 0xAU, 7U, 4U, 18U, 5U, 0x12U, 8U };
  UInt256 val2 = uint256_create(data2);
  ASSERT(12U == val2.data[0]);
  ASSERT(10U == val2.data[1]);
  ASSERT(7U == val2.data[2]);
  ASSERT(4U == val2.data[3]);
  ASSERT(18U == val2.data[4]);
  ASSERT(5U == val2.data[5]);
  ASSERT(18U == val2.data[6]);
  ASSERT(8U == val2.data[7]);
}

void test_create_from_hex(TestObjs *objs) {
  UInt256 zero = uint256_create_from_hex("0");
  ASSERT_SAME(objs->zero, zero);

  UInt256 one = uint256_create_from_hex("1");
  ASSERT_SAME(objs->one, one);

  UInt256 max = uint256_create_from_hex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
  ASSERT_SAME(objs->max, max);

  // my tests 

  UInt256 two = uint256_create_from_hex("2");
  ASSERT_SAME(objs->two, two);

  UInt256 eighteen = uint256_create_from_hex("12");
  ASSERT_SAME(objs->eighteen, eighteen);
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = uint256_format_as_hex(objs->zero);
  ASSERT(0 == strcmp("0", s));
  free(s);

  s = uint256_format_as_hex(objs->one);
  ASSERT(0 == strcmp("1", s));
  free(s);

  s = uint256_format_as_hex(objs->max);
  ASSERT(0 == strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", s));
  free(s);

  // my tests
  s = uint256_format_as_hex(objs->two);
  ASSERT(0 == strcmp("2", s));
  free(s);

  s = uint256_format_as_hex(objs->eighteen);
  ASSERT(0 == strcmp("12", s));
  free(s);
}

void test_add(TestObjs *objs) {
  UInt256 result;

  result = uint256_add(objs->zero, objs->zero);
  ASSERT_SAME(objs->zero, result);

  result = uint256_add(objs->zero, objs->one);
  ASSERT_SAME(objs->one, result);
  
  result = uint256_add(objs->one, objs->one);
  ASSERT_SAME(objs->two, result);

  result = uint256_add(objs->max, objs->one);
  ASSERT_SAME(objs->zero, result);

  result = uint256_add(objs->one, objs->max);
  ASSERT_SAME(objs->zero, result);

  // my tests

  uint32_t three_data[8] = { 3U };
  UInt256 three;
  INIT_FROM_ARR(three, three_data);

  int32_t five_data[8] = { 5U };
  UInt256 five;
  INIT_FROM_ARR(five, five_data);

  result = uint256_add(objs->max, objs->zero);
  ASSERT_SAME(objs->max, result);

  result = uint256_add(objs->zero, objs->max);
  ASSERT_SAME(objs->max, result);

  result = uint256_add(objs->two, objs->one);
  ASSERT_SAME(three, result);

  result = uint256_add(objs->two, three);
  ASSERT_SAME(five, result);
}

void test_sub(TestObjs *objs) {
  UInt256 result;

  result = uint256_sub(objs->zero, objs->zero);
  ASSERT_SAME(objs->zero, result);

  result = uint256_sub(objs->one, objs->zero);
  ASSERT_SAME(objs->one, result);

  result = uint256_sub(objs->one, objs->one);
  ASSERT_SAME(objs->zero, result);

  result = uint256_sub(objs->zero, objs->one);
  ASSERT_SAME(objs->max, result);

  // my tests

  result = uint256_sub(objs->max, objs->zero);
  ASSERT_SAME(objs->max, result);

  result = uint256_sub(objs->zero, objs->max);
  ASSERT_SAME(objs->one, result);

  uint32_t three_data[8] = { 3U };
  UInt256 three;
  INIT_FROM_ARR(three, three_data);

  result = uint256_sub(objs->two, objs->one);
  ASSERT_SAME(objs->one, result);

  result = uint256_sub(three, objs->two);
  ASSERT_SAME(objs->one, result);

  result = uint256_sub(objs->two, objs->zero);
  ASSERT_SAME(objs->two, result);

  // checks if subtracting to get negative values returns max value

  result = uint256_sub(objs->one, objs->two);
  ASSERT_SAME(objs->max, result);

  result = uint256_sub(objs->one, three);
  ASSERT_SAME(objs->max, result);

  result = uint256_sub(objs->two, three);
  ASSERT_SAME(objs->max, result);
  
  result = uint256_sub(objs->zero, objs->two);
  ASSERT_SAME(objs->max, result);

  result = uint256_sub(objs->zero, three);
  ASSERT_SAME(objs->max, result);
}

void test_negate(TestObjs *objs) {
  UInt256 result;
  UInt256 temp;

  result = uint256_negate(objs->zero);
  ASSERT_SAME(objs->zero, result);

  result = uint256_negate(objs->one);
  ASSERT_SAME(objs->max, result);

  result = uint256_negate(objs->max);
  ASSERT_SAME(objs->one, result);

  // my tests
  
  temp = uint256_add(objs->one, objs->zero); // check to see if negate works properly after an addition
  result = uint256_negate(temp);
  ASSERT_SAME(objs->max, result);

  temp = uint256_sub(objs->max, objs->zero); // check to see if negate works properly after a subtraction
  result = uint256_negate(temp);
  ASSERT_SAME(objs->one, result);

  temp = uint256_negate(objs->one); // check to see if negating twice returns the original value
  ASSERT_SAME(objs->max, temp);
  result = uint256_negate(temp);
  ASSERT_SAME(objs->one, result);

}

void test_rotate_left(TestObjs *objs) {
  UInt256 result;

  // rotating the value with just the most significant bit set
  // one position to the left should result in the value equal to 1
  // (i.e., the value with only the least significant bit set)
  result = uint256_rotate_left(objs->msb_set, 1);
  ASSERT_SAME(objs->one, result);

  // after rotating the "rot" value left by 4 bits, the resulting value should be
  //   D0000000 00000000 00000000 00000000 00000000 00000000 00000000 00000ABC
  result = uint256_rotate_left(objs->rot, 4);
  ASSERT(0x00000ABCU == result.data[0]);
  ASSERT(0U == result.data[1]);
  ASSERT(0U == result.data[2]);
  ASSERT(0U == result.data[3]);
  ASSERT(0U == result.data[4]);
  ASSERT(0U == result.data[5]);
  ASSERT(0U == result.data[6]);
  ASSERT(0xD0000000U == result.data[7]);
}

void test_rotate_right(TestObjs *objs) {
  UInt256 result;

  // rotating 1 right by 1 position should result in a value with just
  // the most-significant bit set
  result = uint256_rotate_right(objs->one, 1);
  // for (int i = 0; i < 8; i++) {
  //   printf("%d\n", objs->msb_set.data[i]);
  // }
  ASSERT_SAME(objs->msb_set, result);

  // after rotating the "rot" value right by 4 bits, the resulting value should be
  //   BCD00000 00000000 00000000 00000000 00000000 00000000 00000000 0000000A
  result = uint256_rotate_right(objs->rot, 4);
  ASSERT(0x0000000AU == result.data[0]); // 10
  ASSERT(0U == result.data[1]);
  ASSERT(0U == result.data[2]);
  ASSERT(0U == result.data[3]);
  ASSERT(0U == result.data[4]);
  ASSERT(0U == result.data[5]);
  ASSERT(0U == result.data[6]);
  ASSERT(0xBCD00000U == result.data[7]); //3167748096  -1127219200
}
