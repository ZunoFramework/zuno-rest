#include "zuno/RestClient.hpp"
#include <algorithm>
#include <curl/curl.h>
#include <iostream>

namespace zuno {

RestClient::RestClient() { curl_global_init(CURL_GLOBAL_DEFAULT); }

RestClient::~RestClient() { curl_global_cleanup(); }

HttpResponse RestClient::performRequest(
    const std::string &url, const std::string &method,
    const nlohmann::json &data,
    const std::unordered_map<std::string, std::string> &headers) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  HttpResponse response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string dataStr;
    if (method != "GET" && method != "HEAD" && method != "DELETE") {
      dataStr = data.dump();
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataStr.c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, dataStr.size());
    }

    if (method != "GET") {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    }
    if (method == "HEAD") {
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    struct curl_slist *chunk = nullptr;
    for (const auto &header : headers) {
      std::string headerStr = header.first + ": " + header.second;
      chunk = curl_slist_append(chunk, headerStr.c_str());
    }

    if (method != "GET" && method != "HEAD") {
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
  }

  return response;
}

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

size_t RestClient::WriteCallback(void *contents, size_t size, size_t nmemb,
                                 void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

nlohmann::json HttpResponse::json() const {
  return nlohmann::json::parse(body);
}

} // namespace zuno