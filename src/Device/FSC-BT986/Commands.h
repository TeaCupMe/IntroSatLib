#ifndef FSC_BT986_AT_COMMANDS_
#define FSC_BT986_AT_COMMANDS_

#define AT_OK       "\r\nOK\r\n"
#define AT_ERROR    "\r\nERROR\r\n"


#include <cstring>
#include <string>


template<typename... Args>
uint8_t* AT_CMD(const char* cmd, Args... args) {
    std::string result = "AT+";
    result += cmd;

    constexpr uint8_t num_args = sizeof...(args);
    if (num_args > 0) {
        result += "=";

        const char* params[] = { reinterpret_cast<const char*>(args)... };
        for (uint8_t i = 0; i < num_args; ++i) {
            if (i > 0) {
                result += ',';
            }
            result += params[i];
        }
    }

    result += "\r\n";

    uint8_t* buffer = new uint8_t[result.size() + 1];
    memcpy(buffer, result.c_str(), result.size() + 1);
    return buffer;
}


#endif /* FSC_BT986_AT_COMMANDS_ */
