#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

class Config
{
private:
    std::string filename;
    std::unordered_map<std::string, std::string> values;

public:
    Config(const std::string& file)
        : filename(file)
    {
        load();
    }

    void load()
    {
        values.clear();

        std::ifstream file(filename);
        if (!file)
            return;

        std::string line;

        while (std::getline(file, line))
        {
            size_t equals = line.find('=');

            if (equals == std::string::npos)
                continue;

            std::string key = line.substr(0, equals);
            std::string value = line.substr(equals + 1);

            values[key] = value;
        }
    }

    std::string get(const std::string& key)
    {
        if (values.find(key) == values.end())
            return "";

        return values[key];
    }

    void set(const std::string& key, const std::string& value)
    {
        values[key] = value;
    }

    void save()
    {
        std::ofstream file(filename);

        for (const auto& pair : values)
        {
            file << pair.first << "=" << pair.second << '\n';
        }
    }
};