
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
  static const size_t hash_size = 5;
  static const size_t min_pad = 64;
  static const size_t block_bits = 512;
  static const size_t block_bytes = block_bits / 8;
  static const size_t block_words = block_bytes / 4;

  std::vector<uint32_t> K;
  std::vector<uint32_t> H;
  std::vector<uint32_t> W;
  std::vector<ternary_operator *> fs;
  uint32_t a, b, c, d, e, T;
  static const size_t block_size = 16;
  static const size_t bytes_per_word = 4;
  size_t total_size;

  // hash a 512-bit block of input.
  //
  void hash_block(std::vector<uint32_t> const &block);

  // Pad the input to a multiple of 512 bits, and add the length
  // in binary to the end.
  static std::string pad(std::string const &input, size_t size);

  // Turn 64 bytes into a block of 16 uint32_t's.
  std::vector<uint32_t> make_block(std::string const &in);

public:

  
  Sha1Builder();
  void append(const std::string& str);
  std::vector<uint32_t> value() const;

};
}
