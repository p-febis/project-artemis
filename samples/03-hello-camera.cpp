#include "Bowstring/Application.h"

class HelloTriangle : public bowstring::Application {
  using bowstring::Application::Application;

public:
  void onInit() override {
    this->setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    const std::vector<bowstring::Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}};

    const std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

    this->createMesh(bowstring::MeshType::eBasic, vertices, indices);
  }

  void onUpdate(float) override {}
};

int main() {
  auto appConfig = bowstring::ApplicationConfig{};
  appConfig.width = 800;
  appConfig.height = 600;
  appConfig.title = "HelloTriangle";

  HelloTriangle app(appConfig);

  app.run();
}
