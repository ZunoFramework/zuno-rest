// include/zuno/RequestInterceptor.hpp
#ifndef REQUEST_INTERCEPTOR_HPP
#define REQUEST_INTERCEPTOR_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace zuno {

class RequestInterceptor {
public:
  virtual ~RequestInterceptor() = default;
  virtual void
  interceptRequest(std::string &url, std::string &method, nlohmann::json &data,
                   std::unordered_map<std::string, std::string> &headers) = 0;
};

} // namespace zuno

#endif // REQUEST_INTERCEPTOR_HPP