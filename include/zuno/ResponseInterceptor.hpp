#ifndef RESPONSE_INTERCEPTOR_HPP
#define RESPONSE_INTERCEPTOR_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace zuno {

struct HttpResponse;

class ResponseInterceptor {
public:
  virtual ~ResponseInterceptor() = default;
  virtual void
  interceptResponse(std::string &url, std::string &method, nlohmann::json &data,
                    std::unordered_map<std::string, std::string> &headers,
                    HttpResponse &response) = 0;
};

} // namespace zuno

#endif // RESPONSE_INTERCEPTOR_HPP