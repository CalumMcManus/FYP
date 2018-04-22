#include "stdafx.h"
#include "CppUnitTest.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <Windows.h>
#include <Util\fileutil.h>
#include <Engine\gameObject.h>
#include <Component\component.h>
#include <Component\transform.h>
#include <glm.hpp>

namespace EngineTest
{		
	using namespace Engine;
	TEST_CLASS(EngineTests)
	{
	public:
		//TestTest
		TEST_METHOD(UnitTestTest)
		{
			int i = 1;
			Assert::IsTrue(i == 1);
		}
		//File Reading unility functions
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
		TEST_METHOD(CheckFileExsists)
		{
			FileUtils testClass;
			bool success = testClass.test_Exists("./Test.txt");
			Assert::IsTrue(success);
		}

		TEST_METHOD(CheckFileRead)
		{
			FileUtils testClass;
			std::string success = testClass.test_ReadFile("./Test.txt");
			Assert::IsTrue(success == "Test");
		}
		//Game logic
		TEST_METHOD(AddGetComponent)
		{
			GameObject* test = new GameObject();
			test->addComponent(new Transform());
			Transform* temp = test->getComponent<Transform>();
			bool success = temp != nullptr;

			Assert::IsTrue(success);
		}
		TEST_METHOD(TransformMath)
		{
			Transform* temp1 = new Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
			temp1->Translate(glm::vec3(1, 1, 1));
			temp1->Rotate(glm::vec3(1, 1, 1));
			temp1->Scale(glm::vec3(2, 2, 2));

			bool success = temp1->getPosition() == glm::vec3(1, 1, 1) && temp1->getRotation() == glm::vec3(1, 1, 1) && temp1->getSize() == glm::vec3(2, 2, 2);
			Assert::IsTrue(true);
		}
	};
}