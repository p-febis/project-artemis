#include "Bowstring/Application.h"
#include "Bowstring/Mesh.h"
#include <optional>

class HelloTriangle : public bowstring::Application {
  using bowstring::Application::Application;

public:
  void onInit() override {
    this->setClearColor({1.0f, 1.0f, 1.0f, 1.0f});
    const std::vector<bowstring::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

    this->createMesh(bowstring::MeshType::eBasic, vertices);
  }

  void onUpdate(float) override {}
};

int main() {
  auto appConfig = bowstring::ApplicationConfig{};
  appConfig.height = 800;
  appConfig.width = 600;
  appConfig.title = "HelloTriangle";

  HelloTriangle app(appConfig);

  app.run();
}
