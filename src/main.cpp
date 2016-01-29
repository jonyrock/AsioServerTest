#include "server/server.hpp"

#include <iostream>
#include <stdexcept>

#include "server/sha1Builder.hpp"

using namespace std;


int main(int argc, char* argv[]) {
  try {
    auto& server = Server::Server::getInstance();
    server.run();
  } catch (const std::exception& e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
}