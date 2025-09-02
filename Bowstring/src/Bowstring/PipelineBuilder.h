#pragma once

#include "Bowstring/pch.hpp"
#include <cstdint>
#include <vector>

namespace bowstring {
class PipelineBuilder {
public:
  struct Result {
    VkPipelineLayout layout;
    VkPipeline pipeline;
  };

  PipelineBuilder();

  PipelineBuilder &setShader(vk::ShaderStageFlagBits shaderStage,
                             vk::ShaderModule shaderModule);
  PipelineBuilder &setTopology(vk::PrimitiveTopology topology);
  PipelineBuilder &setDynamicStates(const vk::DynamicState *pDynamicStates,
                                    uint32_t dynamicStatesCount);
  PipelineBuilder &setBindingDescriptions(
      vk::VertexInputBindingDescription *pBindingDescriptions,
      uint32_t bindingDescriptionsCount);
  PipelineBuilder &setVertexAttributeDescriptions(
      vk::VertexInputAttributeDescription *pAttributeDescriptions,
      uint32_t attributeDescriptionsCount);
  PipelineBuilder &setColorAttachments(vk::Format *pColorAttachmentFormats,
                                       uint32_t colorAttachmentCount);

  Result build(vk::Device &device);

private:
  vk::PipelineViewportStateCreateInfo m_ViewportStateCreateInfo;
  vk::PipelineInputAssemblyStateCreateInfo m_InputAssemblyCreateInfo;
  vk::PipelineVertexInputStateCreateInfo m_VertexInputCreateInfo{};
  vk::PipelineRasterizationStateCreateInfo m_RasterizationStateCreateInfo;
  vk::PipelineMultisampleStateCreateInfo m_MultiSampingStateCreateInfo;
  vk::PipelineColorBlendAttachmentState m_ColorBlendAttachmentState;
  vk::PipelineColorBlendStateCreateInfo m_ColorBlendingStateCreateInfo;
  vk::PipelineRenderingCreateInfo m_PiplineRenderingCreateInfo;
  vk::PipelineLayoutCreateInfo m_PipelineLayoutCreateInfo;

  vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo;

  std::vector<vk::PipelineShaderStageCreateInfo> m_ShaderStageCreateInfos;
};
} // namespace bowstring
