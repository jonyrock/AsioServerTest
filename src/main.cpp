#include "server/server.hpp"

#include <iostream>
#include <stdexcept>

#include "server/sha1Builder.hpp"

#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
  try {

//    crypto::Sha1Builder hasher;
//    hasher.append("ololo");
//    std::vector<uint32_t> h = hasher.value(); 
//    for(auto i: h) {
//      std::cout << i << std::endl;
//    }
////    
////
////    sprintf(textToWrite,"%lu", h[0]);
////    
////    cout << textToWrite[0];
////    cout << textToWrite[1];
////    cout << textToWrite[2];
////    cout << textToWrite[3];
//
//
//    std::uint32_t value(3033586844);
//    std::bitset<32> bits(value);
//    std::cout << bits.to_string() << std::endl;
//
//    char v = 'b';
//    
//    std::bitset<4> bits2(value);
//    std::cout << bits2.to_string();
//    
//    return 0;

    auto& server = Server::Server::getInstance();
    server.run();
  } catch (const std::exception& e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
}