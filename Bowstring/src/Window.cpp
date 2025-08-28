#include "Bowstring/pch.hpp"
#include "Bowstring/Window.h"
#include "Bowstring/Logging.h"
#include <stdexcept>

bowstring::Window::Window(uint32_t width, uint32_t height, const char *title)
    : m_Width(width), m_Height(height) {
  if (!glfwInit() || !glfwVulkanSupported()) {
    throw std::runtime_error(
        "Failed to start GLFW or it does not support Vulkan");
  }

  BS_LOG_DEBUG("Creating Window(title=\"{}\", width={}, height={})", title,
               height, width);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  this->m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

vk::SurfaceKHR bowstring::Window::produceSurface(vk::Instance instance) {
  VkSurfaceKHR surface;

  VkResult result =
      glfwCreateWindowSurface(instance, this->m_Window, nullptr, &surface);

  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Surface");
  }

  return surface;
};
void bowstring::Window::mainLoop(const std::function<void()> &update) {
  while (!glfwWindowShouldClose(m_Window)) {
    glfwPollEvents();
    update();
  }
}

bowstring::Window::~Window() {
  glfwDestroyWindow(this->m_Window);
  glfwTerminate();
}
