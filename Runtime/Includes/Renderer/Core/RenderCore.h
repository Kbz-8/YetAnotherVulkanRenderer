#ifndef __YAVR_RENDER_CORE__
#define __YAVR_RENDER_CORE__

#include <PreCompiled.h>
#include <Utils/Singleton.h>

#include <Renderer/Core/Instance.h>
#include <Renderer/Core/Device.h>
#include <Renderer/Core/Queues.h>
#include <Renderer/Core/ValidationLayers.h>

namespace Yavr
{
	#ifdef YAVR_DEBUG
		constexpr bool ENABLE_VALIDATION_LAYERS = true;
	#else
		constexpr bool ENABLE_VALIDATION_LAYERS = false;
	#endif

	const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };

	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;

	const char* VerbaliseVkResult(VkResult result);

	class RenderCore : public Singleton<RenderCore>
	{
		friend class Singleton<RenderCore>;

		public:
			void Init();
			void Destroy();

			inline bool IsInit() const noexcept { return m_is_init; }
			inline Instance& GetInstance() noexcept { return m_instance; }
			inline Device& GetDevice() noexcept { return m_device; }
			inline Queues& GetQueue() noexcept { return m_queues; }
			inline ValidationLayers& GetLayers() noexcept { return m_layers; }

		private:
			RenderCore() = default;
			~RenderCore() = default;

		private:
			ValidationLayers m_layers;
			Queues m_queues;
			Device m_device;
			Instance m_instance;
			bool m_is_init = false;
	};
}

#endif
