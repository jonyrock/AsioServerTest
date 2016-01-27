#include "server.hpp"

#include "connection.hpp"
#include "connectionManager.hpp"
#include "requestHandler.hpp"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

#include <signal.h>
#include <iostream>


using namespace std;


namespace Server {

struct ServerImpl : public Server {

  short m_port;
  boost::asio::io_service m_ioService;
  boost::asio::signal_set m_signals;
  boost::asio::ip::tcp::acceptor m_acceptor;
  ConnectionManager m_connectionManager;
  ConnectionPtr m_newConnection;
  RequestHandler m_requestHandler;

  ServerImpl(short port) :
    m_port(port),
    m_ioService(),
    m_signals(m_ioService),
    m_acceptor(m_ioService),
    m_connectionManager(),
    m_newConnection(),
    m_requestHandler(".")
  {
    m_signals.add(SIGINT);
    m_signals.add(SIGTERM);
#if defined(SIGQUIT)
    m_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    m_signals.async_wait(boost::bind(&ServerImpl::handleStop, this));
    
    boost::asio::ip::tcp::endpoint endpoint(
      boost::asio::ip::address::from_string("127.0.0.1"), port
    );
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(
      boost::asio::ip::tcp::acceptor::reuse_address(true)
    );
    m_acceptor.bind(endpoint);
    m_acceptor.listen();

    startAccept();

  }
  
  void run() override {
    cout << "server start" << endl;
    // TODO: run in CPU's core number + 1
    m_ioService.run();
  }
  
  void startAccept() {
    m_newConnection.reset(
      new Connection(m_ioService, m_connectionManager, m_requestHandler)
    );
    m_acceptor.async_accept(
      m_newConnection->socket(),
      boost::bind(
        &ServerImpl::handleAccept, this, boost::asio::placeholders::error
      )
    );
  }

  void handleAccept(const boost::system::error_code& e) {
    if (!m_acceptor.is_open()) {
      return;
    }
    if (!e) {
      m_connectionManager.start(m_newConnection);
    }
    startAccept();
  }

  void handleStop() {
    m_acceptor.close();
    m_connectionManager.stopAll();
  }
    
};

Server& Server::getInstance(short port) {
  static ServerImpl instance(port);
  return instance;
}

} // namespace Server