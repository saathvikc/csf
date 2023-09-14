#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint32_t value.
// Only the least-significant 32 bits are initialized directly,
// all other bits are set to 0.
// MS1
UInt256 uint256_create_from_u32(uint32_t val) {
  UInt256 result;
  for (int i = 0; i < 8; i++) {
    if (i > 0) {
      result.data[i] = (uint32_t) 0;
    } else {
      result.data[i] = val;
    }
  }
  return result;
}

// Create a UInt256 value from an array of NWORDS uint32_t values.
// The element at index 0 is the least significant, and the element
// at index 7 is the most significant.
// MS1
UInt256 uint256_create(const uint32_t data[8]) {
  UInt256 result;
  for (int i = 0; i < 8; i++) {
    result.data[i] = data[i];
  }
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
// MS2
UInt256 uint256_create_from_hex(const char *hex) { // tests passed but needs more work
  UInt256 result;
  char *ptr;

  for (int j = 0; j < 8; j++) {
    result.data[j] = (uint32_t) 0;
  }

  int length = strlen(hex);
  uint32_t digit = (uint32_t) 0;
 
  if (length >= 8) {
    int dist = length - 8;
    char temp[9];

    for (int i = 7; i >= 0; i--) {
      // memset(temp, '\0', sizeof(temp));
      strncpy(temp, hex + dist, 8);
      digit = strtoul(temp, &ptr, 16);
      result.data[i] = digit;
      dist-=8;
    }
  } else {
    digit = strtoul(hex, &ptr, 16);
    result.data[0] = digit;
  }
  
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
// MS2
char *uint256_format_as_hex(UInt256 val) {
  char *hex = (char*) malloc (9 * sizeof(char));
  
  for (int i = 7; i >= 0; i--) {
    sprintf(hex, "%x", val.data[i]);
    // printf("%d", val.data[i]);
    // printf("%s\n", str);
    // if (0 != strcmp("0", str)) {
    //   strcat(hex, str);
    // } else if (i == 0) {
    //   strcat(hex, str);
    // }
  }

  hex[9] = '\0';
  printf("%s", hex);
  return hex;
}

// Get 32 bits of data from a UInt256 value.
// Index 0 is the least significant 32 bits, index 7 is the most
// significant 32 bits.
// MS1
uint32_t uint256_get_bits(UInt256 val, unsigned index) {
  uint32_t bits;
  bits = val.data[index];
  return bits;
}

// Compute the sum of two UInt256 values.
// MS2
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;
  uint32_t overflow = 0;
  for (int i = 0; i < 8; i++) {
    uint32_t left_temp = left.data[i];
    uint32_t right_temp = right.data[i];
    uint32_t sum_temp = left_temp + right_temp + overflow;
    if (sum_temp < left_temp || sum_temp < right_temp) {
      overflow = 1;
    } else {
      overflow = 0;
    }

    sum.data[i] = sum_temp;
  }
  return sum;
}

// Compute the difference of two UInt256 values.
// MS2
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result;
  UInt256 right_negate = uint256_negate(right);
  result = uint256_add(left, right_negate);

  return result;
}

// Return the two's-complement negation of the given UInt256 value.
// MS2
UInt256 uint256_negate(UInt256 val) {
  UInt256 result;
  for (int i = 0; i < 8; i++) {
    uint32_t temp = val.data[i];
    temp = ~temp;
    result.data[i] = temp;
  } 

  UInt256 one = uint256_create_from_u32((uint32_t) 1);
  result = uint256_add(result, one);
  return result;
}

// Return the result of rotating every bit in val nbits to
// the left.  Any bits shifted past the most significant bit
// should be shifted back into the least significant bits.
// MS2
UInt256 uint256_rotate_left(UInt256 val, unsigned nbits) {
  UInt256 result;
  for (unsigned i = 0; i < nbits; i++) {
    unsigned mask;
    mask = (1 << nbits) - 1;
    int lastXbits = val.data[0] & mask;
    printf("%d", lastXbits);
  }
  return result;
}

// Return the result of rotating every bit in val nbits to
// the right. Any bits shifted past the least significant bit
// should be shifted back into the most significant bits.
// MS2
UInt256 uint256_rotate_right(UInt256 val, unsigned nbits) {
  UInt256 result;

  for (int i = 0; i < 8; i++) {
    result.data[i] = 0;
  }

  unsigned lastXbits;
  unsigned lastYbits;
  for (unsigned i = 0; i < nbits; i++) {
    unsigned mask;
    mask = (1 << nbits) - 1;
    lastXbits = val.data[0] & mask;
    // printf("%d", lastXbits);
  }

  for (unsigned i = 0; i < nbits; i++) {
    unsigned mask;
    mask = (1 << nbits) - 1;
    lastYbits = val.data[8] & mask;
    //printf("%d", lastYbits);
  }

  result.data[8] |= lastXbits << 31;

  printf("%d", result.data[8]);

  // for (int i = 0; i < 8; i++) {
  //   printf("%d", result.data[i]);
  // }

  return result;
}
