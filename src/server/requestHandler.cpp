//@formatter:off

#include "requestHandler.hpp"

#include "reply.hpp"
#include "request.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include <fstream>
#include <iostream>


namespace Server {


RequestHandler::RequestHandler() { }


void RequestHandler::handleRequestGet(const Request& req, Reply& rep) {
   
  // Decode url to path.
  std::string requestPath;
  if (!urlDecode(req.uri, requestPath)) {
    rep = Reply::stockReply(Reply::BadRequest);
    return;
  }
  // TODO: return 404 if path != '/'

  // Fill out the reply to be sent to the client.
  rep.status = Reply::Ok;
  
  rep.content = "<h1>Upload file</h1>"
                "<form method=\"post\" enctype=\"multipart/form-data\">\n"
                "  <input type=\"file\" name=\"formFile\" >\n"
                "  <input type=\"submit\">\n"
                "</form>";
  
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(
                           rep.content.size()
                         );
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
}

void RequestHandler::handleRequestPostBegin(const Request& req) {
  // prepare for handleRequestPostSome
  m_sha1BuilderPtr = boost::make_shared<Crypto::Sha1Builder>();
}

void RequestHandler::handleRequestPostSome(const Request& req) {
  std::string str(req.postChunk, req.postChunk + req.postChunkSize);
  m_sha1BuilderPtr->append(str);
}

void RequestHandler::handleRequestPostEnd(const Request& req, Reply& rep) {
  m_sha1BuilderPtr->end();
  rep.status = Reply::Ok;
  rep.content = m_sha1BuilderPtr->toString();
  m_sha1BuilderPtr.reset();
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(
    rep.content.size()
  );
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/plain";
  
}

bool RequestHandler::urlDecode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else if (in[i] == '+') {
      out += ' ';
    } else {
      out += in[i];
    }
  }
  return true;
}

} // namespace Server
