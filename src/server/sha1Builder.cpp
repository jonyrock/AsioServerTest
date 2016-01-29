#include "sha1Builder.hpp"

#include <sstream>
#include <boost/algorithm/hex.hpp>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp> 


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
void Sha1Builder::hashBlock(std::vector<uint32_t> const &block) {
  assert(block.size() == BLOCK_WORDS);

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


// Turn 64 bytes into a block of 16 uint32_t's.
std::vector<uint32_t> Sha1Builder::makeBlock(std::string const &in) {
  assert(in.size() >= BLOCK_BYTES);

  std::vector<uint32_t> ret(BLOCK_WORDS);

  for (size_t i=0; i< BLOCK_WORDS; i++) {
    size_t s = i*4;
    ret[i] = word(in[s], in[s+1], in[s+2], in[s+3]);
  }
  return ret;
}


// Construct a SHA-1 object. More expensive that typical 
// ctor, but not expected to be copied a lot or anything
// like that, so it should be fairly harmless.
Sha1Builder::Sha1Builder() : K(80), H(5), W(80), fs(80) {
  static const uint32_t H0[] = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0
  };
  static const uint32_t Ks[] = {
    0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6
  };

  std::copy(H0, H0+ HASH_SIZE, H.begin());

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

  m_inputSize += input.size();
  
  std::string temp = m_appendReminder + input;

  std::vector<uint32_t> block(blockSize);

  size_t num = temp.size() / BLOCK_BYTES;

  for (unsigned blockNum = 0; blockNum < num; blockNum++) {
    size_t s;
    for (size_t i = 0; i < blockSize; i++) {
      s = blockNum * BLOCK_BYTES + i * 4;
      block[i] = word(temp[s], temp[s + 1], temp[s + 2], temp[s + 3]);
    }
    hashBlock(block);
  }
  
  size_t reminderLength = temp.size() % BLOCK_BYTES;
  m_appendReminder = temp.substr(temp.length() - reminderLength, reminderLength);

}

void Sha1Builder::end() {

  // pad
  size_t length = m_inputSize * 8 + 1;
  size_t remainder = length % BLOCK_BITS;
  size_t padLen = BLOCK_BITS - remainder;

  if (padLen < MIN_PAD) {
    padLen += BLOCK_BITS;
  }
  padLen++;

  padLen &= ~7;
  std::string padding(padLen / 8, '\0');

  for (size_t i = 0; i < sizeof(padding.size()); i++) {
    padding[padding.size() - i - 1] = (length - 1) >> (i * 8) & 0xff;
  }
  padding[0] |= (unsigned char)0x80;

  m_appendReminder += padding;
  
  // small hack: add empty string to the updated reminder string
  append("");
  
}

std::vector<uint32_t> Sha1Builder::value() const {
  return H;
}

std::string Sha1Builder::toHexStr(uint32_t n) {
  std::string res;
  for (char i = 7; i >= 0; i--) {
    int v = boost::lexical_cast<int>(n >> (i * 4) & 0xf);
    res += boost::str(boost::format("%x") % v);
  }
  return res;
}

std::string Sha1Builder::toString() const {
  std::stringstream res;
  for(auto& h: H) {
    res << toHexStr(h);
  }
  return res.str();
}


}