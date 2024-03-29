/*
 * Copyright (C) 2021 UBports foundation.
 * Author(s): Marius Gripsgard <marius@ubports.com>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 3, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranties of MERCHANTABILITY,
 * SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "utils.h"

#include "logger.h"

#include <sstream>
#include <fstream>

namespace utils {
namespace path {

std::string join(std::string first, std::string second) {
    if (first.back() != '/')
        first.append("/");
    first.append(second);

    return first;
}

std::string join(std::string first, std::string file, std::string suffix) {
    auto path = join(first, file);
    path.append(suffix);

    return path;
}

std::string join(std::string first, std::string second, std::string file, std::string suffix) {
    return join(join(first, second), file, suffix);
}

bool exists(std::string name)
{
    Log::verbose("Seeing if %s exists", name.c_str());
    std::ifstream infile(name);
    return infile.good();
}

}; // path

namespace env {

std::string get(const char *name, std::string dval)
{
    const char* env = getenv(name);
    if (env) {
         Log::verbose("Using provided env %s", env);
        return std::string(env);
    }

    return dval;
}

bool set(const char *name, std::string val, bool overwrite)
{
    return setenv(name, val.c_str(), overwrite) == 0;
}

bool unset(const char *name)
{
    return unsetenv(name) == 0;
}

}; // env

namespace string {

std::vector<std::string> split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
} // string


bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

bool startsWith(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

};
}; // utils
