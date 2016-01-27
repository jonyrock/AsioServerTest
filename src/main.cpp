#include "server/server.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {
  try {
    Server::Server server;
    server.run();  
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << endl;
  }
}