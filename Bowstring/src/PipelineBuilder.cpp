#include "Bowstring/PipelineBuilder.h"

bowstring::PipelineBuilder::PipelineBuilder() {
  this->m_ViewportStateCreateInfo.viewportCount = 1;
  this->m_ViewportStateCreateInfo.scissorCount = 1;

  this->m_RasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
  this->m_RasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
  this->m_RasterizationStateCreateInfo.polygonMode = vk::PolygonMode::eFill;
  this->m_RasterizationStateCreateInfo.lineWidth = 1.0f;
  this->m_RasterizationStateCreateInfo.cullMode = vk::CullModeFlagBits::eBack;
  this->m_RasterizationStateCreateInfo.frontFace = vk::FrontFace::eClockwise;
  this->m_RasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;

  this->m_MultiSampingStateCreateInfo.sampleShadingEnable = VK_FALSE;
  this->m_MultiSampingStateCreateInfo.rasterizationSamples =
      vk::SampleCountFlagBits::e1;
  this->m_ColorBlendAttachmentState.colorWriteMask =
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  this->m_ColorBlendAttachmentState.blendEnable = VK_FALSE;

  this->m_ColorBlendingStateCreateInfo.logicOpEnable = VK_FALSE;
  this->m_ColorBlendingStateCreateInfo.attachmentCount = 1;
  this->m_ColorBlendingStateCreateInfo.pAttachments =
      &this->m_ColorBlendAttachmentState;

  this->m_PipelineLayoutCreateInfo.setLayoutCount = 0;
}
bowstring::PipelineBuilder &
bowstring::PipelineBuilder::setDepthAttachmentFormat(vk::Format format) {
  this->m_PiplineRenderingCreateInfo.depthAttachmentFormat = format;
  return *this;
};

bowstring::PipelineBuilder &bowstring::PipelineBuilder::setDepthStencilData(
    bool bDepthTestEnable, bool bDepthWriteEnable, vk::CompareOp compareOp) {
  this->m_DepthStencilCreateInfo.depthTestEnable = bDepthTestEnable;
  this->m_DepthStencilCreateInfo.depthWriteEnable = bDepthWriteEnable;
  this->m_DepthStencilCreateInfo.depthCompareOp = compareOp;
  return *this;
};

bowstring::PipelineBuilder &
bowstring::PipelineBuilder::setShader(vk::ShaderStageFlagBits shaderStage,
                                      vk::ShaderModule shaderModule) {

  vk::PipelineShaderStageCreateInfo shaderStageCreateInfo{};
  shaderStageCreateInfo.stage = shaderStage;
  shaderStageCreateInfo.module = shaderModule;
  shaderStageCreateInfo.pName = "main";

  this->m_ShaderStageCreateInfos.push_back(shaderStageCreateInfo);

  return *this;
};

bowstring::PipelineBuilder &bowstring::PipelineBuilder::setDynamicStates(
    const vk::DynamicState *pDynamicStates, uint32_t dynamicStatesCount) {
  m_DynamicStateCreateInfo.dynamicStateCount = dynamicStatesCount;
  m_DynamicStateCreateInfo.pDynamicStates = pDynamicStates;

  return *this;
};

bowstring::PipelineBuilder &bowstring::PipelineBuilder::setBindingDescriptions(
    vk::VertexInputBindingDescription *pBindingDescriptions,
    uint32_t bindingDescriptionsCount) {
  this->m_VertexInputCreateInfo.vertexBindingDescriptionCount =
      bindingDescriptionsCount;
  this->m_VertexInputCreateInfo.pVertexBindingDescriptions =
      pBindingDescriptions;

  return *this;
};

bowstring::PipelineBuilder &
bowstring::PipelineBuilder::setVertexAttributeDescriptions(
    vk::VertexInputAttributeDescription *pAttributeDescriptions,
    uint32_t attributeDescriptionsCount) {
  this->m_VertexInputCreateInfo.vertexAttributeDescriptionCount =
      attributeDescriptionsCount;
  this->m_VertexInputCreateInfo.pVertexAttributeDescriptions =
      pAttributeDescriptions;

  return *this;
};

bowstring::PipelineBuilder &bowstring::PipelineBuilder::setColorAttachments(
    vk::Format *pColorAttachmentFormats, uint32_t colorAttachmentCount) {

  this->m_PiplineRenderingCreateInfo.colorAttachmentCount =
      colorAttachmentCount;
  this->m_PiplineRenderingCreateInfo.pColorAttachmentFormats =
      pColorAttachmentFormats;
  return *this;
};

bowstring::PipelineBuilder &
bowstring::PipelineBuilder::setTopology(vk::PrimitiveTopology topology) {
  this->m_InputAssemblyCreateInfo.topology = topology;
  return *this;
};

bowstring::PipelineBuilder::Result
bowstring::PipelineBuilder::build(vk::Device &device) {
  bowstring::PipelineBuilder::Result result{};
  result.layout =
      device.createPipelineLayout(this->m_PipelineLayoutCreateInfo, nullptr);

  vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
  pipelineCreateInfo.pNext = &this->m_PiplineRenderingCreateInfo;
  pipelineCreateInfo.stageCount = this->m_ShaderStageCreateInfos.size();
  pipelineCreateInfo.pStages = this->m_ShaderStageCreateInfos.data();
  pipelineCreateInfo.pVertexInputState = &this->m_VertexInputCreateInfo;
  pipelineCreateInfo.pInputAssemblyState = &this->m_InputAssemblyCreateInfo;
  pipelineCreateInfo.pViewportState = &this->m_ViewportStateCreateInfo;
  pipelineCreateInfo.pRasterizationState =
      &this->m_RasterizationStateCreateInfo;
  pipelineCreateInfo.pMultisampleState = &this->m_MultiSampingStateCreateInfo;
  pipelineCreateInfo.pColorBlendState = &this->m_ColorBlendingStateCreateInfo;
  pipelineCreateInfo.pDynamicState = &this->m_DynamicStateCreateInfo;
  pipelineCreateInfo.pDepthStencilState = &this->m_DepthStencilCreateInfo;
  pipelineCreateInfo.layout = result.layout;
  pipelineCreateInfo.subpass = 0;
  result.pipeline =
      device
          .createGraphicsPipelines(VK_NULL_HANDLE, pipelineCreateInfo, nullptr)
          .value.front();
  return result;
};
