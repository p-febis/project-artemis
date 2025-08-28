// The Bowstring Vulkan Renderer
// Refrences:
//   - https://vulkan-tutorial.com/
//   - https://vkguide.dev/
//   - https://github.com/charles-lunarg/vk-bootstrap

#include "Bowstring/Renderer.h"
#include "Bowstring/Logging.h"
#include "VkBootstrap.h"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_enums.hpp>

void bowstring::Renderer::initialize(bowstring::Window &window) {
  this->m_InstanceContainer = vkb::InstanceBuilder()
                                  .set_app_name("Example Vulkan App")
                                  .request_validation_layers()
                                  .require_api_version(1, 3, 0)
                                  .build()
                                  .value();

  BS_LOG_DEBUG("Created Vulkan Instance");

  this->m_Instance = this->m_InstanceContainer.instance;
  this->m_Surface = window.produceSurface(this->m_Instance);

  vk::PhysicalDeviceVulkan13Features features{};
  features.dynamicRendering = true;

  vkb::PhysicalDevice physicalDevice =
      vkb::PhysicalDeviceSelector(this->m_InstanceContainer)
          .set_required_features_13(features)
          .set_surface(this->m_Surface)
          .select()
          .value();

  this->m_DeviceContainer = vkb::DeviceBuilder(physicalDevice).build().value();
  this->m_Device = this->m_DeviceContainer.device;

  BS_LOG_DEBUG("Created Vulkan Instance: {}", (void *)this->m_Instance);
  BS_LOG_DEBUG("Created Surface: {}", (void *)this->m_Surface);
  BS_LOG_DEBUG("Selected Physical Device: {}",
               (void *)physicalDevice.physical_device);
  BS_LOG_DEBUG("Created Logical Device: {}", (void *)this->m_Device);

  this->createSwapchain(window.getWidth(), window.getHeight());
  this->retrieveQueues();
  this->createCommandPool();
  this->createCommandBuffers();
  this->createSyncObjects();
}

void bowstring::Renderer::createSyncObjects() {
  this->m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  this->m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
  this->m_RenderFinishedSemaphores.resize(
      this->m_SwapchainContainer.image_count);
  this->m_ImageInFlight.resize(this->m_SwapchainContainer.image_count,
                               VK_NULL_HANDLE);

  vk::SemaphoreCreateInfo semaphoreCreateInfo{};
  vk::FenceCreateInfo fenceCreateInfo{};

  fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

  for (uint32_t i = 0; i < this->m_SwapchainContainer.image_count; i++) {
    this->m_RenderFinishedSemaphores[i] =
        this->m_Device.createSemaphore(semaphoreCreateInfo);
  }

  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    this->m_ImageAvailableSemaphores[i] =
        this->m_Device.createSemaphore(semaphoreCreateInfo);
    this->m_InFlightFences[i] = this->m_Device.createFence(fenceCreateInfo);
  }

  BS_LOG_DEBUG("Created ImageAvailableSemaphores[{}]",
               this->m_ImageAvailableSemaphores.size());
  BS_LOG_DEBUG("Created RenderFinishedSemaphores[{}]",
               this->m_RenderFinishedSemaphores.size());
  BS_LOG_DEBUG("Created InFlightFences[{}]", this->m_InFlightFences.size());
}

void bowstring::Renderer::createCommandPool() {
  vk::CommandPoolCreateInfo commandPoolCreateInfo = {};
  commandPoolCreateInfo.flags =
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
  commandPoolCreateInfo.queueFamilyIndex =
      this->m_DeviceContainer.get_queue_index(vkb::QueueType::graphics).value();

  this->m_CommandPool =
      this->m_Device.createCommandPool(commandPoolCreateInfo, nullptr);
  BS_LOG_DEBUG("Created Command Pool: {}", (void *)this->m_CommandPool);
};

void bowstring::Renderer::createCommandBuffers() {
  this->m_CommandBuffers.resize(this->m_SwapchainContainer.image_count);

  vk::CommandBufferAllocateInfo allocationInfo{};
  allocationInfo.commandPool = this->m_CommandPool;
  allocationInfo.level = vk::CommandBufferLevel::ePrimary;
  allocationInfo.commandBufferCount = this->m_CommandBuffers.size();

  this->m_CommandBuffers =
      this->m_Device.allocateCommandBuffers(allocationInfo);
  BS_LOG_DEBUG("Created Command Buffers[{}]", this->m_CommandBuffers.size());
}

