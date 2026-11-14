#pragma once
#include <string>
#include <vector>

class IniVariable
{
    public:
    std::string name;
    std::string value;
};

class IniContent
{
    public:
    std::string name;
    std::vector<IniVariable> variables;
    bool HasVariable(std::string name);
    IniVariable& GetVariable(std::string name);
};

class IniParser
{
    public:
    std::vector<IniContent> ini_content;
    void Parse(std::string content);
    bool HasContent(std::string name);
    IniContent& GetContent(std::string name);

    IniParser() {}
};