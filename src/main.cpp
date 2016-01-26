#include "server.hpp"

#include <iostream>


using namespace std;

int main() {
  Server server;
  server.start();
  cout << "server started" << endl;
}