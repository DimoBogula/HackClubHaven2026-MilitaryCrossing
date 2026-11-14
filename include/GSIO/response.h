#pragma once
#include <string>
#define GSIO_response_Default_Message "No message provided"
#define GSIO_response_Error_Prefix "Fatal Error"
#define GSIO_response_Warning_Prefix "Warning"
#define GSIO_response_Info_Prefix "Info"

class GSIO_response
{
    public:
    bool success = false;
    std::string message = GSIO_response_Default_Message;
    GSIO_response(bool s, std::string m = GSIO_response_Default_Message) : success(s), message(m) {}
};