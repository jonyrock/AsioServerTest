#include "sha1Builder.hpp"
// Please see comments in sha.h for licensing information, etc.
//

// Many people don't like the names I usually use for namespaces, so I've kept this one
// short and simple.
//
namespace crypto {
namespace {
uint32_t ROTL(uint32_t const &value, unsigned bits) {
  uint32_t mask = (1 << bits) - 1;
  return value << bits | (value >> (32-bits))&mask;
}

struct f1 : ternary_operator {
  uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x&z);
  }
};

struct f2 : ternary_operator {
  uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
  }
};

struct f3 : ternary_operator {
  uint32_t operator()(uint32_t x, uint32_t y, uint32_t z) {
    return (x&y) ^ (x&z) ^ (y&z);
  }
};

uint32_t word(int a, int b, int c, int d) {
  a &= 0xff;
  b &= 0xff;
  c &= 0xff;
  d &= 0xff;
  int val =  a << 24 | b << 16 | c << 8 | d;
  return val;
}
}

// hash a 512-bit block of input.
//
void Sha1Builder::hash_block(std::vector<uint32_t> const &block) {
  assert(block.size() == block_words);

  int t;
  std::copy(block.begin(), block.end(), W.begin());
  for (t=16; t<80; t++) {
    W[t] = ROTL(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
  }

  a = H[0]; b = H[1]; c = H[2]; d = H[3]; e = H[4];

  for (t=0; t<80; t++) {
    T = ROTL(a, 5) + (*fs[t])(b, c, d) + e + K[t] + W[t];
    e = d;
    d = c;
    c = ROTL(b, 30);
    b = a;
    a = T;
  }
  H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e;
}

// Pad the input to a multiple of 512 bits, and put the length
// in binary at the end. 
std::string Sha1Builder::pad(std::string const &input, size_t size) {
  size_t length = size * 8 + 1;
  size_t remainder = length % block_bits;
  size_t pad_len = block_bits-remainder;

  if (pad_len < min_pad)
    pad_len += block_bits;
  ++pad_len;

  pad_len &= ~7;
  std::string padding(pad_len/8, '\0');

  for (size_t i=0; i<sizeof(padding.size()); i++) {
    padding[padding.size() - i - 1] = (length - 1) >> (i * 8) & 0xff;
  }
  padding[0] |= (unsigned char)0x80;

  std::string ret(input+padding);
  return ret;
}

// Turn 64 bytes into a block of 16 uint32_t's.
std::vector<uint32_t> Sha1Builder::make_block(std::string const &in) {
  assert(in.size() >= block_bytes);

  std::vector<uint32_t> ret(block_words);

  for (size_t i=0; i<block_words; i++) {
    size_t s = i*4;
    ret[i] = word(in[s], in[s+1], in[s+2], in[s+3]);
  }
  return ret;
}


// Construct a SHA-1 object. More expensive that typical 
// ctor, but not expected to be copied a lot or anything
// like that, so it should be fairly harmless.
Sha1Builder::Sha1Builder() : K(80), H(5), W(80), fs(80), total_size(0) {
  static const uint32_t H0[] = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0
  };
  static const uint32_t Ks[] = {
    0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6
  };

  std::copy(H0, H0+hash_size, H.begin());

  std::fill_n(K.begin()+00, 20, Ks[0]);
  std::fill_n(K.begin()+20, 20, Ks[1]);
  std::fill_n(K.begin()+40, 20, Ks[2]);
  std::fill_n(K.begin()+60, 20, Ks[3]);

  static f1 sf1;
  static f2 sf2;
  static f3 sf3;

  std::fill_n(fs.begin()+00, 20, &sf1);
  std::fill_n(fs.begin()+20, 20, &sf2);
  std::fill_n(fs.begin()+40, 20, &sf3);
  std::fill_n(fs.begin()+60, 20, &sf2);
}


void Sha1Builder::append(const std::string& input) {
  std::string temp(pad(input, total_size + input.size()));
  std::vector<uint32_t> block(block_size);

  size_t num = temp.size() / block_bytes;

  for (unsigned block_num = 0; block_num < num; block_num++) {
    size_t s;
    for (size_t i = 0; i < block_size; i++) {
      s = block_num * block_bytes + i * 4;
      block[i] = word(temp[s], temp[s + 1], temp[s + 2], temp[s + 3]);
    }
    hash_block(block);
  }

}

std::vector<uint32_t> Sha1Builder::value() const { 
  return H;
}


}