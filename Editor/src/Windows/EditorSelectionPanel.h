#pragma once
#include "../Actions/EditorSelection.h"
#include "src/Scene/Entities/Entity.h"

#include "src/FileSystem/FileSystem.h"
#include <src/Resource/Project.h>

#include "../ComponentsPanel/TransformPanel.h"
#include "../ComponentsPanel/LightPanel.h"
#include "../ComponentsPanel/ScriptPanel.h"
#include "../ComponentsPanel/MeshPanel.h"
#include "../ComponentsPanel/QuakeMapPanel.h"
#include "../ComponentsPanel/CameraPanel.h"
#include "../ComponentsPanel/RigidbodyPanel.h"
#include "../ComponentsPanel/BoxColliderPanel.h"
#include "../ComponentsPanel/CapsuleColliderPanel.h"
#include "../ComponentsPanel/CylinderColliderPanel.h"
#include "../ComponentsPanel/SphereColliderPanel.h"
#include "../ComponentsPanel/MeshColliderPanel.h"
#include "../ComponentsPanel/CharacterControllerPanel.h"
#include "../ComponentsPanel/SpritePanel.h"
#include "../ComponentsPanel/ParticleEmitterPanel.h"
#include "../ComponentsPanel/SkinnedModelPanel.h"
#include "../ComponentsPanel/BonePanel.h"
#include "../ComponentsPanel/AudioEmitterPanel.h"
#include "../ComponentsPanel/NetScriptPanel.h"
#include "../ComponentsPanel/NavMeshVolumePanel.h"
#include "../ComponentsPanel/UIPanel.h"

#include <src/Scene/Components/WrenScriptComponent.h>
#include <src/Resource/Prefab.h>


namespace Nuake
{
	class Scene;
}

class EditorSelectionPanel 
{
private:
	TransformPanel mTransformPanel;
	LightPanel mLightPanel;
	ScriptPanel mScriptPanel;
	NetScriptPanel mNetScriptPanel;
	MeshPanel mMeshPanel;
	SkinnedModelPanel mSkinnedModelPanel;
	QuakeMapPanel mQuakeMapPanel;
	CameraPanel mCameraPanel;
	RigidbodyPanel mRigidbodyPanel;
	BoxColliderPanel mBoxColliderPanel;
	SphereColliderPanel mSphereColliderPanel;
	MeshColliderPanel mMeshColliderPanel;
	CapsuleColliderPanel mCapsuleColliderPanel;
	CylinderColliderPanel mCylinderColliderPanel;
	SpritePanel mSpritePanel;
	CharacterControllerPanel mCharacterControllerPanel;
	ParticleEmitterPanel mParticleEmitterPanel;
	BonePanel mBonePanel;
	AudioEmitterPanel mAudioEmitterPanel;
	NavMeshVolumePanel mNavMeshVolumePanel;
	UIPanel mUiPanel;

	Ref<Nuake::File> currentFile;
	Ref<Nuake::Resource> selectedResource;

	Ref<Nuake::Scene> virtualScene;
public:
	EditorSelectionPanel();

	void Draw(EditorSelection selection);

	void DrawNone();
	void DrawEntity(Nuake::Entity entity);
	void DrawAddComponentMenu(Nuake::Entity entity);

	void DrawFile(Ref<Nuake::File> file);
	void DrawResource(Nuake::Resource resource);
	void DrawPrefabPanel(Ref<Nuake::Prefab> prefab);
private:
	void ResolveFile(Ref<Nuake::File> file);
	void DrawMaterialPanel(Ref<Nuake::Material> material);
	void DrawProjectPanel(Ref<Nuake::Project> project);
	void DrawWrenScriptPanel(Ref<Nuake::WrenScript> wrenFile);
	void DrawNetScriptPanel(Ref<Nuake::File> file);
};