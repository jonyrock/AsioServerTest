#pragma once

#include <string>

namespace Server {
namespace MimeTypes {

/// Convert a file extension into a MIME type.
std::string extensionToType(const std::string& extension);

} // namespace mimeTypes
} // namespace server
