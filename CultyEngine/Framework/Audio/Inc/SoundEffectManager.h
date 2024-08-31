#pragma once

namespace CultyEngine::Audio
{
	using SoundID = std::size_t;

	class SoundEffectManager final
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static SoundEffectManager* Get();

		SoundEffectManager() = default;
		~SoundEffectManager();

		SoundEffectManager(const SoundEffectManager&) = delete;
		SoundEffectManager(const SoundEffectManager&&) = delete;
		SoundEffectManager& operator=(const SoundEffectManager&) = delete;
		SoundEffectManager& operator=(const SoundEffectManager&&) = delete;

		void SetRootPath(const std::filesystem::path& root);

		SoundID Load(const std::filesystem::path& filename);
		void Clear();

		void Play(SoundID id, bool loop = false);
		void Stop(SoundID id);

	private:
		struct Entry
		{
			std::unique_ptr<DirectX::SoundEffect> effect;
			std::unique_ptr<DirectX::SoundEffectInstance> instance;
		};

		using SoundEffects = std::unordered_map<SoundID, std::unique_ptr<Entry>>;
		SoundEffects mInventory;

		std::filesystem::path mRoot;
	};
}