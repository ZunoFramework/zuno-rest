#include "zuno/RestClient.hpp"
#include <iostream>
#include <unordered_map>

// Interceptor de solicitud: muestra la URL y método antes de enviar
class ExampleRequestInterceptor : public zuno::RequestInterceptor {
public:
  void interceptRequest(
      std::string &url, std::string &method, nlohmann::json &data,
      std::unordered_map<std::string, std::string> &headers) override {
    std::cout << "[→ Solicitud] " << method << " " << url << std::endl;
  }
};

// Interceptor de respuesta: muestra la longitud del cuerpo recibido
class ExampleResponseInterceptor : public zuno::ResponseInterceptor {
public:
  void interceptResponse(std::string &url, std::string &method,
                         nlohmann::json &data,
                         std::unordered_map<std::string, std::string> &headers,
                         zuno::HttpResponse &response) override {
    std::cout << "[← Respuesta] Código: " << response.statusCode
              << " | Bytes: " << response.body.size() << std::endl;
  }
};

int main() {
  // Crear cliente y configurar interceptores
  zuno::RestClient client;
  client.setRequestInterceptor(std::make_shared<ExampleRequestInterceptor>());
  client.setResponseInterceptor(std::make_shared<ExampleResponseInterceptor>());

  // Encabezados comunes
  std::unordered_map<std::string, std::string> headers = {
      {"Content-Type", "application/json"}, {"charset", "UTF-8"}};

  // 🔍 URLs
  const std::string baseUrl = "https://jsonplaceholder.typicode.com/posts";
  const std::string resourceUrl = baseUrl + "/1";

  // 📡 Peticiones
  zuno::HttpResponse getResp = client.get(resourceUrl, headers);
  zuno::HttpResponse postResp = client.post(
      baseUrl, {{"title", "foo"}, {"body", "bar"}, {"userId", 1}}, headers);
  zuno::HttpResponse putResp = client.put(
      resourceUrl,
      {{"id", 1}, {"title", "foo"}, {"body", "bar"}, {"userId", 1}}, headers);
  zuno::HttpResponse patchResp =
      client.patch(resourceUrl, {{"title", "foo"}}, headers);
  zuno::HttpResponse deleteResp = client.del(resourceUrl, headers);
  zuno::HttpResponse headResp = client.head(resourceUrl, headers);

  // 📋 Mostrar contenido de respuestas relevantes
  std::cout << "\n[GET Body]\n" << getResp.body << "\n";

  return 0;
}
