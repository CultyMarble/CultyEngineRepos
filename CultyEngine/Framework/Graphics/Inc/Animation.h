#pragma once
#include "Keyframe.h"
#include "Transform.h"

namespace CultyEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;

	private:
		const MathC::Vector3& GetPosition(float time) const;
		const MathC::Quaternion& GetRotation(float time) const;
		const MathC::Vector3& GetScale(float time) const;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}