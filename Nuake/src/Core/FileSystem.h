#pragma once
#include "Core.h"
#include "String.h"

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace Nuake
{
	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::string OpenFolder();
	};

	class Directory;
	class File;

	class FileSystem
	{
	public:
		static std::string Root;

		static Ref<Directory> RootDirectory;

		static void SetRootDirectory(const std::string path);

		static void Scan();
		static std::string AbsoluteToRelative(const std::string& path);
		static std::string RelativeToAbsolute(const std::string& path);
		static std::string GetParentPath(const std::string& fullPath);
		static Ref<Directory> GetFileTree();
		static Ref<File> GetFile(const std::string& path);
		static std::string GetFileNameFromPath(const std::string& path);
		static void ScanDirectory(Ref<Directory> directory);
		static void GetDirectories();

		static bool MakeDirectory(const std::string& path, bool absolute = false);
		static bool DirectoryExists(const std::string& path, bool absolute = false);
		static bool FileExists(const std::string& path, bool absolute = false);

		static std::string ReadFile(const std::string& path, bool absolute = false);

		static std::ofstream fileWriter;
		static bool BeginWriteFile(const std::string path, bool absolute = false);
		static bool WriteLine(const std::string line);
		static void EndWriteFile();
		static uintmax_t DeleteFileFromPath(const std::string& path);
		static uintmax_t DeleteFolder(const std::string& path);
		static std::string GetConfigFolderPath();
	};

	enum class FileType
	{
		Unkown,
		Image,
		Material,
		Mesh,
		Script,
		NetScript,
		Project,
		Prefab,
		Scene,
		Wad,
		Map,
		Assembly,
		Solution,
		Audio
	};

	class File
	{
	private:
		std::string Type;
		std::string Name;
		std::string RelativePath;
		std::string AbsolutePath;
		Ref<Directory> Parent;
	public:

		std::string GetExtension() const { return Type; }
		std::string GetName() const { return Name; }
		std::string GetRelativePath() const { return RelativePath; }
		std::string GetAbsolutePath() const { return AbsolutePath; }
		Ref<Directory> GetParent() const { return Parent; }
		FileType GetFileType() const
		{
			std::string ext = GetExtension();
			if (ext == ".png" || ext == ".jpg")
			{
				return FileType::Image;
			}
			if (ext == ".ogg" || ext == ".wav")
			{
				return FileType::Audio;
			}
			if (ext == ".material")
			{
				return FileType::Material;
			}

			if (ext == ".scene")
			{
				return FileType::Scene;
			}

			if (ext == ".project")
			{
				return FileType::Project;
			}

			if (ext == ".script")
			{
				return FileType::Script;
			}

			if (ext == ".prefab")
			{
				return FileType::Prefab;
			}

			if (ext == ".wad")
			{
				return FileType::Wad;
			}

			if (ext == ".map")
			{
				return FileType::Map;
			}

			if (ext == ".dll")
			{
				return FileType::Assembly;
			}

			if (ext == ".cs")
			{
				return FileType::NetScript;
			}

			if (ext == ".sln")
			{
				return FileType::Solution;
			}

			if (ext == ".mesh")
			{
				return FileType::Mesh;
			}

			return FileType::Unkown;
		}

		std::string GetFileTypeAsString() const
		{
			std::string ext = GetExtension();
			if (ext == ".png" || ext == "jpg")
			{
				return "Image";
			}

			if (ext == ".material")
			{
				return "Material";
			}

			if (ext == ".scene")
			{
				return "Scene";
			}

			if (ext == ".project")
			{
				return "Project";
			}

			if (ext == ".script")
			{
				return "Script";
			}

			if (ext == ".prefab")
			{
				return "Prefab";
			}

			if (ext == ".wad")
			{
				return "Wad";
			}

			if (ext == ".map")
			{
				return "Map";
			}

			if (ext == ".map")
			{
				return "Assembly";
			}

			if (ext == ".cs")
			{
				return "C# Script";
			}

			return "File";
		}

		std::string Read()
		{
			return FileSystem::ReadFile(AbsolutePath, true);
		}

		bool IsValid()
		{
			return FileSystem::FileExists(AbsolutePath, true);
		}

		File(Ref<Directory> parentDir, const std::string& absolutePath, const std::string& name, const std::string& type)
		{
			AbsolutePath = absolutePath;
			Parent = parentDir;
			RelativePath = FileSystem::AbsoluteToRelative(absolutePath);
			Name = name;
			Type = type;
		}
	};

	class Directory
	{
	public:
		std::string Name;
		std::string FullPath;
		Ref<Directory> Parent;
		std::vector<Ref<Directory>> Directories;
		std::vector<Ref<File>> Files;

		Directory(const std::string& path);
		Directory() = default;
		~Directory() = default;
	};
}
