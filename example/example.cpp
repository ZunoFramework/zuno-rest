#include "zuno/RestClient.hpp"
#include <iostream>
#include <unordered_map>

int main() {
  zuno::RestClient client;

  std::unordered_map<std::string, std::string> headers = {
      {"Content-Type", "application/json"}, {"charset", "UTF-8"}};

  std::string getUrl = "https://jsonplaceholder.typicode.com/posts/1";
  zuno::HttpResponse getResponse = client.get(getUrl, headers);
  std::cout << "GET Response: " << getResponse.body << std::endl;
  if (getResponse.success) {
    nlohmann::json getJson = getResponse.json();
    std::cout << "GET JSON Response: " << getJson.dump(2) << std::endl;
  }

  std::string postUrl = "https://jsonplaceholder.typicode.com/posts";
  nlohmann::json postData = {{"title", "foo"}, {"body", "bar"}, {"userId", 1}};
  std::cout << "POST Data: " << postData.dump() << std::endl;
  zuno::HttpResponse postResponse = client.post(postUrl, postData, headers);
  std::cout << "POST Response: " << postResponse.body << std::endl;
  if (postResponse.success) {
    nlohmann::json postJson = postResponse.json();
    std::cout << "POST JSON Response: " << postJson.dump(2) << std::endl;
  }

  std::string putUrl = "https://jsonplaceholder.typicode.com/posts/1";
  nlohmann::json putData = {
      {"id", 1}, {"title", "foo"}, {"body", "bar"}, {"userId", 1}};
  zuno::HttpResponse putResponse = client.put(putUrl, putData, headers);
  std::cout << "PUT Response: " << putResponse.body << std::endl;
  if (putResponse.success) {
    nlohmann::json putJson = putResponse.json();
    std::cout << "PUT JSON Response: " << putJson.dump(2) << std::endl;
  }

  std::string patchUrl = "https://jsonplaceholder.typicode.com/posts/1";
  nlohmann::json patchData = {{"title", "foo"}};
  zuno::HttpResponse patchResponse = client.patch(patchUrl, patchData, headers);
  std::cout << "PATCH Response: " << patchResponse.body << std::endl;
  if (patchResponse.success) {
    nlohmann::json patchJson = patchResponse.json();
    std::cout << "PATCH JSON Response: " << patchJson.dump(2) << std::endl;
  }

  std::string deleteUrl = "https://jsonplaceholder.typicode.com/posts/1";
  zuno::HttpResponse deleteResponse = client.del(deleteUrl, headers);
  std::cout << "DELETE Response: " << deleteResponse.body << std::endl;

  std::string headUrl = "https://jsonplaceholder.typicode.com/posts/1";
  zuno::HttpResponse headResponse = client.head(headUrl, headers);
  std::cout << "HEAD Response: " << headResponse.body << std::endl;

  return 0;
}