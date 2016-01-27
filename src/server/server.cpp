#include "server.hpp"

#include <boost/asio.hpp>

#include <iostream>


using namespace std;

namespace Server {

Server::Server(short port): m_port(port) {

}

void Server::run() {
  cout << "server start" << endl;
}

} // namespace Server