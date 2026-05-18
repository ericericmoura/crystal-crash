#pragma once

#include <string>
#include <fstream>
#include <ios>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

template <typename T>
class DataHandler
{
private:
    inline static const int kIndentAmount = 4;

    std::string filepath_;

    void CreateTemplateJsonFile();

    void SaveToFile(const json& data);

    json ReadFromFile();

    static bool IsEmpty(std::ifstream& pFile)
    {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

public:
    DataHandler(std::string filepath);

    T GetBlueprint();
    void SaveBlueprint(T blueprint);

    json GetRawData();
    void SaveRawData(json data);
};

template<typename T>
inline void DataHandler<T>::CreateTemplateJsonFile()
{
    json oldData = ReadFromFile();

    bool hasData = oldData.size() > 0;

    if (hasData)
    {
        return;
    }

    T item{};

    json data = item;

    SaveToFile(data);
}

template<typename T>
void DataHandler<T>::SaveToFile(const json& data)
{
    std::ofstream outputFile(filepath_);
    outputFile << data.dump(kIndentAmount);
    outputFile.close();
}

template<typename T>
json DataHandler<T>::ReadFromFile()
{
    std::ifstream inputFile(filepath_);

    if (!IsEmpty(inputFile))
    {
        json data;
        inputFile >> data;
        return data;
    }

    return json();
}

template<typename T>
inline DataHandler<T>::DataHandler(std::string filepath) : filepath_(filepath)
{
    CreateTemplateJsonFile();
}

template<typename T>
inline T DataHandler<T>::GetBlueprint()
{
    T blueprint = ReadFromFile();

    return blueprint;
}

template<typename T>
void DataHandler<T>::SaveBlueprint(T newItem)
{
    json data = newItem;

    SaveToFile(data);
}

template<typename T>
inline json DataHandler<T>::GetRawData()
{
    return ReadFromFile();
}

template<typename T>
inline void DataHandler<T>::SaveRawData(json data)
{
    SaveToFile(data);
}

}