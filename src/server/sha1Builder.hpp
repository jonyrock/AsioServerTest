
#pragma once 


#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdint.h>


namespace crypto {
namespace {
struct ternary_operator {
  virtual uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) = 0;
};
}

class Sha1Builder {
  static const size_t HASH_SIZE = 5;
  static const size_t MIN_PAD = 64;
  static const size_t BLOCK_BITS = 512;
  static const size_t BLOCK_BYTES = BLOCK_BITS / 8;
  static const size_t BLOCK_WORDS = BLOCK_BYTES / 4;

  std::vector<uint32_t> K;
  std::vector<uint32_t> H;
  std::vector<uint32_t> W;
  std::vector<ternary_operator *> fs;
  uint32_t a, b, c, d, e, T;
  static const size_t blockSize = 16;
  static const size_t bytes_per_word = 4;
  size_t totalSize;

  // hash a 512-bit block of input.
  //
  void hashBlock(std::vector<uint32_t> const &block);

  // Pad the input to a multiple of 512 bits, and add the length
  // in binary to the end.
  static std::string pad();

  // Turn 64 bytes into a block of 16 uint32_t's.
  std::vector<uint32_t> makeBlock(std::string const &in);
  
  size_t m_inputSize = 0;
  std::string m_appendReminder;

public:

  static std::string toHexStr(uint32_t);
  
  Sha1Builder();
  void append(const std::string& str);
  void end();
  std::vector<uint32_t> value() const;
  std::string toString() const;

};
}
