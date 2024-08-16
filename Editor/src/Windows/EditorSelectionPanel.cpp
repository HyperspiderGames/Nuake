#define IMGUI_DEFINE_MATH_OPERATORS

#include "EditorSelectionPanel.h"
#include "../Misc/ImGuiTextHelper.h"
#include <src/Scene/Components/Components.h>

#include <src/Rendering/Textures/Material.h>
#include <src/Resource/ResourceLoader.h>
#include <src/Resource/FontAwesome5.h>
#include <src/Scripting/WrenScript.h>

#include <Engine.h>
#include <src/Resource/Prefab.h>

EditorSelectionPanel::EditorSelectionPanel()
{
}

void EditorSelectionPanel::ResolveFile(Ref<Nuake::File> file)
{
    using namespace Nuake;

    currentFile = file;

    if (currentFile->GetExtension() == ".project")
    {

    }

    if (currentFile->GetExtension() == ".material")
    {
        Ref<Material> material = ResourceLoader::LoadMaterial(currentFile->GetRelativePath());
        selectedResource = material;
    }
}

void EditorSelectionPanel::Draw(EditorSelection selection)
{
    if (ImGui::Begin("Properties"))
    {
        switch (selection.Type)
        {
            case EditorSelectionType::None:
            {
                DrawNone();
                break;
            }

            case EditorSelectionType::Entity:
            {
                DrawEntity(selection.Entity);
                break;
            }
            case EditorSelectionType::File:
            {
                if (currentFile != selection.File)
                {
                    ResolveFile(selection.File);
                }

				if (!selection.File->IsValid())
				{
					std::string text = "File is invalid";
					auto windowWidth = ImGui::GetWindowSize().x;
					auto windowHeight = ImGui::GetWindowSize().y;

					auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
					auto textHeight = ImGui::CalcTextSize(text.c_str()).y;
					ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
					ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);

					ImGui::TextColored({1, 0.1, 0.1, 1.0}, text.c_str());
				}

                DrawFile(selection.File);
                break;
            }
            case EditorSelectionType::Resource:
            {
                DrawResource(selection.Resource);
                break;
            }
        }
    }
    ImGui::End();

}

void EditorSelectionPanel::DrawNone()
{
    std::string text = "No selection";
    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;

    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
    auto textHeight = ImGui::CalcTextSize(text.c_str()).y;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);

    ImGui::Text(text.c_str());
}

void EditorSelectionPanel::DrawEntity(Nuake::Entity entity)
{
	if (!entity.IsValid())
	{
		return;
	}

    DrawAddComponentMenu(entity);

    // Draw each component properties panels.
    mTransformPanel.Draw(entity);
    mLightPanel.Draw(entity);
    mScriptPanel.Draw(entity);
	mNetScriptPanel.Draw(entity);
	mAudioEmitterPanel.Draw(entity);
	mParticleEmitterPanel.Draw(entity);
    mSpritePanel.Draw(entity);
    mMeshPanel.Draw(entity);
	mSkinnedModelPanel.Draw(entity);
	mBonePanel.Draw(entity);
    mQuakeMapPanel.Draw(entity);
    mCameraPanel.Draw(entity);
    mRigidbodyPanel.Draw(entity);
    mBoxColliderPanel.Draw(entity);
    mSphereColliderPanel.Draw(entity);
	mCapsuleColliderPanel.Draw(entity);
	mCylinderColliderPanel.Draw(entity);
    mMeshColliderPanel.Draw(entity);
    mCharacterControllerPanel.Draw(entity);
	mAudioEmitterPanel.Draw(entity);
	mNavMeshVolumePanel.Draw(entity);

	using namespace Nuake;
	
	float availWidth = ImGui::GetContentRegionAvail().x;
	const float buttonWidth = 200.f;
	float posX = (availWidth / 2.f) - (buttonWidth / 2.f);
	ImGui::SetCursorPosX(posX);
	
	if (UI::PrimaryButton("Add Component", { buttonWidth, 32 }))
	{
		ImGui::OpenPopup("ComponentPopup");
	}

	if (ImGui::BeginPopup("ComponentPopup"))
	{
		MenuItemComponent("Wren Script", WrenScriptComponent);
		MenuItemComponent("C# Script", NetScriptComponent);
		MenuItemComponent("Camera", CameraComponent);
		MenuItemComponent("Light", LightComponent);
		ImGui::Separator();
		MenuItemComponent("Model", ModelComponent);
		MenuItemComponent("Skinned Model", SkinnedModelComponent);
		MenuItemComponent("Bone", BoneComponent)
			ImGui::Separator();
		MenuItemComponent("Sprite", SpriteComponent)
			MenuItemComponent("Particle Emitter", ParticleEmitterComponent)
			ImGui::Separator();
		MenuItemComponent("Character Controller", CharacterControllerComponent)
			MenuItemComponent("Rigid body", RigidBodyComponent)
			ImGui::Separator();
		MenuItemComponent("Box collider", BoxColliderComponent)
			MenuItemComponent("Capsule collider", CapsuleColliderComponent)
			MenuItemComponent("Cylinder collider", CylinderColliderComponent)
			MenuItemComponent("Sphere collider", SphereColliderComponent)
			MenuItemComponent("Mesh collider", MeshColliderComponent)
			ImGui::Separator();
		MenuItemComponent("Quake map", QuakeMapComponent);
		ImGui::Separator();
		MenuItemComponent("Audio Emitter", AudioEmitterComponent);
		ImGui::Separator();
		MenuItemComponent("NavMesh Volume", NavMeshVolumeComponent);
		ImGui::EndPopup();
	}

}

