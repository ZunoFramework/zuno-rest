#include "zuno/RestClient.hpp"
#include <future>
#include <iostream>
#include <unordered_map>

class ExampleInterceptor : public zuno::RequestInterceptor {
public:
  void interceptRequest(
      std::string &url, std::string &method, nlohmann::json &data,
      std::unordered_map<std::string, std::string> &headers) override {
    std::cout << "[Intercept] → " << method << " " << url << std::endl;
  }
};

class SimpleResponseInterceptor : public zuno::ResponseInterceptor {
public:
  void interceptResponse(std::string &url, std::string &method,
                         nlohmann::json &data,
                         std::unordered_map<std::string, std::string> &headers,
                         zuno::HttpResponse &response) override {
    std::cout << "[Response] ← " << response.statusCode
              << " bytes: " << response.body.size() << std::endl;
  }
};

int main() {
  zuno::RestClient client;

  auto reqInterceptor = std::make_shared<ExampleInterceptor>();
  client.setRequestInterceptor(reqInterceptor);

  auto resInterceptor = std::make_shared<SimpleResponseInterceptor>();
  client.setResponseInterceptor(resInterceptor);

  std::unordered_map<std::string, std::string> headers = {
      {"Content-Type", "application/json"}, {"charset", "UTF-8"}};

  std::string getUrl = "https://jsonplaceholder.typicode.com/posts/1";
  std::string postUrl = "https://jsonplaceholder.typicode.com/posts";
  std::string putUrl = "https://jsonplaceholder.typicode.com/posts/1";
  std::string patchUrl = "https://jsonplaceholder.typicode.com/posts/1";
  std::string deleteUrl = "https://jsonplaceholder.typicode.com/posts/1";
  std::string headUrl = "https://jsonplaceholder.typicode.com/posts/1";

  nlohmann::json postData = {{"title", "foo"}, {"body", "bar"}, {"userId", 1}};
  nlohmann::json putData = {
      {"id", 1}, {"title", "foo"}, {"body", "bar"}, {"userId", 1}};
  nlohmann::json patchData = {{"title", "foo"}};

  // Disparar peticiones async
  auto getFuture = client.getAsync(getUrl, headers);
  auto postFuture = client.postAsync(postUrl, postData, headers);
  auto putFuture = client.putAsync(putUrl, putData, headers);
  auto patchFuture = client.patchAsync(patchUrl, patchData, headers);
  auto deleteFuture = client.delAsync(deleteUrl, headers);
  auto headFuture = client.headAsync(headUrl, headers);

  // Esperar resultados
  zuno::HttpResponse getResp = getFuture.get();
  std::cout << "[GET Body]\n" << getResp.body << "\n\n";

  zuno::HttpResponse postResp = postFuture.get();
  std::cout << "[POST Status] " << postResp.statusCode << "\n";

  zuno::HttpResponse putResp = putFuture.get();
  std::cout << "[PUT Status] " << putResp.statusCode << "\n";

  zuno::HttpResponse patchResp = patchFuture.get();
  std::cout << "[PATCH Status] " << patchResp.statusCode << "\n";

  zuno::HttpResponse deleteResp = deleteFuture.get();
  std::cout << "[DELETE Status] " << deleteResp.statusCode << "\n";

  zuno::HttpResponse headResp = headFuture.get();
  std::cout << "[HEAD Status] " << headResp.statusCode << "\n";

  return 0;
}
