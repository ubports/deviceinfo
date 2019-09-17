/*
 * Copyright (C) 2019 UBports foundation.
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
 * Written with help from: https://github.com/benhoyt/inih/blob/master/ini.c
 *
 */

#pragma once

#include <string>
#include <map>
#include <set>

/* Typedef for prototype of handler function. */
typedef int (*ini_handler)(void* user, const char* section,
                           const char* name, const char* value);

/* Typedef for prototype of fgets-style reader function. */
typedef char* (*ini_reader)(char* str, int num, void* stream);

class IniParser
{
public:
    IniParser() {};
    IniParser(std::string filename);
    IniParser(FILE *file);

    int parseError() const;
    const std::set<std::string>& sections() const;

    bool contains(std::string section, std::string name) const;
    std::string get(std::string section, std::string name,
                    std::string default_value) const;
    long getInteger(std::string section, std::string name, long default_value) const;
    double getReal(std::string section, std::string name, double default_value) const;
    bool getBoolean(std::string section, std::string name, bool default_value) const;

protected:
    int m_error = -1;
    std::map<std::string, std::string> m_values;
    std::set<std::string> m_sections;
    static std::string makeKey(std::string section, std::string name);
    static int valueHandler(void* user, const char* section, const char* name,
                            const char* value);
};
