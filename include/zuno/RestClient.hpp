// include/zuno/RestClient.hpp

#ifndef ZUNO_RESTCLIENT_H
#define ZUNO_RESTCLIENT_H

#include "RequestInterceptor.hpp"
#include "ResponseInterceptor.hpp"
#include <functional>
#include <future>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace zuno {

// Base response structure
struct BaseResponse {
  int statusCode;
  std::unordered_map<std::string, std::string> headers;
  bool success;
};

// Standard HTTP response
struct HttpResponse : public BaseResponse {
  std::string body;
  
  nlohmann::json json() const;
};

// Stream chunk for streaming responses
struct StreamChunk {
  std::string data;
  bool isLast;
};

// Callback type for streaming responses
using StreamCallback = std::function<void(const StreamChunk&)>;

// Stream callback structure for internal use
struct StreamCallbackData {
  StreamCallback userCallback;
  bool isFirstChunk;
};

class RestClient {
public:
  RestClient();
  ~RestClient();

  // Métodos síncronos - Standard responses
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
       
  // Métodos síncronos - Streaming responses
  BaseResponse
  getStream(const std::string &url, StreamCallback callback,
            const std::unordered_map<std::string, std::string> &headers = {});
  BaseResponse
  postStream(const std::string &url, const nlohmann::json &data, StreamCallback callback,
             const std::unordered_map<std::string, std::string> &headers = {});
  BaseResponse
  putStream(const std::string &url, const nlohmann::json &data, StreamCallback callback,
            const std::unordered_map<std::string, std::string> &headers = {});
  BaseResponse
  patchStream(const std::string &url, const nlohmann::json &data, StreamCallback callback,
              const std::unordered_map<std::string, std::string> &headers = {});
  BaseResponse
  delStream(const std::string &url, StreamCallback callback,
            const std::unordered_map<std::string, std::string> &headers = {});

  // Métodos asíncronos - Standard responses
  std::future<HttpResponse>
  getAsync(const std::string &url,
           const std::unordered_map<std::string, std::string> &headers = {});
  std::future<HttpResponse>
  postAsync(const std::string &url, const nlohmann::json &data,
            const std::unordered_map<std::string, std::string> &headers = {});
  std::future<HttpResponse>
  putAsync(const std::string &url, const nlohmann::json &data,
           const std::unordered_map<std::string, std::string> &headers = {});
  std::future<HttpResponse>
  patchAsync(const std::string &url, const nlohmann::json &data,
             const std::unordered_map<std::string, std::string> &headers = {});
  std::future<HttpResponse>
  delAsync(const std::string &url,
           const std::unordered_map<std::string, std::string> &headers = {});
  std::future<HttpResponse>
  headAsync(const std::string &url,
            const std::unordered_map<std::string, std::string> &headers = {});
            
  // Métodos asíncronos - Streaming responses
  std::future<BaseResponse>
  getStreamAsync(const std::string &url, StreamCallback callback,
                 const std::unordered_map<std::string, std::string> &headers = {});
  std::future<BaseResponse>
  postStreamAsync(const std::string &url, const nlohmann::json &data, StreamCallback callback,
                  const std::unordered_map<std::string, std::string> &headers = {});
  std::future<BaseResponse>
  putStreamAsync(const std::string &url, const nlohmann::json &data, StreamCallback callback,
                 const std::unordered_map<std::string, std::string> &headers = {});
  std::future<BaseResponse>
  patchStreamAsync(const std::string &url, const nlohmann::json &data, StreamCallback callback,
                   const std::unordered_map<std::string, std::string> &headers = {});
  std::future<BaseResponse>
  delStreamAsync(const std::string &url, StreamCallback callback,
                 const std::unordered_map<std::string, std::string> &headers = {});

  void
  setRequestInterceptor(std::shared_ptr<RequestInterceptor> requestInterceptor);
  void setResponseInterceptor(
      std::shared_ptr<ResponseInterceptor> responseInterceptor);

private:
  HttpResponse
  performRequest(const std::string &url, const std::string &method,
                 const nlohmann::json &data,
                 const std::unordered_map<std::string, std::string> &headers);
                 
  BaseResponse
  performStreamRequest(const std::string &url, const std::string &method,
                       const nlohmann::json &data,
                       const std::unordered_map<std::string, std::string> &headers,
                       StreamCallback callback);

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
                              
  static size_t StreamWriteCallback(void *contents, size_t size, size_t nmemb,
                                    void *userp);

  std::shared_ptr<RequestInterceptor> requestInterceptor;
  std::shared_ptr<ResponseInterceptor> responseInterceptor;
};

} // namespace zuno

#endif // ZUNO_RESTCLIENT_H
