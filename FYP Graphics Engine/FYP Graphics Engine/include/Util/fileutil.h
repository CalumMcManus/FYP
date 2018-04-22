#pragma once
#include <string>
#include <sstream>

#include <windows.h>
#include <shlobj.h>
#include <Commdlg.h>
#include <set>
#include <iostream>
#include <fstream>
#include <vector>

namespace Engine {
	//! FileUtils 
	/*!
	FileUtils class. Contains functions for reading and writing files as well as string manipulation for file paths.
	*/
	class FileUtils
	{
	private:

	public:
		FileUtils() {};
		//Using C library instead of fstream as it is faster
		//This requires _CRT_SECURE_NO_WARNINGS in preprocessor

		//! The read_file static member function
		/*!
		Reads in file and returns it as a string.
		\param path Directory
		*/
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

		//! The FileSize static member function
		/*!
		Returns the file size of requested file
		\param path Directory
		*/
		static __int64 FileSize(const char* path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			if (!GetFileAttributesEx(path, GetFileExInfoStandard, &fad))
				return -1; // error condition, could call GetLastError to find out more
			LARGE_INTEGER size;
			size.HighPart = fad.nFileSizeHigh;
			size.LowPart = fad.nFileSizeLow;
			return size.QuadPart;
		}

		//Opens windows file explorer to browse files
		//! The BrowseFiles static member function
		/*!
		Opens dialogue to select a file, returns file path
		\param name Dialogue name
		*/
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
		//! The BrowseFolder static member function
		/*!
		Opens dialogue to select a folder, returns directory
		*/
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
		//! The CreateFolder static member function
		/*!
		Creates a folder at a given path with a given name
		\param atPath Directory
		\param folderName Name of new folder
		*/
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
		//! The CreateFolder static member function
		/*!
		Returns true if a file exsists at given path
		\param path Directory
		*/
		inline static bool Exists(const std::string& path) {
			if (FILE *file = fopen(path.c_str(), "r")) {
				fclose(file);
				return true;
			}
			else {
				return false;
			}
		}

		static std::vector<std::string> splitpath(
			const std::string& str
			, const std::set<char> delimiters)
		{
			std::vector<std::string> result;

			char const* pch = str.c_str();
			char const* start = pch;
			for (; *pch; ++pch)
			{
				if (delimiters.find(*pch) != delimiters.end())
				{
					if (start != pch)
					{
						std::string str(start, pch);
						result.push_back(str);
					}
					else
					{
						result.push_back("");
					}
					start = pch + 1;
				}
			}
			result.push_back(start);

			return result;
		}
		//! The TransferFile static member function
		/*!
		Copies a file from one location and duplicates it to a second location
		\param file1 file1
		\param file2 file1
		*/
		static void TransferFile(std::string file1, std::string file2)
		{
			std::ifstream ifs(file1, std::ios::binary);
			std::ofstream ofs(file2, std::ios::binary);

			ofs << ifs.rdbuf();
		}

		//Unit Test Wrappers
		std::vector<std::string> test_splitpath(
			const std::string& str
			, const std::set<char> delimiters)
		{
			return splitpath(str, delimiters);
		}

	};
}
