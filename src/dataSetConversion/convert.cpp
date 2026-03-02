#include "convert.hpp"

std::vector<int> Convertor::getLineInfo(std::string & line)
{
    std::vector<int> tab;

    for (char pos : line) {
        if (pos >= '0' && pos <= '9') {
            int a = pos - '0';

            for (int i = 0; i < a; i++) {
                tab.push_back(0);
            }
        } else
            tab.push_back(values.at(pos));
    }
    return tab;
}

std::vector<int> Convertor::getCastlingInfo(std::string & line)
{
    std::vector<int> tab(4, 0);

    if (line == "-")
        return tab;

    for (char pos : line) {
        if (pos == 'K')
            tab[0] = values.at('K');
        else if (pos == 'Q')
            tab[1] = values.at('Q');
        else if (pos == 'k')
            tab[2] = values.at('k');
        else if (pos == 'q')
            tab[3] = values.at('q');
    }

    return tab;
}

std::vector<int> Convertor::getEnPassantInfo(std::string & line)
{
    std::vector<int> tab(2, -1);

    if (line == "-")
        return tab;

    tab[0] = line[0] - 'a';
    tab[1] = line[1] - '0';

    return tab;
}

std::vector<std::string> Convertor::getDataSet(std::string filepath)
{
    std::ifstream in(filepath);
    std::vector<std::string> res;
    std::string line;

    while (getline(in, line, '\n')) {
        res.push_back(line);
    }
    
    return res;
}

void Convertor::storeResult(std::vector<std::vector<int>> & res, std::string filepath)
{
    std::ofstream out(filepath, std::ios::app);

    for (auto & tmp : res) {
        for (auto & th : tmp) {
            out << th << " ";
        }
        out << std::endl;
    }
}

std::vector<int> Convertor::parseDataSetLine(std::string & newline)
{
    std::stringstream stmp(newline);
    std::vector<std::string> spaceTab;
    std::vector<int> res;
    std::string ss;

    while (std::getline(stmp, ss,  ' ')) {
        spaceTab.push_back(ss);
    }
        
    std::stringstream  sboard(spaceTab[0]);
    std::string line;

    while (std::getline(sboard, line,  '/')) {
        std::vector<int> resint = getLineInfo(line);
        res.insert(res.end(), resint.begin(), resint.end());
    }

    res.push_back(sideToMove.at(spaceTab[1]));

    std::vector<int> rescastle = getCastlingInfo(spaceTab[2]);
    res.insert(res.end(), rescastle.begin(), rescastle.end());

    std::vector<int> enpassant = getEnPassantInfo(spaceTab[3]);
    res.insert(res.end(), enpassant.begin(), enpassant.end());

    res.push_back(stoi(spaceTab[4]));
    res.push_back(stoi(spaceTab[5]));

    std::string tmpLabel = "";
    for (size_t i = 6; i < spaceTab.size(); i++)
        tmpLabel.insert(tmpLabel.end(), spaceTab[i].begin(), spaceTab[i].end());

    auto a = labelMapping.at(tmpLabel);
    res.insert(res.end(), a.begin(), a.end());
    return res;
}

std::vector<std::vector<int>> Convertor::parseDataSet(std::vector<std::string> & infosTab)
{
    std::vector<std::vector<int>> allResults;
    
    for (auto & tmp : infosTab) {
        allResults.push_back(parseDataSetLine(tmp));
    }

    return allResults;
}

void Convertor::convertToIntFIle(std::string inputFile, std::string outputFile)
{
    std::vector<std::string> infosTab = getDataSet(inputFile);

    std::vector<std::vector<int>> allResults = parseDataSet(infosTab);

    storeResult(allResults, outputFile);
}

void Convertor::retrieveBoardInfoTest(std::string inputFile, std::string outputFile)
{
    std::vector<std::string> infosTab = getDataSet(inputFile);
    std::ofstream out(outputFile, std::ios::app);

    for (auto & tmp : infosTab) {
        std::stringstream stmp(tmp);
        std::vector<std::string> spaceTab;
        std::string ss;

        while (std::getline(stmp, ss,  ' '))
            spaceTab.push_back(ss);
        
        for (int i = 0; i < 5; i++)
            out << spaceTab[i] << " ";
        out << spaceTab[5] << " ";
        out << std::endl;
    }
}

std::vector<float> Convertor::retrieveBoardInfo(std::string line)
{

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

    std::stringstream stmp(line);
    std::vector<std::string> spaceTab;
    std::vector<float> tab;
    std::string ss;

    while (std::getline(stmp, ss,  ' ')) {
        spaceTab.push_back(ss);
    }

    auto getLInfo = [&](std::string & l) {
        std::vector<float> res;
        for (char pos : l) {
            if (pos >= '0' && pos <= '9') {
                int a = pos - '0';

                for (int i = 0; i < a; i++) {
                    res.push_back(0.0f);
                }
            } else
                res.push_back(static_cast<float>(values.at(pos)));
        }
        return res;
    };

    auto getCInfo = [&](std::string & l) {
        std::vector<float> res(4, 0.0f);

        if (l == "-")
            return res;

        for (char pos : l) {
            if (pos == 'K')
                res[0] = static_cast<float>(values.at('K'));
            else if (pos == 'Q')
                res[1] = static_cast<float>(values.at('Q'));
            else if (pos == 'k')
                res[2] = static_cast<float>(values.at('k'));
            else if (pos == 'q')
                res[3] = static_cast<float>(values.at('q'));
        }

        return res;
    };

    auto getEInfo = [&](std::string & l) {
        std::vector<float> res(2, -1.0f);

        if (l == "-")
            return res;

        res[0] = static_cast<float>(l[0] - 'a');
        res[1] = static_cast<float>(l[1] - '0');

        return res;
    };
        
    std::stringstream  sboard(spaceTab[0]);
    std::string l;

    while (std::getline(sboard, l,  '/')) {
        std::vector<float> resfloat = getLInfo(l);
        for (auto & val : resfloat)
            tab.push_back(val);
    }

    tab.push_back(static_cast<float>(sideToMove.at(spaceTab[1])));

    std::vector<float> rescastle = getCInfo(spaceTab[2]);
    for (auto & val : rescastle)
        tab.push_back(val);

    std::vector<float> enpassant = getEInfo(spaceTab[3]);
    for (auto & val : enpassant)
        tab.push_back(val);

    tab.push_back(static_cast<float>(stoi(spaceTab[4])));
    tab.push_back(static_cast<float>(stoi(spaceTab[5])));

    return tab;
}

void Convertor::mergeDataSets(std::vector<std::string> inputFiles)
{
    std::vector<std::string> fileData;

    for (const auto& inputFile : inputFiles) {
        std::vector<std::string> dataSet = getDataSet(inputFile);
        fileData.insert(fileData.end(), dataSet.begin(), dataSet.end());
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(fileData.begin(), fileData.end(), std::default_random_engine(seed));

    std::shuffle(fileData.begin(), fileData.end(), std::default_random_engine(seed));

    for (size_t i = 0; i < 4; ++i) {
        std::string outputFile = "merged_many_pieces" + std::to_string(i) + "_.txt";
        std::ofstream out(outputFile, std::ios::app);
        for (size_t j = i; j < fileData.size(); j += 4) {
            out << fileData[j] << std::endl;
        }
    }
}
