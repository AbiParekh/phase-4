// FinalTest.cpp : This file contains the 'main' function for testing the finalMerge class. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "FinalMerge.h"
using namespace std;

bool TestCase_setParameters();

bool TestCase_IsolateWord();

bool TestCase_mapToVectorConversion();

void ExportResults(std::map<std::string, bool> exportMap);


class FinalMergeTESTFramework : public Final
{
public: 

       FinalMergeTESTFramework() {}

	   bool isolateWordFramework(const std::string& formattedWord, const std::string& startString, const std::string& endString, std::string& isolatedWord)
	   {
		   bool output = IsolateWord(formattedWord, startString, endString, isolatedWord);
		   return output;
	   }

	   bool mergeFromReduceFramework(std::string& outputFileName) {
		   return mergeFromReduce(outputFileName);
	   }


};

int main(int argc, char* argv[])
{
	std::map<std::string, bool> testCaseOutput;

	std::pair<std::string, bool> settingParameters("Parameters test case", TestCase_setParameters());
	testCaseOutput.insert(settingParameters);

	std::pair < std::string, bool> IsoWord("Testing for isolate word", TestCase_IsolateWord());
	testCaseOutput.insert(IsoWord);

	ExportResults(testCaseOutput);


    return 0;
}


void ExportResults(std::map<std::string, bool> exportMap) {
	bool results = true;
	std::cout << "Unit Test Output:" << endl;

	for (std::map<std::string, bool>::iterator i = exportMap.begin(); i != exportMap.end(); i++) {
		std::string wrap = "";
		if (i->second == 0) wrap = "test fail";
		else wrap = "Test pass";
		std::cout << "Unit Test: " << i->first << "Output: " << wrap << std::endl;
	};
}


bool TestCase_setParameters() {

	Final finVec;
	bool results = true;

	std::string firstPos = "positionFirst";
	std::string secondPos = "positionSecond";
	std::string verify, verifiers;

	finVec.setParameters(firstPos, secondPos);
	finVec.setParameters(verify, verifiers);

	if (firstPos != verify) results = false;
	if (secondPos != verifiers) results = false;

	return results;

}

bool TestCase_IsolateWord() {

	bool results_IsolateWord = true;
	std::string delim = "\"";
	std::string formattedWord = "(\"row\", 1)";
	std::string formattedWord2 = "(\"depth_charge\", 1)";
	std::string formattedWord3 = "NO Deliminator";
	std::string isloatedWord = "";
	std::string isloatedWord2 = "";
	std::string isloatedWord3 = "";
	FinalMergeTESTFramework final;

	bool results = final.isolateWordFramework(formattedWord, delim, delim, isloatedWord);

	int correctWord = isloatedWord.compare("row");
	if (correctWord != 0 || results == false)
	{
		std::cout << "Test_IsolateWord: Unable to Isolate row" << std::endl;
		results_IsolateWord = false;
	}


	results = final.isolateWordFramework(formattedWord2, delim, delim, isloatedWord2);
	correctWord = isloatedWord2.compare("depth_charge");
	if (correctWord != 0 || results == false)
	{
		std::cout << "Test_IsolateWord: Unable to Isolate depth_charge" << std::endl;
		results_IsolateWord = false;
	}

	results = final.isolateWordFramework(formattedWord3, delim, delim, isloatedWord3);
	if (results != false)
	{
		std::cout << "Test_IsolateWord: did not fail when deliminator is missing" << std::endl;
		results_IsolateWord = false;
	}

	return results_IsolateWord;
}

bool TestCase_mapToVectorConversion() {

}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