void EditorSelectionPanel::DrawAddComponentMenu(Nuake::Entity entity)
{
	using namespace Nuake;
    if (entity.HasComponent<NameComponent>())
    {
        auto& entityName = entity.GetComponent<NameComponent>().Name;
        ImGuiTextSTD("##Name", entityName);
    }
    
}

void EditorSelectionPanel::DrawFile(Ref<Nuake::File> file)
{
    using namespace Nuake;
	switch (file->GetFileType())
	{
		case FileType::Material:
		{
			MaterialEditor matEditor;
			matEditor.Draw(std::static_pointer_cast<Material>(selectedResource));
			break;
		}
		case FileType::Project:
		{
			DrawProjectPanel(Nuake::Engine::GetProject());
			break;
		}
		case FileType::Script:
		{
			DrawWrenScriptPanel(CreateRef<WrenScript>(file, true));
			break;
		}
		case FileType::NetScript:
		{
			DrawNetScriptPanel(file);
			break;
		}
		case FileType::Prefab:
		{
			//Ref<Prefab> prefab = CreateRef<Prefab>(file->GetRelativePath());
			//DrawPrefabPanel(prefab);
			break;
		}
	}
}

void EditorSelectionPanel::DrawResource(Nuake::Resource resource)
{

}

void EditorSelectionPanel::DrawMaterialPanel(Ref<Nuake::Material> material)
{
    using namespace Nuake;

    std::string materialTitle = material->Path;
    {
        UIFont boldfont = UIFont(Fonts::SubTitle);
        ImGui::Text(material->Path.c_str());

    }
    ImGui::SameLine();
    {
        UIFont boldfont = UIFont(Fonts::Icons);
        if (ImGui::Button(ICON_FA_SAVE))
        {
            std::string fileData = material->Serialize().dump(4);

            FileSystem::BeginWriteFile(material->Path);
            FileSystem::WriteLine(fileData);
            FileSystem::EndWriteFile();
        }
    }

    bool flagsHeaderOpened;
    {
		UIFont boldfont = UIFont(Fonts::Bold); 
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 8.f));
		flagsHeaderOpened = ImGui::CollapsingHeader(" FLAGS", ImGuiTreeNodeFlags_DefaultOpen);
		ImGui::PopStyleVar(2);
    }
	
    if (flagsHeaderOpened)
    {
		ImGui::BeginTable("##Flags", 3, ImGuiTableFlags_BordersInner);
		{
			ImGui::TableSetupColumn("name", 0, 0.3f);
			ImGui::TableSetupColumn("set", 0, 0.6f);
			ImGui::TableSetupColumn("reset", 0, 0.1f);
			ImGui::TableNextColumn();

			ImGui::Text("Unlit");
			ImGui::TableNextColumn();

			bool unlit = material->data.u_Unlit == 1;
			ImGui::Checkbox("Unlit", &unlit);
			material->data.u_Unlit = (int)unlit;
		}
		ImGui::EndTable();
    }

	const auto TexturePanelHeight = 100;
    const ImVec2 TexturePanelSize = ImVec2(0, TexturePanelHeight);
    bool AlbedoOpened;
	{
		UIFont boldfont = UIFont(Fonts::Bold);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 8.f));
        AlbedoOpened = ImGui::CollapsingHeader("Albedo", ImGuiTreeNodeFlags_DefaultOpen);
		ImGui::PopStyleVar(2);
	}

	if (AlbedoOpened)
	{
        ImGui::BeginChild("##albedo", TexturePanelSize, true);
        {
			uint32_t textureID = 0;
			if (material->HasAlbedo())
			{
				textureID = material->m_Albedo->GetID();
			}

			if (ImGui::ImageButtonEx(ImGui::GetCurrentWindow()->GetID("#image1"), (void*)textureID, ImVec2(80, 80), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
			{
				std::string texture = FileDialog::OpenFile("*.png | *.jpg");
				if (texture != "")
				{
					material->SetAlbedo(TextureManager::Get()->GetTexture(texture));
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Clear Texture"))
				{
					material->m_Albedo = nullptr;
				}
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::ColorEdit3("Color", &material->data.m_AlbedoColor.r);
        }
        ImGui::EndChild();
	}

	if (ImGui::CollapsingHeader("Normal", ImGuiTreeNodeFlags_DefaultOpen))
	{
        ImGui::BeginChild("##normal", TexturePanelSize, true);
        {
			uint32_t textureID = 0;
			if (material->HasNormal())
			{
				textureID = material->m_Normal->GetID();
			}

			if (ImGui::ImageButtonEx(ImGui::GetCurrentWindow()->GetID("#image3"), (void*)textureID, ImVec2(80, 80), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
			{
				std::string texture = FileDialog::OpenFile("*.png | *.jpg");
				if (texture != "")
				{
					material->SetNormal(TextureManager::Get()->GetTexture(texture));
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Clear Texture"))
				{
					material->m_Normal = nullptr;
				}
				ImGui::EndPopup();
			}
        }
        ImGui::EndChild();
	}

	if (ImGui::CollapsingHeader("AO", ImGuiTreeNodeFlags_DefaultOpen))
	{
        ImGui::BeginChild("##ao", TexturePanelSize, true);
        {
			uint32_t textureID = 0;
			if (material->HasAO())
			{
				textureID = material->m_AO->GetID();
			}

			if (ImGui::ImageButtonEx(ImGui::GetCurrentWindow()->GetID("#image2"), (void*)textureID, ImVec2(80, 80), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1)))
			{
				std::string texture = FileDialog::OpenFile("Image files (*.png) | *.png | Image files (*.jpg) | *.jpg");
				if (texture != "")
				{
					material->SetAO(TextureManager::Get()->GetTexture(texture));
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Clear Texture"))
				{
					material->m_AO = nullptr;
				}
				ImGui::EndPopup();
			}
        }
        ImGui::EndChild();
	}
	
	if (ImGui::CollapsingHeader("Metalness", ImGuiTreeNodeFlags_DefaultOpen))
	{
        ImGui::BeginChild("##metalness", TexturePanelSize, true);
        {
			uint32_t textureID = 0;
			if (material->HasMetalness())
			{
				textureID = material->m_Metalness->GetID();
			}

			if (ImGui::ImageButtonEx(ImGui::GetCurrentWindow()->GetID("#image4"), (void*)textureID, ImVec2(80, 80), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
			{
				std::string texture = FileDialog::OpenFile("*.png | *.jpg");
				if (texture != "")
				{
					material->SetMetalness(TextureManager::Get()->GetTexture(texture));
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Clear Texture"))
				{
					material->m_Metalness = nullptr;
				}
				ImGui::EndPopup();
			}

			ImGui::SameLine();
			ImGui::DragFloat("Value##4", &material->data.u_MetalnessValue, 0.01f, 0.0f, 1.0f);
        }
        ImGui::EndChild();
	}

	if (ImGui::CollapsingHeader("Roughness", ImGuiTreeNodeFlags_DefaultOpen))
	{
        ImGui::BeginChild("##roughness", TexturePanelSize, true);
        {
			uint32_t textureID = 0;
			if (material->HasRoughness())
			{
				textureID = material->m_Roughness->GetID();
			}

			if (ImGui::ImageButtonEx(ImGui::GetCurrentWindow()->GetID("#image5"), (void*)textureID, ImVec2(80, 80), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
			{
				std::string texture = FileDialog::OpenFile("*.png | *.jpg");
				if (texture != "")
				{
					material->SetRoughness(TextureManager::Get()->GetTexture(texture));
				}
			}

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Clear Texture"))
				{
					material->m_Roughness = nullptr;
				}
				ImGui::EndPopup();
			}
        }
        ImGui::EndChild();
	}
}

void EditorSelectionPanel::DrawProjectPanel(Ref<Nuake::Project> project)
{
    ImGui::InputText("Project Name", &project->Name);
    ImGui::InputTextMultiline("Project Description", &project->Description);

    if (ImGui::Button("Locate"))
    {
		const std::string& locationPath = Nuake::FileDialog::OpenFile("TrenchBroom (.exe)\0TrenchBroom.exe\0");

		if (!locationPath.empty())
		{
			project->TrenchbroomPath = locationPath;
		}
    }

    ImGui::SameLine();
    ImGui::InputText("Trenchbroom Path", &project->TrenchbroomPath);
}

void EditorSelectionPanel::DrawWrenScriptPanel(Ref<Nuake::WrenScript> wrenFile)
{
	auto filePath = wrenFile->GetFile()->GetAbsolutePath();
	std::string fileContent = Nuake::FileSystem::ReadFile(filePath, true);
	
	ImGui::Text("Content");
	ImGui::SameLine(ImGui::GetWindowWidth()-90);
	if(ImGui::Button("Open..."))
	{
		Nuake::OS::OpenIn(filePath);
	}
	
	ImGui::Separator();
	
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetWindowWidth());
	ImGui::Text(fileContent.c_str(), ImGui::GetWindowWidth());
	
	ImGui::PopTextWrapPos();
}

void EditorSelectionPanel::DrawNetScriptPanel(Ref<Nuake::File> file)
{
	auto filePath = file->GetRelativePath();
	std::string fileContent = Nuake::FileSystem::ReadFile(filePath);

	ImGui::Text("Content");
	ImGui::SameLine(ImGui::GetWindowWidth() - 90);
	if (ImGui::Button("Open..."))
	{
		Nuake::OS::OpenIn(file->GetAbsolutePath());
	}

	ImGui::Separator();

	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetWindowWidth());
	ImGui::Text(fileContent.c_str(), ImGui::GetWindowWidth());

	ImGui::PopTextWrapPos();
}

