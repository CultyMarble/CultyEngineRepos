#include "Precompiled.h"
#include "SoundEffectManager.h"

#include "AudioSystem.h"

using namespace CultyEngine;
using namespace CultyEngine::Audio;

namespace
{
	std::unique_ptr<SoundEffectManager> sSoundEffectManager;
}

void SoundEffectManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sSoundEffectManager == nullptr, "SoundEffectManager: is already initialed!");
	sSoundEffectManager = std::make_unique<SoundEffectManager>();
	sSoundEffectManager->SetRootPath(root);
}

void SoundEffectManager::StaticTerminate()
{
	if (sSoundEffectManager != nullptr)
	{
		sSoundEffectManager->Clear();
		sSoundEffectManager.reset();
	}
}

SoundEffectManager* SoundEffectManager::Get()
{
	ASSERT(sSoundEffectManager != nullptr, "SoundEffectManager: is not initialized!");
	return sSoundEffectManager.get();
}

SoundEffectManager::~SoundEffectManager()
{
	ASSERT(mInventory.empty(), "SoundEffectManager: clear must be called!");
}

void SoundEffectManager::SetRootPath(const std::filesystem::path& root)
{
	mRoot = root;
}

SoundID SoundEffectManager::Load(const std::filesystem::path& fileName)
{
	std::filesystem::path fullPath = mRoot / fileName;
	std::size_t soundID = std::filesystem::hash_value(fullPath);
	auto [iter, sucess] = mInventory.insert({ soundID, nullptr });
	if (sucess)
	{
		AudioSystem* as = AudioSystem::Get();
		auto& soundEventPtr = iter->second;
		soundEventPtr->effect = std::make_unique<SoundEffect>(as->mAudioEngine, fullPath.wstring());
		soundEventPtr->instance = soundEventPtr->effect->CreateInstance();
	}

	return soundID;
}

void SoundEffectManager::Clear()
{
	AudioSystem:Get()->Suspend();
	for (auto& entry : mInventory)
	{
		if (entry.second)
		{
			entry.second->instance->Stop();
			entry.second->instance.reset();
			entry.second->effect->Stop();
			entry.second.reset();
		}
	}
}

void SoundEffectManager::Play(SoundID id, bool loop)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second
	}
}

void SoundEffectManager::Stop(SoundID id)
{
}
