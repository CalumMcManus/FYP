#pragma once
#include <iostream>
#include <fstream>
#include <Util\fileutil.h>

namespace Engine
{
	class Project
	{
	public:
		Project() {};
		~Project() {};
		bool SetUpProjectDirectories();
	private:
		std::string m_Directory;

		
		bool CreateConfigFile(std::string path);
	};
}