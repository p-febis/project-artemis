#pragma once

#include "Window.h"
#include "Renderer.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace bowstring {
struct ApplicationConfig {
  uint32_t width = 0;
  uint32_t height = 0;
  const char *title;
};

class Application {
private:
  ApplicationConfig m_Config;
  Window m_Window;
  Renderer m_Renderer;

public:
  Application(ApplicationConfig &config);
  void setClearColor(glm::vec4 clearColor);
  void run();

  virtual void onInit();
  virtual void onUpdate(float deltaTime);
};
} // namespace bowstring
