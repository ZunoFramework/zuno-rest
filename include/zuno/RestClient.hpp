// include/zuno/RestClient.hpp

#ifndef ZUNO_RESTCLIENT_H
#define ZUNO_RESTCLIENT_H

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace zuno {

struct HttpResponse {
  int statusCode;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
  bool success;

  nlohmann::json json() const;
};

class RestClient {
public:
  RestClient();
  ~RestClient();

  HttpResponse
  get(const std::string &url,
      const std::unordered_map<std::string, std::string> &headers = {});
  HttpResponse
  post(const std::string &url, const nlohmann::json &data,
       const std::unordered_map<std::string, std::string> &headers = {});
  HttpResponse
  put(const std::string &url, const nlohmann::json &data,
      const std::unordered_map<std::string, std::string> &headers = {});
  HttpResponse
  patch(const std::string &url, const nlohmann::json &data,
        const std::unordered_map<std::string, std::string> &headers = {});
  HttpResponse
  del(const std::string &url,
      const std::unordered_map<std::string, std::string> &headers = {});
  HttpResponse
  head(const std::string &url,
       const std::unordered_map<std::string, std::string> &headers = {});

private:
  HttpResponse
  performRequest(const std::string &url, const std::string &method,
                 const nlohmann::json &data,
                 const std::unordered_map<std::string, std::string> &headers);
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
};

} // namespace zuno

#endif // ZUNO_RESTCLIENT_H