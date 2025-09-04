#include "Bowstring/Application.h"
#include "Bowstring/Mesh.h"
#include "Bowstring/MeshType.h"
#include <memory>

class HelloTriangle : public bowstring::Application {
  using bowstring::Application::Application;

public:
  void onInit() override {
    this->setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    const std::vector<bowstring::Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

    auto meshEntity = this->createEntity();
    this->getpRenderer()->ensureGraphicsPipeline(bowstring::MeshType::eBasic);
    auto mesh = std::make_shared<bowstring::Mesh>(this->getpRenderer(), vertices);
    meshEntity.addComponent<bowstring::MeshComponent>({mesh});
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
