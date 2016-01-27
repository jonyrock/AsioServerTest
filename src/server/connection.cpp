#include "connection.hpp"

#include "connectionManager.hpp"
#include "requestHandler.hpp"

#include <boost/bind.hpp>
#include <vector>



namespace Server {

Connection::Connection(
  boost::asio::io_service& ioService,
  ConnectionManager& manager,
  RequestHandler& handler
) : m_socket(ioService),
    m_connectionManager(manager),
    m_requestHandler(handler)
{ }

boost::asio::ip::tcp::socket& Connection::socket() {
  return m_socket;
}

void Connection::start() {
  m_socket.async_read_some(
    boost::asio::buffer(m_buffer),
    boost::bind(
      &Connection::handleRead, shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void Connection::stop() {
  m_socket.close();
}

void Connection::handleRead(
  const boost::system::error_code& e,
  std::size_t bytesTransferred
) {
  
  // TODO: compute online function here (SHA1)
  if (!e) {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = 
      m_requestParser.parse(
        m_request, m_buffer.data(), m_buffer.data() + bytesTransferred
      );

    if (result) {
      m_requestHandler.handleRequest(m_request, m_reply);
      boost::asio::async_write(
        m_socket, m_reply.toBuffers(),
        boost::bind(
          &Connection::handleWrite, shared_from_this(),
          boost::asio::placeholders::error
        )
      );
    } else if (!result) {
      m_reply = Reply::stockReply(Reply::BadRequest);
      boost::asio::async_write(
        m_socket, m_reply.toBuffers(),
        boost::bind(
          &Connection::handleWrite, shared_from_this(),
          boost::asio::placeholders::error
        )
      );
    } else {
      m_socket.async_read_some(
        boost::asio::buffer(m_buffer),
        boost::bind(
          &Connection::handleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        )
      );
    }
  } else if (e != boost::asio::error::operation_aborted) {
    m_connectionManager.stop(shared_from_this());
  }
}

void Connection::handleWrite(const boost::system::error_code& e) {
  if (!e) {
    // Initiate graceful connection closure.
    boost::system::error_code ignoredEc;
    m_socket.shutdown(
      boost::asio::ip::tcp::socket::shutdown_both, ignoredEc
    );
  }

  if (e != boost::asio::error::operation_aborted) {
    m_connectionManager.stop(shared_from_this());
  }
}

} // namespace server
