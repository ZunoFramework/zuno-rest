# zuno-rest

`zuno-rest` is a C++ library for making HTTP requests with support for both synchronous and asynchronous operations. It leverages `libcurl` for HTTP operations and `nlohmann/json` for JSON handling.

## Requirements

- CMake 3.14 or higher
- C++20 compatible compiler
- `libcurl`
- `nlohmann/json`
- `libpsl` (Public Suffix List library)

## Installation

To build the project, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/ZunoFrameworkd/zuno-rest.git
   cd zuno-rest
   ```

2. Create a build directory and navigate into it:
   ```sh
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```sh
   cmake ..
   ```

4. Build the project:
   ```sh
   cmake --build .
   ```

## Usage

### Synchronous Example

The following example demonstrates how to make a synchronous HTTP request using `zuno-rest`:

```cpp
#include <zuno/RestClient.hpp>
#include <iostream>

int main() {
    zuno::RestClient client;
    auto response = client.get("https://api.example.com/data");

    if (response.status_code == 200) {
        std::cout << "Response: " << response.body << std::endl;
    } else {
        std::cerr << "Error: " << response.status_code << std::endl;
    }

    return 0;
}
```

### Asynchronous Example

The following example demonstrates how to make asynchronous HTTP requests using `zuno-rest`, including the use of request and response interceptors:

```cpp
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

  // Trigger async requests
  auto getFuture = client.getAsync(getUrl, headers);
  auto postFuture = client.postAsync(postUrl, postData, headers);
  auto putFuture = client.putAsync(putUrl, putData, headers);
  auto patchFuture = client.patchAsync(patchUrl, patchData, headers);
  auto deleteFuture = client.delAsync(deleteUrl, headers);
  auto headFuture = client.headAsync(headUrl, headers);

  // Wait for results
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
```

## Contributing

Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes.
4. Push your branch and open a pull request.

## License

This project is licensed under the AGPL-3.0 License. See the [LICENSE](LICENSE) file for details.