//@formatter:off

#include "connection.hpp"

#include "connectionManager.hpp"
#include "requestHandler.hpp"

#include <boost/bind.hpp>
#include <vector>
#include <iostream>



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

void Connection::handleReadEnd() {
  
  if(m_request.method == "GET") {
    m_requestHandler.handleRequestGet(m_request, m_reply);
  }

  if(m_request.method == "POST") {
    m_requestHandler.handleRequestPostEnd(m_request, m_reply);
  }

  boost::asio::async_write(
    m_socket, m_reply.toBuffers(),
    boost::bind(
      &Connection::handleWrite, shared_from_this(),
      boost::asio::placeholders::error
    )
  );

}

void Connection::handleRead(
  const boost::system::error_code& e,
  std::size_t bytesTransferred
) {
  
  auto pushPostChunk = [this]() {
    if(m_request.method != "POST") {
      return;
    }
    if(m_request.postChunkSize > 0) {
      m_requestHandler.handleRequestPostSome(m_request);
      m_request.postChunkSize = 0;
    }
  }; 
  
  if (!e) {
    boost::tribool result;

    for(size_t i = 0; i < bytesTransferred; i++) {
      std::cout << m_buffer[i] << std::flush;
    }

    boost::tie(result, boost::tuples::ignore) = m_requestParser.parse(
      m_request,
      m_buffer.data(),
      m_buffer.data() + bytesTransferred
    );

    if (result) {
      pushPostChunk();
      handleReadEnd();
    } else if (!result) {
      sendBad();
    } else {
      if(m_request.headerIsReady) {
        // m_request.headerIsReady and indeterminate 
        // could be only if we are in post
        m_requestHandler.handleRequestPostBegin(m_request);
      }
      pushPostChunk();
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

void Connection::sendBad() {
  m_reply = Reply::stockReply(Reply::BadRequest);
  boost::asio::async_write(
    m_socket, m_reply.toBuffers(),
    boost::bind(
      &Connection::handleWrite, shared_from_this(),
      boost::asio::placeholders::error
    )
  );
}


} // namespace server
