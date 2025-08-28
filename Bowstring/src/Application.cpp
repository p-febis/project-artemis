#include "Bowstring/pch.hpp"
#include "Bowstring/Logging.h"
#include "Bowstring/Application.h"

bowstring::Application::Application(ApplicationConfig &config)
    : m_Config(config),
      m_Window(m_Config.width, m_Config.height, m_Config.title) {
  m_Renderer.initialize(this->m_Window);
}

void bowstring::Application::run() {
  this->onInit();
  BS_LOG_DEBUG("Starting Application...");
  this->m_Window.mainLoop(
      [this] { this->m_Renderer.render([](vk::CommandBuffer) {}); });
};

void bowstring::Application::setClearColor(glm::vec4 clearColor) {
  this->m_Renderer.setClearColor(
      {clearColor[0], clearColor[1], clearColor[2], clearColor[3]});
};

// VIRTUALS
void bowstring::Application::onInit() {};
void bowstring::Application::onUpdate(float) {};
