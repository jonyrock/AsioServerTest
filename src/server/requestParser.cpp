
#include "requestParser.hpp"
#include "request.hpp"


namespace Server {

RequestParser::RequestParser()
  : m_state(method_start)
{
}

void RequestParser::reset()
{
  m_state = method_start;
}

boost::tribool RequestParser::consume(Request& req, char input)
{
  switch (m_state)
  {
  case method_start:
    if (!isChar(input) || isCtl(input) || isTspecial(input)) {
      return false;
    } else {
      m_state = method;
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case method:
    if (input == ' ') {
      m_state = uri;
      return boost::indeterminate;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
      return false;
    }
    else
    {
      req.method.push_back(input);
      return boost::indeterminate;
    }
  case uri:
    if (input == ' ')
    {
      m_state = http_version_h;
      return boost::indeterminate;
    }
    else if (isCtl(input))
    {
      return false;
    }
    else
    {
      req.uri.push_back(input);
      return boost::indeterminate;
    }
  case http_version_h:
    if (input == 'H')
    {
      m_state = http_version_t_1;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_t_1:
    if (input == 'T')
    {
      m_state = http_version_t_2;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_t_2:
    if (input == 'T')
    {
      m_state = http_version_p;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_p:
    if (input == 'P')
    {
      m_state = http_version_slash;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_slash:
    if (input == '/')
    {
      req.httpVersionMajor = 0;
      req.httpVersionMinor = 0;
      m_state = http_version_major_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_major_start:
    if (isDigit(input))
    {
      req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
      m_state = http_version_major;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_major:
    if (input == '.')
    {
      m_state = http_version_minor_start;
      return boost::indeterminate;
    }
    else if (isDigit(input))
    {
      req.httpVersionMajor = req.httpVersionMajor * 10 + input - '0';
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_minor_start:
    if (isDigit(input))
    {
      req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
      m_state = http_version_minor;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case http_version_minor:
    if (input == '\r')
    {
      m_state = expecting_newline_1;
      return boost::indeterminate;
    }
    else if (isDigit(input))
    {
      req.httpVersionMinor = req.httpVersionMinor * 10 + input - '0';
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case expecting_newline_1:
    if (input == '\n')
    {
      m_state = header_line_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case header_line_start:
    if (input == '\r')
    {
      m_state = expecting_newline_3;
      return boost::indeterminate;
    }
    else if (!req.headers.empty() && (input == ' ' || input == '\t'))
    {
      m_state = header_lws;
      return boost::indeterminate;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return false;
    }
    else
    {
      req.headers.push_back(Header());
      req.headers.back().name.push_back(input);
      m_state = header_name;
      return boost::indeterminate;
    }
  case header_lws:
    if (input == '\r')
    {
      m_state = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (input == ' ' || input == '\t')
    {
      return boost::indeterminate;
    }
    else if (isCtl(input))
    {
      return false;
    }
    else
    {
      m_state = header_value;
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case header_name:
    if (input == ':')
    {
      m_state = space_before_header_value;
      return boost::indeterminate;
    }
    else if (!isChar(input) || isCtl(input) || isTspecial(input))
    {
      return false;
    }
    else
    {
      req.headers.back().name.push_back(input);
      return boost::indeterminate;
    }
  case space_before_header_value:
    if (input == ' ')
    {
      m_state = header_value;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case header_value:
    if (input == '\r')
    {
      m_state = expecting_newline_2;
      return boost::indeterminate;
    }
    else if (isCtl(input))
    {
      return false;
    }
    else
    {
      req.headers.back().value.push_back(input);
      return boost::indeterminate;
    }
  case expecting_newline_2:
    if (input == '\n')
    {
      m_state = header_line_start;
      return boost::indeterminate;
    }
    else
    {
      return false;
    }
  case expecting_newline_3:
    return (input == '\n');
  default:
    return false;
  }
}

bool RequestParser::isChar(int c)
{
  return c >= 0 && c <= 127;
}

bool RequestParser::isCtl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::isTspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool RequestParser::isDigit(int c)
{
  return c >= '0' && c <= '9';
}

} // namespace Server
