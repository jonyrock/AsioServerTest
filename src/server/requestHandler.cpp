#include "requestHandler.hpp"
#include "mimeTypes.hpp"
#include "reply.hpp"
#include "request.hpp"

#include <boost/lexical_cast.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


namespace Server {
  

RequestHandler::RequestHandler(const std::string& docRoot) { }

void RequestHandler::handleRequest(const Request& req, Reply& rep) {
   
  // Decode url to path.
  std::string requestPath;
  if (!urlDecode(req.uri, requestPath)) {
    rep = Reply::stockReply(Reply::BadRequest);
    return;
  }
  
  std::cout << "requestPath: " << requestPath << std::endl;

  // Fill out the reply to be sent to the client.
  rep.status = Reply::Ok;
  
  rep.content = "<h1>Hi, bro!</h1>";
  
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(
                           rep.content.size()
                         );
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
}

bool RequestHandler::urlDecode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace Server
