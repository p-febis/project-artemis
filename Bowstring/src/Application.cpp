#include "Bowstring/Application.h"
#include "Bowstring/Logging.h"
#include "Bowstring/Mesh.h"

bowstring::Application::Application(ApplicationConfig &config)
    : m_Config(config),
      m_Window(m_Config.width, m_Config.height, m_Config.title) {
  m_Renderer.initialize(this->m_Window);
}

void bowstring::Application::run() {
  this->onInit();
  BS_LOG_DEBUG("Starting Application...");
  this->m_Window.mainLoop([this] {
    auto view = this->m_Registry.view<Mesh>();
    this->m_Renderer.render([&](vk::CommandBuffer commandBuffer) {
      for (auto entity : view) {
        auto &mesh = view.get<Mesh>(entity);
        mesh.bindBuffers(commandBuffer);
        mesh.render(commandBuffer);
      }
    });
  });
};

void bowstring::Application::createMesh(bowstring::MeshType type,
                                        const std::vector<Vertex>& vertices) {
  const auto entity = this->m_Registry.create();
  this->m_Registry.emplace<Mesh>(entity, m_Renderer, std::move(vertices));
  this->m_Renderer.ensureGraphicsPipeline(type);
};

void bowstring::Application::createMesh(bowstring::MeshType type,
                                        const std::vector<Vertex> &vertices,
                                        const std::vector<uint32_t> &indices) {
  const auto entity = this->m_Registry.create();
  this->m_Registry.emplace<Mesh>(entity, m_Renderer, std::move(vertices),
                                 std::move(indices));
  this->m_Renderer.ensureGraphicsPipeline(type);
};

void bowstring::Application::setClearColor(glm::vec4 clearColor) {
  this->m_Renderer.setClearColor(
      {clearColor[0], clearColor[1], clearColor[2], clearColor[3]});
};

// VIRTUALS
void bowstring::Application::onInit() {};
void bowstring::Application::onUpdate(float) {};
