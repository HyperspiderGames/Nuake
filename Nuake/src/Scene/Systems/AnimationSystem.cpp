#include "AnimationSystem.h"

#include <Engine.h>
#include "src/Scene/Scene.h"
#include "src/Scene/Entities/Entity.h"
#include "src/Scene/Components/SkinnedModelComponent.h"
#include <src/Scene/Components/BoneComponent.h>
#include <future>

namespace Nuake
{
	AnimationSystem::AnimationSystem(Scene* scene)
	{
		m_Scene = scene;
	}

	bool AnimationSystem::Init()
	{
		return true;
	}

	void AnimationSystem::Update(Timestep ts)
	{
		if (Engine::GetGameState() == GameState::Paused)
		{
			return;
		}

		auto view = m_Scene->m_Registry.view<TransformComponent, SkinnedModelComponent>();
		for (auto e : view)
		{
			auto [transformComponent, skinnedComponent] = view.get<TransformComponent, SkinnedModelComponent>(e);

			Ref<SkinnedModel> model = skinnedComponent.ModelResource;

			// Load the model if it's not already loaded
			// TODO: [WiggleWizard] Needs some sort of flag to infer that we've tried loading it once already, so
			// we don't try loading the model every frame if it's invalid.
			if (skinnedComponent.ModelPath.Exist() && skinnedComponent.ModelResource == nullptr)
			{
				skinnedComponent.LoadModel(e, m_Scene);
			}
			
			if (!model)
			{
				continue;
			}

			float animationTime = 0.0f;
			Ref<SkeletalAnimation> animation = model->GetCurrentAnimation();
			if (animation && model->IsPlaying)
			{
				animationTime = animation->GetCurrentTime() + (ts * animation->GetTicksPerSecond());
				animation->SetCurrentTime(animationTime);
			}

			auto& rootBone = model->GetSkeletonRootNode();
			UpdateBonePositionTraversal(rootBone, animation, animationTime, model->IsPlaying);
		}
	}

	void AnimationSystem::UpdateBonePositionTraversal(SkeletonNode& bone, Ref<SkeletalAnimation> animation, float time, bool isPlaying)
	{
		if (Entity boneEnt = m_Scene->GetEntityByID(bone.EntityHandle); 
			boneEnt.IsValid())
		{
			// Update the bone transform
			auto& transformComponent = boneEnt.GetComponent<TransformComponent>();
			bone.FinalTransform = transformComponent.GetGlobalTransform() * bone.Offset;
			
			if (animation)
			{
				if (auto& animationTrack = animation->GetTrack(bone.Name); 
					!animationTrack.IsEmpty() && isPlaying)
				{
					// Get Update transform
					animationTrack.Update(time);

					const Matrix4& finalTransform = animationTrack.GetFinalTransform();

					Vector3 localPosition;
					Quat localRotation;
					Vector3 localScale;
					Decompose(finalTransform, localPosition, localRotation, localScale);

					transformComponent.SetLocalPosition(localPosition);
					transformComponent.SetLocalRotation(localRotation);
					transformComponent.SetLocalScale(localScale);
					transformComponent.SetLocalTransform(finalTransform);
				}
			}
			
		}

		for (auto& childBone : bone.Children)
		{
			UpdateBonePositionTraversal(childBone, animation, time, isPlaying);
		}
	}


	void AnimationSystem::FixedUpdate(Timestep ts)
	{
		
	}

	void AnimationSystem::EditorUpdate()
	{

	}

	void AnimationSystem::Exit()
	{

	}
}
