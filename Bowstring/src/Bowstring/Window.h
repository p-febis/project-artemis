#pragma once
#include <cstdint>
#include <functional>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace bowstring {
class Window {
private:
  GLFWwindow *m_Window;
  uint32_t m_Width;
  uint32_t m_Height;

public:
  Window(uint32_t width, uint32_t height, const char *title);
  vk::SurfaceKHR produceSurface(vk::Instance instance);
  void mainLoop(const std::function<void()> &update);
  uint32_t getWidth() { return m_Width; };
  uint32_t getHeight() { return m_Height; };
  ~Window();
};
} // namespace bowstring
