#pragma once
#include "src/Scripting/WrenScript.h"
#include "src/Resource/Serializable.h"
#include "src/FileSystem/FileSystem.h"
#include "src/FileSystem/File.h"

#include "src/Core/Logger.h"

namespace Nuake 
{
	class WrenScriptComponent
	{
	public:
		std::string Script;

		unsigned int mModule = 0;
		Ref<WrenScript> mWrenScript = nullptr;

		void LoadScript(const std::string& path)
		{
			Script = path;
			Ref<Nuake::File> nuakeFile = Nuake::FileSystem::GetFile(path);

			if (!nuakeFile)
			{
				Logger::Log("Failed to load script: " + path, "script", CRITICAL);
				return;
			}

			mWrenScript = CreateRef<Nuake::WrenScript>(nuakeFile, true);
			auto modules = mWrenScript->GetModules();
			if (modules.size() > 0)
			{
				mModule = 0;
			}
		}

		json Serialize()
		{
			BEGIN_SERIALIZE();
			SERIALIZE_VAL(Script);
			SERIALIZE_VAL(mModule)
			END_SERIALIZE();
		}

		bool Deserialize(const json& j)
		{
			if (j.contains("Script"))
			{
				Script = j["Script"];
				if(!Script.empty())
				{
					LoadScript(Script);
				}
			}

			if (j.contains("mModule"))
			{
				mModule = j["mModule"];
			}

			return true;
		}
	};
}