#pragma once
#include <string>
#include <sstream>
//Windows only headers

#include <windows.h>
#include <shlobj.h>
#include <Commdlg.h>
namespace Engine {
	class FileUtils
	{
	private:

	public:
		//Using C library instead of fstream as it is faster
		//This requires _CRT_SECURE_NO_WARNINGS in preprocessor
		static std::string read_file(const char* filePath)
		{
			FILE* file = fopen(filePath, "rt");
			fseek(file, 0, SEEK_END);
			unsigned long length = ftell(file);
			char* data = new char[length + 1];
			memset(data, 0, length + 1);
			fseek(file, 0, SEEK_SET);
			fread(data, 1, length, file);
			fclose(file);

			std::string result(data);
			delete[] data;
			return result;
		}

		static std::string read_shader(const char* fileName)
		{
			const char * path = "../../Shaders/" + (char)fileName;
			return read_file(path);
		}

		static __int64 FileSize(const char* name)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			if (!GetFileAttributesEx(name, GetFileExInfoStandard, &fad))
				return -1; // error condition, could call GetLastError to find out more
			LARGE_INTEGER size;
			size.HighPart = fad.nFileSizeHigh;
			size.LowPart = fad.nFileSizeLow;
			return size.QuadPart;
		}

		//Opens windows file explorer to browse files
		static std::string BrowseFiles(const char* name)
		{
			const int BUFSIZE = 1024;
			char buffer[BUFSIZE] = { 0 };
			OPENFILENAME ofns = { 0 };
			ofns.lStructSize = sizeof(ofns);
			//ofns.lpstrFilter = "OBJ Files (*.obj)\0*.obj\0";
			ofns.lpstrFile = buffer;
			ofns.nMaxFile = BUFSIZE;
			ofns.lpstrTitle = name;
			ofns.Flags = OFN_NOCHANGEDIR;
			GetOpenFileName(&ofns);
			std::string pathValue(buffer);
			
			return pathValue;
		}

		//Opens windows file explorer to browse folders
		static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
		{

			if (uMsg == BFFM_INITIALIZED)
			{
				std::string tmp = (const char *)lpData;
				std::cout << "path: " << tmp << std::endl;
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}

			return 0;
		}

		static std::string BrowseFolder()//std::string saved_path)
		{
			TCHAR path[MAX_PATH];

			//const char * path_param = saved_path.c_str();

			BROWSEINFO bi = { 0 };
			bi.lpszTitle = ("Browse for folder...");
			bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			bi.lpfn = BrowseCallbackProc;
			bi.lParam = (LPARAM)"./";

			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

			if (pidl != 0)
			{
				//get the name of the folder and put it in path
				SHGetPathFromIDList(pidl, path);

				//free memory used
				IMalloc * imalloc = 0;
				if (SUCCEEDED(SHGetMalloc(&imalloc)))
				{
					imalloc->Free(pidl);
					imalloc->Release();
				}
				std::string slash("/");
				return path + slash;
			}

			return "";
		}

		static bool CreateFolder(std::string atPath, std::string folderName)
		{
			std::string folderPath = std::string(atPath + folderName);
			if (CreateDirectory(folderPath.c_str(), NULL) ||
				ERROR_ALREADY_EXISTS == GetLastError())
			{
				std::cout << "Successfully created folder at: " << folderPath << std::endl;
				return true;
			}
			else
			{
				std::cout << "Failed to created folder at: " << folderPath << " Error: " << GetLastError() << std::endl;
				return false;
			}
		}

	};
}
