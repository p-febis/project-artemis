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
    this->m_Renderer.render([this](vk::CommandBuffer commandBuffer) {
      if (this->m_Mesh.has_value()) {
        this->m_Mesh->bindBuffers(commandBuffer);
        this->m_Mesh->render(commandBuffer);
      }
    });
  });
};

bowstring::Mesh &
bowstring::Application::createMesh(bowstring::MeshType type,
                                   std::vector<Vertex> vertices) {
  this->m_Renderer.ensureGraphicsPipeline(type);
  this->m_Mesh.emplace(m_Renderer, std::move(vertices));
  return *m_Mesh;
};

void bowstring::Application::setClearColor(glm::vec4 clearColor) {
  this->m_Renderer.setClearColor(
      {clearColor[0], clearColor[1], clearColor[2], clearColor[3]});
};

// VIRTUALS
void bowstring::Application::onInit() {};
void bowstring::Application::onUpdate(float) {};
