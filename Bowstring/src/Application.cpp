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

  this->m_World.system<MeshComponent>().each([this](
                                                 MeshComponent &meshContainer) {
    this->m_Renderer.render([meshContainer](vk::CommandBuffer commandBuffer) {
      if (!meshContainer.mesh)
        return;
      meshContainer.mesh->bindBuffers(commandBuffer);
      meshContainer.mesh->render(commandBuffer);
    });
  });

  this->m_Window.mainLoop([this] { this->m_World.progress(); });
};

void bowstring::Application::createMesh(bowstring::MeshType type,
                                        const std::vector<Vertex> &vertices) {
  const auto entity = this->m_World.entity();
  entity.set(MeshComponent{std::make_shared<Mesh>(&m_Renderer, vertices)});
  this->m_Renderer.ensureGraphicsPipeline(type);
};

void bowstring::Application::createMesh(bowstring::MeshType type,
                                        const std::vector<Vertex> &vertices,
                                        const std::vector<uint32_t> &indices) {
  const auto entity = this->m_World.entity();
  entity.set(
      MeshComponent{std::make_shared<Mesh>(&m_Renderer, vertices, indices)});
  this->m_Renderer.ensureGraphicsPipeline(type);
};

void bowstring::Application::setClearColor(glm::vec4 clearColor) {
  this->m_Renderer.setClearColor(
      {clearColor[0], clearColor[1], clearColor[2], clearColor[3]});
};

// VIRTUALS
void bowstring::Application::onInit() {};
void bowstring::Application::onUpdate(float) {};
