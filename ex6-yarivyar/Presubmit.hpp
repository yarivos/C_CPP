#ifndef PRESUBMIT_H
#define PRESUBMIT_H
#include "HashMap.hpp"
#include "Helpers.h"
#include <iostream>

#ifndef __DISABLE_PRESUBMISSION_TESTS
#define PRESUBMISSION_ASSERT(testCase) __ASSERT_TEST("Presubmission", testCase)
#else
#define PRESUBMISSION_ASSERT(testCase)
#endif

//-------------------------------------------------------
// Tests
//-------------------------------------------------------

int __presubmit_testCreateHashMaps() {
  HashMap<int, int> intMap{};
  HashMap<std::string, std::string> stringMap{};
  HashMap<double, double> doubleMap{};

  ASSERT_MAP_PROPERTIES(intMap, 0, 16, 0);
  ASSERT_MAP_PROPERTIES(stringMap, 0, 16, 0);
  ASSERT_MAP_PROPERTIES(doubleMap, 0, 16, 0);

  auto dynamicallyAllocated = new HashMap<int, std::string>();
  delete dynamicallyAllocated;

  return 1;
}

int __presubmit_testInsert() {
  HashMap<int, int> map;
  map.insert(5, 1);
  map.insert(1, 5);

  return 1;
}

int __presubmit_testSize() {
  HashMap<int, int> map;
  RETURN_ASSERT_TRUE(map.size() == 0);
}

int __presubmit_testCapacity() {
  HashMap<int, int> map;
  RETURN_ASSERT_TRUE(map.capacity() == 16); // Default capacity is 16.
}

int __presubmit_testEmpty() {
  HashMap<int, int> map;
  RETURN_ASSERT_TRUE(map.empty());
}

int __presubmit_testClear() {
  HashMap<int, int> map;
  if (!map.empty()) {
    return 0;
  }

  map.insert(1, 5);
  map.clear();

  RETURN_ASSERT_TRUE(map.empty());
}

int __presubmit_testBucketSize() {
  HashMap<int, int> map;
  map.insert(1, 1);
  RETURN_ASSERT_TRUE(map.bucket_size (1) == 1);
}

int __presubmit_testGetElement() {
  HashMap<int, int> map;
  map.insert(5, 1);

  ASSERT_TRUE(map.at(5) == 1);  // using HashMap::at
  RETURN_ASSERT_TRUE(map[5] == 1); // using HashMap::operator[]
}

int __presubmit_testContainsKey() {
  HashMap<int, int> map;
  map.insert(5, 1);
  RETURN_ASSERT_TRUE(map.contains_key (5));
}

int __presubmit_testAssignment() {
  HashMap<int, int> m1{};
  m1.insert(1, 2);

  HashMap<int, int> m2 = m1;
  RETURN_ASSERT_TRUE(m2.contains_key (1));
}

int __presubmit_testComparison() {
  HashMap<int, int> m1{};
  HashMap<int, int> m2{};

  RETURN_ASSERT_TRUE(m1 == m2);
}

int __presubmit_testIterator() {
  HashMap<int, int> map;

  map.begin();
  map.end();
  map.cbegin();
  map.cend();

  return 1;
}

int __presubmit_testVectorsCtor() {
  /* Create the keys & values */
  std::vector<int> keys;
  std::vector<int> values;
  keys.push_back(1);
  values.push_back(1);

  /* Create the map */
  HashMap<int, int> map(keys, values);
  RETURN_ASSERT_TRUE(map.size() == 1 && map.contains_key (1));
}

int __presubmit_testCopyCtor() {
  HashMap<int, int> m1{};
  m1.insert(1, 2);

  HashMap<int, int> m2(m1);
  RETURN_ASSERT_TRUE(m2.contains_key (1));
}

//-------------------------------------------------------
//  The main entry point
//-------------------------------------------------------
int runPreSubmissionChecks() {
  PRESUBMISSION_ASSERT(__presubmit_testCreateHashMaps);
  PRESUBMISSION_ASSERT(__presubmit_testInsert);
  PRESUBMISSION_ASSERT(__presubmit_testSize);
  PRESUBMISSION_ASSERT(__presubmit_testCapacity);
  PRESUBMISSION_ASSERT(__presubmit_testEmpty);
  PRESUBMISSION_ASSERT(__presubmit_testClear);
  PRESUBMISSION_ASSERT(__presubmit_testBucketSize);
  PRESUBMISSION_ASSERT(__presubmit_testGetElement);
  PRESUBMISSION_ASSERT(__presubmit_testContainsKey);
  PRESUBMISSION_ASSERT(__presubmit_testAssignment);
  PRESUBMISSION_ASSERT(__presubmit_testComparison);
  PRESUBMISSION_ASSERT(__presubmit_testIterator);
  PRESUBMISSION_ASSERT(__presubmit_testVectorsCtor);
  PRESUBMISSION_ASSERT(__presubmit_testCopyCtor);

  return 1;
}

#endif