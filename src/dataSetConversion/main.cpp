#include "convert.hpp"

int main(int ac, char **av)
{
    if (std::string(av[1]) == "--help") {
        std::cout << "Usage: ./convert [--convert | --tests <input_file> [ <output_file> ]] [--merge <input1> <input2> ... ] " << std::endl;
        std::cout << "--convert : Convert the dataset from input_file to integer format." << std::endl;
        std::cout << "--tests : Get board info without label." << std::endl;
        std::cout << "      <input_file> : path to the input dataset file." << std::endl;
        std::cout << "      <output_file> : (optional) path to the output file to store results." << std::endl;
        std::cout << "--merge : Merge differents file config into one." << std::endl;
        std::cout << "      <input1> <input2> ... : paths to the input dataset files to merge." << std::endl;
        std::cout << "If output_file is not provided, " << std::endl;
        std::cout << "   - for --convert, results will be stored in 'dataSetInt.txt'." << std::endl;
        std::cout << "   - for --tests, results will be stored in 'boardTest.txt'." << std::endl;
        std::cout << "   - for --merge, results will be stored basing on the name of the input files." << std::endl;
        return 0;
    }
    if (std::string(av[1]) == "--convert" && (ac == 3 || ac == 4)) {
        Convertor convertor;

        if (ac == 4)
            convertor.convertToIntFIle(av[2], av[3]);
        else
            convertor.convertToIntFIle(av[2]);
        
        return 0;
    } else if (std::string(av[1]) == "--tests" && (ac == 3 || ac == 4)) {
        Convertor convertor;

        if (ac == 4)
            convertor.retrieveBoardInfoTest(av[2], av[3]);
        else
            convertor.retrieveBoardInfoTest(av[2]);
        
        return 0;
    } else if (std::string(av[1]) == "--merge" && ac >= 4) {
        Convertor convertor;
        std::vector<std::string> inputFiles;

        for (int i = 2; i < ac; i++)
            inputFiles.push_back(av[i]);

        convertor.mergeDataSets(inputFiles);

        return 0;
    } else {
        std::cerr << "Invalid arguments. Use --help for usage information." << std::endl;
        return 1;
    }
    
}
