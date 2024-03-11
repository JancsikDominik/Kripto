#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <codecvt>
#include <unordered_map>
#include <ranges>
#include <algorithm>
#include <vector>
#include <utility>


// less typing its fine in a cpp file
using FrequencyMap = std::unordered_map<char, int>;


std::optional<std::string> ReadFile(const std::filesystem::path& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file " << filepath << std::endl;
        return {};
    }

    std::locale::global(std::locale(""));

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}


char ToEnglishAlpha(char ch)
{
    char correctedChar = ch;

    // lazy to do it in a clever way, its good enough
    if (ch == 'á')
        correctedChar = 'a';
    else if (ch == 'ó' || ch == 'ö' || ch == 'ő')
        correctedChar = 'o';
    else if (ch == 'í')
        correctedChar = 'i';
    else if (ch == 'é')
        correctedChar = 'e';
    else if (ch == 'ü' || ch == 'ű' || ch == 'û' || ch == 'ú')
        correctedChar = 'u';

    return correctedChar;
}


FrequencyMap GetLetterFrequencies(const std::string& content)
{
    FrequencyMap map;

    // making sure its set just in case
    std::locale::global(std::locale(""));

    for (char ch : content)
    {
        if (!std::isalpha(ch, std::locale()))
            continue;

        char correctedChar = ToEnglishAlpha(std::tolower(ch));
        map[correctedChar]++;
    }

    return map;
}


void PrintFrequencies(const FrequencyMap& freq)
{
    std::vector<std::pair<char, int>> freqVector;

    for (const auto& [key, value] : freq)
    {
        freqVector.push_back({ key, value });
    }

    std::ranges::sort(freqVector, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    for (const auto& [ch, freq] : freqVector)
    {
        std::cout << ch << " - " << freq << std::endl;
    }
}


int main()
{
    std::optional<std::string> fileContent = ReadFile("./egri_1.txt");

    if (fileContent.has_value())
    {
        FrequencyMap freq = GetLetterFrequencies(*fileContent);
        PrintFrequencies(freq);
    }
    else
    {
        std::cerr << "Couldn't Read file" << std::endl;
    }
}