void bowstring::Renderer::createSwapchain(uint32_t width, uint32_t height) {
  vkb::SwapchainBuilder swapchainBuilder(this->m_DeviceContainer);

  auto swapChainResult =
      swapchainBuilder.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
          .set_desired_extent(width, height)
          .set_old_swapchain(this->m_SwapchainContainer)
          .build();

  if (!swapChainResult) {
    throw std::runtime_error("Failed to create the swapchain");
  }

  vkb::destroy_swapchain(this->m_SwapchainContainer);

  this->m_SwapchainContainer = swapChainResult.value();

  for (auto image : this->m_SwapchainContainer.get_images().value()) {
    this->m_SwapchainImages.push_back(image);
  }
  for (auto imageView : this->m_SwapchainContainer.get_image_views().value()) {
    this->m_SwapchainImageViews.push_back(imageView);
  }

  BS_LOG_DEBUG("Created Swapchain with {} images",
               this->m_SwapchainImages.size(),
               this->m_SwapchainImageViews.size());

  BS_LOG_DEBUG("Destroyed old Swapchain");
};

void bowstring::Renderer::cleanupSwapchain() {
  for (auto imageView : this->m_SwapchainImageViews) {
    this->m_Device.destroyImageView(imageView);
  }
  vkb::destroy_swapchain(this->m_SwapchainContainer);
};

void bowstring::Renderer::retrieveQueues() {
  auto graphicsQueueResult =
      this->m_DeviceContainer.get_queue(vkb::QueueType::graphics);
  auto presentQueueResult =
      this->m_DeviceContainer.get_queue(vkb::QueueType::present);

  if (!graphicsQueueResult || !presentQueueResult) {
    throw std::runtime_error("Failed to retrieve queues");
  }

  this->m_GraphicsQueue = graphicsQueueResult.value();
  this->m_PresentQueue = presentQueueResult.value();

  BS_LOG_DEBUG("Graphics Queue: {}", (void *)this->m_GraphicsQueue);
  BS_LOG_DEBUG("Present Queue: {}", (void *)this->m_PresentQueue);
};

void bowstring::Renderer::setClearColor(vk::ClearColorValue color) {
  this->m_ClearColor = color;
};

void bowstring::Renderer::transitionImageToPresent(
    vk::CommandBuffer commandBuffer, uint32_t imageIndex) {

  vk::ImageMemoryBarrier imageMemoryBarrier{};
  imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
  imageMemoryBarrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
  imageMemoryBarrier.newLayout = vk::ImageLayout::ePresentSrcKHR;
  imageMemoryBarrier.image = this->m_SwapchainImages[imageIndex];

  vk::ImageSubresourceRange subresourceRange = {};
  subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  subresourceRange.levelCount = 1;
  subresourceRange.layerCount = 1;
  imageMemoryBarrier.subresourceRange = subresourceRange;

  commandBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eBottomOfPipe,
      vk::DependencyFlagBits::eByRegion, 0, nullptr, 0, nullptr, 1,
      &imageMemoryBarrier);
}

void bowstring::Renderer::transitionImageToOptimal(
    vk::CommandBuffer commandBuffer, uint32_t imageIndex) {

  vk::ImageMemoryBarrier imageMemoryBarrier{};
  imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
  imageMemoryBarrier.oldLayout = vk::ImageLayout::eUndefined;
  imageMemoryBarrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
  imageMemoryBarrier.image = this->m_SwapchainImages[imageIndex];

  vk::ImageSubresourceRange subresourceRange = {};
  subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  subresourceRange.levelCount = 1;
  subresourceRange.layerCount = 1;
  imageMemoryBarrier.subresourceRange = subresourceRange;

  commandBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eTopOfPipe,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::DependencyFlagBits::eByRegion, 0, nullptr, 0, nullptr, 1,
      &imageMemoryBarrier);
}

void bowstring::Renderer::beginRecordCommandBuffer(
    vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
  vk::CommandBufferBeginInfo beginInfo{};

  auto result = commandBuffer.begin(&beginInfo);
  if (result != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to begin commandBuffer recording");
  };

  this->transitionImageToOptimal(commandBuffer, imageIndex);

  vk::RenderingAttachmentInfo colorAttachmentInfo{};
  colorAttachmentInfo.imageView = this->m_SwapchainImageViews[imageIndex];
  colorAttachmentInfo.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
  colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachmentInfo.clearValue = this->m_ClearColor;

  vk::RenderingInfo renderingInfo{};
  renderingInfo.layerCount = 1;
  renderingInfo.renderArea.offset = vk::Offset2D(0, 0);
  renderingInfo.renderArea.extent = this->m_SwapchainContainer.extent;
  renderingInfo.colorAttachmentCount = 1;
  renderingInfo.pColorAttachments = &colorAttachmentInfo;

  commandBuffer.beginRendering(renderingInfo);
};

