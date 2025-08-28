// example/example_stream.cpp
#include "zuno/RestClient.hpp"
#include <iostream>
#include <string>

int main() {
  // Create a REST client
  zuno::RestClient client;
  
  std::cout << "Demonstrating streaming API..." << std::endl;
  
  // Example 1: Synchronous streaming request
  std::cout << "\nSynchronous streaming GET request:" << std::endl;
  
  // Define a callback function to process stream chunks
  auto streamCallback = [](const zuno::StreamChunk& chunk) {
    if (!chunk.isLast) {
      std::cout << "Received chunk: " << chunk.data << std::endl;
    } else {
      std::cout << "Stream completed." << std::endl;
    }
  };
  
  // Make a streaming GET request
  zuno::BaseResponse response = client.getStream(
    "https://jsonplaceholder.typicode.com/posts", 
    streamCallback
  );
  
  std::cout << "Response status code: " << response.statusCode << std::endl;
  std::cout << "Response success: " << (response.success ? "true" : "false") << std::endl;
  
  // Example 2: Asynchronous streaming request
  std::cout << "\nAsynchronous streaming POST request:" << std::endl;
  
  // Create some JSON data for the POST request
  nlohmann::json postData = {
    {"title", "Streaming test"},
    {"body", "This is a test of the streaming API"},
    {"userId", 1}
  };
  
  // Make an asynchronous streaming POST request
  auto futureResponse = client.postStreamAsync(
    "https://jsonplaceholder.typicode.com/posts",
    postData,
    [](const zuno::StreamChunk& chunk) {
      if (!chunk.isLast) {
        std::cout << "Async received chunk: " << chunk.data << std::endl;
      } else {
        std::cout << "Async stream completed." << std::endl;
      }
    }
  );
  
  // Wait for the asynchronous request to complete
  zuno::BaseResponse asyncResponse = futureResponse.get();
  
  std::cout << "Async response status code: " << asyncResponse.statusCode << std::endl;
  std::cout << "Async response success: " << (asyncResponse.success ? "true" : "false") << std::endl;
  
  return 0;
}