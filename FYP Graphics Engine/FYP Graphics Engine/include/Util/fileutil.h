#pragma once
#include <string>

//Windows only headers

#include <windows.h>


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

	};
}