void bowstring::Renderer::endRecordCommandBuffer(
    vk::CommandBuffer commandBuffer, uint32_t imageIndex) {
  commandBuffer.endRendering();
  this->transitionImageToPresent(commandBuffer, imageIndex);
  commandBuffer.end();
}

void bowstring::Renderer::render(
    std::function<void(const vk::CommandBuffer &)> renderFunction) {
  auto resultWait = this->m_Device.waitForFences(
      this->m_InFlightFences[this->m_CurrentFrame], VK_TRUE, UINT64_MAX);

  if (resultWait != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to wait for in flight fence");
  }

  uint32_t imageIndex = 0;

  auto nextImageResult = this->m_Device.acquireNextImageKHR(
      this->m_SwapchainContainer.swapchain, UINT64_MAX,
      this->m_ImageAvailableSemaphores[this->m_CurrentFrame], VK_NULL_HANDLE,
      &imageIndex);

  if (nextImageResult == vk::Result::eErrorOutOfDateKHR) {
    // TODO: Recreate Swapchain
    // return;
  }

  if (nextImageResult != vk::Result::eSuccess &&
      nextImageResult != vk::Result::eSuboptimalKHR) {
    BS_LOG_DEBUG("nextImageResult: {}", static_cast<int>(nextImageResult));
    throw std::runtime_error("Failed to retrieve next image");
  }

  if (this->m_ImageInFlight[imageIndex] != VK_NULL_HANDLE) {
    resultWait = this->m_Device.waitForFences(
        this->m_ImageInFlight[this->m_CurrentFrame], VK_TRUE, UINT64_MAX);

    if (resultWait != vk::Result::eSuccess) {
      throw std::runtime_error("Failed to wait for imageInFlight fence");
    }
  }
  this->m_ImageInFlight[imageIndex] =
      this->m_InFlightFences[this->m_CurrentFrame];

  auto resultReset = this->m_Device.resetFences(
      1, &this->m_InFlightFences[this->m_CurrentFrame]);

  if (resultReset != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to reset inFlightFence");
  }

  this->m_CommandBuffers[this->m_CurrentFrame].reset();
  this->beginRecordCommandBuffer(this->m_CommandBuffers[this->m_CurrentFrame],
                                 imageIndex);
  renderFunction(this->m_CommandBuffers[this->m_CurrentFrame]);
  this->endRecordCommandBuffer(this->m_CommandBuffers[this->m_CurrentFrame],
                               imageIndex);

  vk::SubmitInfo submitInfo{};
  std::array<vk::Semaphore, 1> waitSemaphores{
      this->m_ImageAvailableSemaphores[this->m_CurrentFrame]};
  std::array<vk::PipelineStageFlags, 1> waitStages{
      vk::PipelineStageFlagBits::eColorAttachmentOutput};

  submitInfo.waitSemaphoreCount = waitSemaphores.size();
  submitInfo.pWaitSemaphores = waitSemaphores.data();
  submitInfo.pWaitDstStageMask = waitStages.data();

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &this->m_CommandBuffers[this->m_CurrentFrame];

  std::array<vk::Semaphore, 1> signalSemaphores{
      this->m_ImageAvailableSemaphores[this->m_CurrentFrame]};

  submitInfo.signalSemaphoreCount = signalSemaphores.size();
  submitInfo.pSignalSemaphores = signalSemaphores.data();

  this->m_GraphicsQueue.submit(submitInfo,
                               this->m_InFlightFences[this->m_CurrentFrame]);

  vk::PresentInfoKHR presentInfo{};
  presentInfo.waitSemaphoreCount = waitSemaphores.size();
  presentInfo.pWaitSemaphores = waitSemaphores.data();
  presentInfo.swapchainCount = 1;

  auto swapchain = vk::SwapchainKHR(this->m_SwapchainContainer.swapchain);
  presentInfo.pSwapchains = &swapchain;
  presentInfo.pImageIndices = &imageIndex;

  auto presentResult = this->m_PresentQueue.presentKHR(&presentInfo);
  if (presentResult != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to submit to presentQueue");
  }

  this->m_Device.waitIdle();
  this->m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
};

bowstring::Renderer::~Renderer() {

  for (uint32_t i = 0; i < this->m_SwapchainContainer.image_count; i++) {
    this->m_Device.destroySemaphore(this->m_RenderFinishedSemaphores[i]);
  }
  for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    this->m_Device.destroySemaphore(this->m_ImageAvailableSemaphores[i]);
    this->m_Device.destroyFence(this->m_InFlightFences[i]);
  }

  this->m_Device.destroyCommandPool(this->m_CommandPool);
  this->cleanupSwapchain();
  vkb::destroy_device(this->m_DeviceContainer);
  this->m_Instance.destroySurfaceKHR(this->m_Surface);
  vkb::destroy_instance(this->m_InstanceContainer);
};
