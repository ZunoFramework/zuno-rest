#include "zuno/RestClient.hpp"
#include <iostream>
#include <unordered_map>

class ExampleInterceptor : public zuno::RequestInterceptor {
public:
  void interceptRequest(
      std::string &url, std::string &method, nlohmann::json &data,
      std::unordered_map<std::string, std::string> &headers) override {
    std::cout << "Intercepting request to URL: " << url << std::endl;
  }
};

class SimpleResponseInterceptor : public zuno::ResponseInterceptor {
public:
  void interceptResponse(std::string &url, std::string &method,
                         nlohmann::json &data,
                         std::unordered_map<std::string, std::string> &headers,
                         zuno::HttpResponse &response) override {
    std::cout << "Response data: " << response.body << std::endl;
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
  zuno::HttpResponse getResponse = client.get(getUrl, headers);

  std::string postUrl = "https://jsonplaceholder.typicode.com/posts";
  nlohmann::json postData = {{"title", "foo"}, {"body", "bar"}, {"userId", 1}};

  std::string putUrl = "https://jsonplaceholder.typicode.com/posts/1";
  nlohmann::json putData = {
      {"id", 1}, {"title", "foo"}, {"body", "bar"}, {"userId", 1}};
  zuno::HttpResponse putResponse = client.put(putUrl, putData, headers);

  std::string patchUrl = "https://jsonplaceholder.typicode.com/posts/1";
  nlohmann::json patchData = {{"title", "foo"}};
  zuno::HttpResponse patchResponse = client.patch(patchUrl, patchData, headers);

  std::string deleteUrl = "https://jsonplaceholder.typicode.com/posts/1";
  zuno::HttpResponse deleteResponse = client.del(deleteUrl, headers);

  std::string headUrl = "https://jsonplaceholder.typicode.com/posts/1";
  zuno::HttpResponse headResponse = client.head(headUrl, headers);

  return 0;
}