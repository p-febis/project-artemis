#pragma once

#include "Bowstring/AllocatedData.h"
#include "Bowstring/MeshType.h"
#include "Bowstring/VMA.h"
#include "Bowstring/Window.h"
#include <VkBootstrap.h>
#include <functional>

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

namespace bowstring {

class Mesh;
class Renderer {
public:
  void initialize(Window &window);
  void setClearColor(vk::ClearColorValue clearColor);
  void ensureGraphicsPipeline(MeshType type);
  void renderMesh(vk::CommandBuffer commandBuffer, Mesh &mesh);
  void render(std::function<void(const vk::CommandBuffer &)> renderFunction);
  vk::CommandBuffer startOneTimeSubmit();
  void endOneTimeSubmit(vk::CommandBuffer commandBuffer);
  VmaAllocator getAllocator() { return m_Allocator; };

  ~Renderer();

private:
  uint32_t m_CurrentFrame = 0;

  VmaAllocator m_Allocator;
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

  vk::PipelineLayout m_SimplePipelineLayout;
  vk::Pipeline m_SimplePipeline;

  AllocatedImage m_DepthImage;
  vk::ImageView m_DepthImageView;

  void createSwapchain(uint32_t width, uint32_t height);
  void createDepthResources();
  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();
  void cleanupSwapchain();
  vk::ShaderModule createShaderModule(const std::vector<char> &code);
  void transitionImageLayout(vk::CommandBuffer commandBuffer, vk::Image image,
                             vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout);
  void beginRecordCommandBuffer(vk::CommandBuffer commandBuffer,
                                uint32_t imageIndex);
  void endRecordCommandBuffer(vk::CommandBuffer commandBuffer,
                              uint32_t imageIndex);
  void createSimplePipeline();
  void retrieveQueues();
};

} // namespace bowstring
