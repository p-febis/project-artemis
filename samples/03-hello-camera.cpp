#include "Bowstring/Application.h"
#include "Bowstring/Mesh.h"
#include "Bowstring/Vertex.h"
#include <memory>

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
    this->getRenderer()->ensureGraphicsPipeline(bowstring::MeshType::eBasic);

    auto renderingEntity = this->createEntity();
    auto mesh = std::make_shared<bowstring::Mesh>(this->getRenderer(),
                                                  vertices, indices);
    renderingEntity.addComponent<bowstring::MeshComponent>({mesh});
  }

  void onUpdate(float) override {}
};

int main() {
  auto appConfig = bowstring::ApplicationConfig{};
  appConfig.width = 800;
  appConfig.height = 600;
  appConfig.title = "HelloCamera";

  HelloTriangle app(appConfig);

  app.run();
}
