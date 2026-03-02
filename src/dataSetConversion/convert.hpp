#ifndef   CONVERT_HPP
#define   CONVERT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>

class Convertor
{
private:
    const std::map<char, int> values = 
    {
        {'P', +1},
        {'N', +2},
        {'B', +3},
        {'R', +4},
        {'Q', +5},
        {'K', +6},
        {'p', -1},
        {'n', -2},
        {'b', -3},
        {'r', -4},
        {'q', -5},
        {'k', -6}
    };

    const std::map<std::string, int> sideToMove =
    {
        {"w", 1},
        {"b", -1}
    };

    const std::map<std::string, std::vector<int>> labelMapping =
    {
        {"CheckWhite",	    {1, 0, 0, 0, 0}},
        {"CheckBlack",	    {0, 1, 0, 0, 0}},
        {"CheckmateWhite",	{0, 0, 1, 0, 0}},
        {"CheckmateBlack",	{0, 0, 0, 1, 0}},
        {"Nothing",	        {0, 0, 0, 0, 1}}
    };
    
public:
    Convertor() = default;
    ~Convertor() = default;

    std::vector<int> parseDataSetLine(std::string & line);
    std::vector<int> getLineInfo(std::string & line);
    std::vector<int> getCastlingInfo(std::string & line);
    std::vector<int> getEnPassantInfo(std::string & line);
    
    void storeResult(std::vector<std::vector<int>> & res, std::string filepath);
    std::vector<std::string> getDataSet(std::string filepath);
    std::vector<std::vector<int>> parseDataSet(std::vector<std::string> & infosTab);

    void convertToIntFIle(std::string inputFile, std::string outputFile = "dataSetInt.txt");
    void retrieveBoardInfoTest(std::string inputFile, std::string outputFile = "boardTest.txt");
    static std::vector<float> retrieveBoardInfo(std::string line);
    void mergeDataSets(std::vector<std::string> inputFiles);
    
};

#endif /* !CONVERT_HPP */