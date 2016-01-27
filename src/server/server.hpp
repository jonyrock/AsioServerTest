#pragma once 

// #include "connection.hpp"
// #include "connection_manager.hpp"
// #include "request_handler.hpp"

namespace Server {

class Server {

  public:
    Server(short port = 3000);
    // make it noncopyable
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void run();
  
  private:
    short m_port;
  
};

} // namespace Server