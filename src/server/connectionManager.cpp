#include "connectionManager.hpp"

#include <algorithm>
#include <boost/bind.hpp>

namespace Server {

void ConnectionManager::start(ConnectionPtr c) {
  // TODO: lock connections in multithreading app
  m_connections.insert(c);
  c->start();
}

void ConnectionManager::stop(ConnectionPtr c) {
  m_connections.erase(c);
  c->stop();
}

void ConnectionManager::stopAll() {
  std::for_each(
    m_connections.begin(), m_connections.end(),
    boost::bind(&Connection::stop, _1)
  );
  m_connections.clear();
}

} // namespace Server
