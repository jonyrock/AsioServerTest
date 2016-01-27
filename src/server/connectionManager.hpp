
#pragma once

#include <set>
#include <boost/noncopyable.hpp>
#include "connection.hpp"

namespace Server {

/// Manages open connections so that they may be cleanly 
/// stopped when the server needs to shut down.
class ConnectionManager
  : private boost::noncopyable
{
public:
  /// Add the specified connection to the manager and start it.
  void start(ConnectionPtr c);

  /// Stop the specified connection.
  void stop(ConnectionPtr c);

  /// Stop all connections.
  void stopAll();

private:
  /// The managed connections.
  std::set<ConnectionPtr> m_connections;
};

} // namespace Server
