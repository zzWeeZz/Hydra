#include "HYpch.h"
#include "VulkanGraphicsPipeline.h"
#include <Hydra/API/Vulkan/Resources/VulkanFramebuffer.h>
#include "Hydra/API/Vulkan/Backend/VulkanDevice.h"
#include <Hydra/API/Vulkan/Resources/VulkanShader.h>
namespace Hydra
{
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const GraphicsPipelineSpecification& specs, Ptr<VulkanDevice> device) : GraphicsPipeline(specs)
	{
		m_DeviceHandle = device;
		Validate();
	}
	void VulkanGraphicsPipeline::Validate()
	{
		std::vector<VkDynamicState> dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		auto vulkanShader = std::reinterpret_pointer_cast<VulkanShader>(m_Specs.shaderObject.lock());
		std::vector<VkPipelineShaderStageCreateInfo> pipeStages;
		auto& stages = vulkanShader->m_VkStages;

		for (VkShaderStageFlagBits& stage : stages)
		{
			auto& spirv = vulkanShader->GetSpirvFromVkShaderStage(stage);

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = spirv.size();
			createInfo.pCode = spirv.data();

			VkShaderModule shaderModule;
			HY_VK_CHECK(vkCreateShaderModule(m_DeviceHandle.lock()->GetHandle(), &createInfo, nullptr, &shaderModule));


			VkPipelineShaderStageCreateInfo& shaderStageInfo = pipeStages.emplace_back();
			shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageInfo.stage = stage;
			shaderStageInfo.pName = "main";
			shaderStageInfo.module = shaderModule;
		}

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_Specs.width);
		viewport.height = static_cast<float>(m_Specs.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent.width = static_cast<uint32_t>(m_Specs.width);
		scissor.extent.height = static_cast<uint32_t>(m_Specs.height);

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional
		depthStencil.back = {}; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;


		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = m_Specs.bindingDescription.binding;
		bindingDescription.stride = m_Specs.bindingDescription.stride;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> vkBindingDesc(m_Specs.vertexBindingAttributes.size());

		for (size_t i = 0; auto& binding : vkBindingDesc)
		{
			auto& specbinding = m_Specs.vertexBindingAttributes[i];
			binding.binding = specbinding.binding;
			binding.location = specbinding.location;
			binding.offset = specbinding.offset;
			binding.format = GetVkFormat(specbinding.format);
			i++;
		}


		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vkBindingDesc.size());
		vertexInputInfo.pVertexAttributeDescriptions = vkBindingDesc.data(); // Optional


		std::vector<VkDescriptorSetLayout> descriptorlayouts;
		for (auto& binding : vulkanShader->m_Layouts)
		{
			auto& bindings = binding.second;
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
			layoutInfo.pBindings = bindings.data();
			auto& descriptorLayout = descriptorlayouts.emplace_back();
			HY_VK_CHECK(vkCreateDescriptorSetLayout(m_DeviceHandle.lock()->GetHandle(), &layoutInfo, nullptr, &descriptorLayout));
			VulkanAllocator::CustomDeletion([=]() {vkDestroyDescriptorSetLayout(m_DeviceHandle.lock()->GetHandle(), descriptorLayout, nullptr); });
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pSetLayouts = descriptorlayouts.data();
		pipelineLayoutInfo.setLayoutCount = descriptorlayouts.size();
		HY_VK_CHECK(vkCreatePipelineLayout(m_DeviceHandle.lock()->GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
		VulkanAllocator::CustomDeletion([&]() { vkDestroyPipelineLayout(m_DeviceHandle.lock()->GetHandle(), m_PipelineLayout, nullptr); });


		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(pipeStages.size());
		pipelineInfo.pStages = pipeStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		//pipelineInfo.pDepthStencilState = &depthStencil;

		pipelineInfo.layout = m_PipelineLayout;
		auto vulkanFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(m_Specs.framebufferObject.lock());

		auto& formats = vulkanFramebuffer->GetFormats();

		// Fetch vk formats
		std::vector<VkFormat> vkFormats(formats.size());

		for (size_t i = 0; auto & vkformat : vkFormats)
		{
			vkformat = GetVkFormat(formats[i]);
			i++;
		}


		const VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
			.colorAttachmentCount = static_cast<uint32_t>(vkFormats.size()),
			.pColorAttachmentFormats = vkFormats.data(),
			.depthAttachmentFormat = VK_FORMAT_UNDEFINED,
		};

		pipelineInfo.pNext = &pipelineRenderingCreateInfo;
		pipelineInfo.renderPass = nullptr;

		HY_VK_CHECK(vkCreateGraphicsPipelines(m_DeviceHandle.lock()->GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));
		VulkanAllocator::CustomDeletion([&]() {vkDestroyPipeline(m_DeviceHandle.lock()->GetHandle(), m_Pipeline, nullptr); });

		for (auto& pipelineStage : pipeStages)
		{
			vkDestroyShaderModule(m_DeviceHandle.lock()->GetHandle(), pipelineStage.module, nullptr);
		}
	}
}