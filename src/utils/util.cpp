#include "utils/util.h"

std::string get_env_var(std::string const &key)
{
    char *val = getenv(key.c_str());
    return val == NULL ? "" : std::string(val);
}