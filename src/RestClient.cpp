#include "zuno/RestClient.hpp"
#include "zuno/RequestInterceptor.hpp"
#include "zuno/ResponseInterceptor.hpp"
#include <curl/curl.h>
#include <future>
#include <iostream>
#include <memory>

namespace zuno {

RestClient::RestClient() { curl_global_init(CURL_GLOBAL_DEFAULT); }

RestClient::~RestClient() { curl_global_cleanup(); }

void RestClient::setRequestInterceptor(
    std::shared_ptr<RequestInterceptor> requestInterceptor) {
  this->requestInterceptor = requestInterceptor;
}

void RestClient::setResponseInterceptor(
    std::shared_ptr<ResponseInterceptor> responseInterceptor) {
  this->responseInterceptor = responseInterceptor;
}

HttpResponse RestClient::performRequest(
    const std::string &url, const std::string &method,
    const nlohmann::json &data,
    const std::unordered_map<std::string, std::string> &headers) {

  std::string mutableUrl = url;
  std::string mutableMethod = method;
  nlohmann::json mutableData = data;
  std::unordered_map<std::string, std::string> mutableHeaders = headers;

  if (requestInterceptor) {
    requestInterceptor->interceptRequest(mutableUrl, mutableMethod, mutableData,
                                         mutableHeaders);
  }

  CURL *curl = curl_easy_init();
  CURLcode res;
  std::string readBuffer;
  HttpResponse response;

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, mutableUrl.c_str());

    std::string dataStr;
    if (mutableMethod != "GET" && mutableMethod != "HEAD" &&
        mutableMethod != "DELETE") {
      dataStr = mutableData.dump();
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataStr.c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, dataStr.size());
    }

    if (mutableMethod != "GET") {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, mutableMethod.c_str());
    }
    if (mutableMethod == "HEAD") {
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    struct curl_slist *chunk = nullptr;
    for (const auto &header : mutableHeaders) {
      std::string headerStr = header.first + ": " + header.second;
      chunk = curl_slist_append(chunk, headerStr.c_str());
    }

    if (mutableMethod != "GET" && mutableMethod != "HEAD") {
      std::string contentLengthHeader =
          "Content-Length: " + std::to_string(dataStr.size());
      chunk = curl_slist_append(chunk, contentLengthHeader.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    res = curl_easy_perform(curl);

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      response.success = false;
      return response;
    }

    response.statusCode = static_cast<int>(httpCode);
    response.body = readBuffer;
    response.success = (httpCode >= 200 && httpCode < 300);

    if (responseInterceptor) {
      responseInterceptor->interceptResponse(
          mutableUrl, mutableMethod, mutableData, mutableHeaders, response);
    }
  }

  return response;
}

// Síncronos
HttpResponse
RestClient::get(const std::string &url,
                const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "GET", {}, headers);
}

HttpResponse
RestClient::post(const std::string &url, const nlohmann::json &data,
                 const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "POST", data, headers);
}

HttpResponse
RestClient::put(const std::string &url, const nlohmann::json &data,
                const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "PUT", data, headers);
}

HttpResponse
RestClient::patch(const std::string &url, const nlohmann::json &data,
                  const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "PATCH", data, headers);
}

HttpResponse
RestClient::del(const std::string &url,
                const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "DELETE", {}, headers);
}

HttpResponse
RestClient::head(const std::string &url,
                 const std::unordered_map<std::string, std::string> &headers) {
  return performRequest(url, "HEAD", {}, headers);
}

// Asíncronos
std::future<HttpResponse> RestClient::getAsync(
    const std::string &url,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::get, this, url, headers);
}

std::future<HttpResponse> RestClient::postAsync(
    const std::string &url, const nlohmann::json &data,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::post, this, url, data,
                    headers);
}

std::future<HttpResponse> RestClient::putAsync(
    const std::string &url, const nlohmann::json &data,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::put, this, url, data,
                    headers);
}

std::future<HttpResponse> RestClient::patchAsync(
    const std::string &url, const nlohmann::json &data,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::patch, this, url, data,
                    headers);
}

std::future<HttpResponse> RestClient::delAsync(
    const std::string &url,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::del, this, url, headers);
}

std::future<HttpResponse> RestClient::headAsync(
    const std::string &url,
    const std::unordered_map<std::string, std::string> &headers) {
  return std::async(std::launch::async, &RestClient::head, this, url, headers);
}

// Callback de escritura
size_t RestClient::WriteCallback(void *contents, size_t size, size_t nmemb,
                                 void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

// Conversión de body a JSON
nlohmann::json HttpResponse::json() const {
  return nlohmann::json::parse(body);
}

} // namespace zuno
