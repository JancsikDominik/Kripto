#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <codecvt>
#include <unordered_map>


// less typing its fine in a cpp file
using FrequencyMap = std::unordered_map<char, int>;


std::optional<std::string> ReadFile(const std::filesystem::path& filepath)
{
    if (!std::filesystem::exists(filepath))
    {
        std::cerr << "Error: File " << filepath << " does not exist." << std::endl;
        return {};
    }

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
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
    for (const auto& [key, value] : freq)
    {
        std::cout << key << " - " << value << std::endl;
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
        std::cout << "Couldn't Read file" << std::endl;
    }
}