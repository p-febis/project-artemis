#pragma once

#include "Bowstring/Window.h"
#include "Bowstring/pch.hpp"
#include <VkBootstrap.h>
#include <functional>

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

namespace bowstring {
class Renderer {
public:
  void initialize(Window &window);
  void setClearColor(vk::ClearColorValue clearColor);
  void render(std::function<void(const vk::CommandBuffer &)> renderFunction);
  ~Renderer();

private:
  uint32_t m_CurrentFrame = 0;

  vk::ClearColorValue m_ClearColor;
  vkb::Instance m_InstanceContainer;
  vkb::Device m_DeviceContainer;
  vkb::Swapchain m_SwapchainContainer;

  vk::Device m_Device;
  vk::Instance m_Instance;
  vk::SurfaceKHR m_Surface;
  vk::Queue m_GraphicsQueue;
  vk::Queue m_PresentQueue;
  vk::CommandPool m_CommandPool;

  std::vector<vk::Image> m_SwapchainImages;
  std::vector<vk::ImageView> m_SwapchainImageViews;

  std::vector<vk::CommandBuffer> m_CommandBuffers;
  std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
  std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
  std::vector<vk::Fence> m_InFlightFences;
  std::vector<vk::Fence> m_ImageInFlight;

  void createSwapchain(uint32_t width, uint32_t height);
  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();
  void cleanupSwapchain();
  void transitionImageToPresent(vk::CommandBuffer commandBuffer,
                                uint32_t imageIndex);
  void transitionImageToOptimal(vk::CommandBuffer commandBuffer,
                                uint32_t imageIndex);
  void beginRecordCommandBuffer(vk::CommandBuffer commandBuffer,
                                uint32_t imageIndex);
  void endRecordCommandBuffer(vk::CommandBuffer commandBuffer,
                              uint32_t imageIndex);
  void retrieveQueues();
};

} // namespace bowstring
