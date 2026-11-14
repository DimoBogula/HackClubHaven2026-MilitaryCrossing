#include <ini.h>
#include <cstdint>

enum class IniParserState
{
    Scanning = 0,
    VariableValue,
    HeaderName
};

void IniParser::Parse(std::string content)
{
    ini_content.clear();
    IniParserState state = IniParserState::Scanning;
    std::string buffer = "";
    IniContent* currentParent = nullptr;
    while (content.size() > 0)
    {
        buffer += content.front();
        content.erase(content.begin());
        if (content.front() == '\n' && buffer.size() > 1 && state == IniParserState::Scanning)
        {
            currentParent->variables.emplace_back();
            currentParent->variables.back().name = buffer;
            state = IniParserState::Scanning;
            buffer = "";
        }
        if (buffer == "\n" || buffer == " ")
        {
            buffer = "";
            continue;
        }
        if (state == IniParserState::VariableValue)
        {
            if (buffer.back() == '\n')
            {
                buffer.erase(buffer.end()-1);
                currentParent->variables.back().value = buffer;
                state = IniParserState::Scanning;
                buffer = "";
                continue;
            }
        }
        if (state == IniParserState::Scanning && buffer.size() > 1)
        {
            if (buffer.back() == '=')
            {
                buffer.erase(buffer.end()-1);
                currentParent->variables.emplace_back();
                currentParent->variables.back().name = buffer;
                state = IniParserState::VariableValue;
                buffer = "";
                continue;
            }
        }
        if (state == IniParserState::HeaderName && buffer.size() > 1)
        {
            if (buffer.back() == ']')
            {
                buffer.erase(buffer.end()-1);
                ini_content.emplace_back();
                ini_content.back().name = buffer;
                currentParent = &ini_content.back();
                state = IniParserState::Scanning;
                buffer = "";
                continue;
            }
        }
        if (buffer.size() == 1)
        {
            switch (buffer[0])
            {
                case '[':
                buffer = "";
                state = IniParserState::HeaderName;
                continue;
            }
        }
    }
    if (state == IniParserState::VariableValue)
    {
        currentParent->variables.back().value = buffer;
    }
    else if (state == IniParserState::Scanning && buffer.size() > 0)
    {
        currentParent->variables.emplace_back();
        currentParent->variables.back().name = buffer;
    }
}

bool IniParser::HasContent(std::string name)
{
    for (IniContent& c : ini_content) if (c.name == name) return true;
    return false;
}

IniContent &IniParser::GetContent(std::string name)
{
    for (IniContent& c : ini_content) if (c.name == name) return c;
    return ini_content.back();
}

bool IniContent::HasVariable(std::string name)
{
    for (IniVariable& v : variables) if (v.name == name) return true;
    return false;
}

IniVariable &IniContent::GetVariable(std::string name)
{
    for (IniVariable& v : variables) if (v.name == name) return v;
    return variables.back();
}
