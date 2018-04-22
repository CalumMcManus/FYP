#include "stdafx.h"
#include "CppUnitTest.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <Windows.h>
#include <Util\fileutil.h>

namespace EngineTest
{		
	using namespace Engine;
	TEST_CLASS(EngineTests)
	{
	public:
		
		TEST_METHOD(UnitTestTest)
		{
			int i = 1;
			Assert::IsTrue(i == 1);
		}

		TEST_METHOD(FilePathSplit)
		{
			FileUtils testClass;
			std::string examplePath = "./FolderOne/FolderTwo/File.file";
			std::string exampleName = "File.file";
			std::set<char> delims{ '\\', '/' };
			std::string fileName = testClass.test_splitpath(examplePath, delims).back();
			bool success = fileName == exampleName;
			Assert::IsTrue(success);
			
		}

	};
}