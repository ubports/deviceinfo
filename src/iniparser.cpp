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

#include "iniparser.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_SECTION 50
#define MAX_NAME 5
#define INI_MAX_LINE 200
#define INI_INLINE_COMMENT_PREFIXES ";"

char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

char* find_chars_or_comment(const char* s, const char* chars)
{
    int was_space = 0;
    while (*s && (!chars || !strchr(chars, *s)) &&
           !(was_space && strchr(INI_INLINE_COMMENT_PREFIXES, *s))) {
        was_space = isspace((unsigned char)(*s));
        s++;
    }
    return (char*)s;
}

char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler,
                     void* user)
{
    char line[200];
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;

    /* Scan through stream line by line */
    while (reader(line, INI_MAX_LINE, stream) != NULL) {
        lineno++;

        start = line;
        if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
                           (unsigned char)start[1] == 0xBB &&
                           (unsigned char)start[2] == 0xBF) {
            start += 3;
        }
        start = lskip(rstrip(start));

        if (*start == ';' || *start == '#') {
            /* Per Python configparser, allow both ; and # comments at the
               start of a line */
        }
        else if (*prev_name && *start && start > line) {

        end = find_chars_or_comment(start, NULL);
        if (*end)
            *end = '\0';
        rstrip(start);


            /* Non-blank line with leading whitespace, treat as continuation
               of previous name's value (as per Python configparser). */
            if (!handler(user, section, prev_name, start) && !error)
                error = lineno;
        }

        else if (*start == '[') {
            /* A "[section]" line */
            end = find_chars_or_comment(start + 1, "]");
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error) {
                /* No ']' found on section line */
                error = lineno;
            }
        }
        else if (*start) {
            /* Not a comment, must be a name[=:]value pair */
            end = find_chars_or_comment(start, "=:");
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
                end = find_chars_or_comment(value, NULL);
                if (*end)
                    *end = '\0';
                rstrip(value);

                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if (!handler(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                error = lineno;
            }
        }
    }

    return error;
}

int ini_parse_file(FILE* file, ini_handler handler, void* user)
{
    return ini_parse_stream((ini_reader)fgets, file, handler, user);
}

IniParser::IniParser(std::string filename)
{
    FILE* file;
    file = fopen(filename.c_str(), "r");
    if (file) {
        m_error = ini_parse_file(file, valueHandler, this);
        fclose(file);
    }
}

IniParser::IniParser(FILE *file)
{
    m_error = ini_parse_file(file, valueHandler, this);
}

int IniParser::parseError() const
{
    return m_error;
}

const std::set<std::string>& IniParser::sections() const
{
    return m_sections;
}

bool IniParser::contains(std::string section, std::string name) const
{
    std::string key = makeKey(section, name);
    return !!m_values.count(key);
}

std::string IniParser::get(std::string section, std::string name, std::string default_value) const
{
    std::string key = makeKey(section, name);
    return m_values.count(key) ? m_values.at(key) : default_value;
}

long IniParser::getInteger(std::string section, std::string name, long default_value) const
{
    std::string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double IniParser::getReal(std::string section, std::string name, double default_value) const
{
    std::string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool IniParser::getBoolean(std::string section, std::string name, bool default_value) const
{
    std::string valstr = get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

std::string IniParser::makeKey(std::string section, std::string name)
{
    std::string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int IniParser::valueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    IniParser* reader = (IniParser*)user;
    std::string key = makeKey(section, name);
    if (reader->m_values[key].size() > 0)
        reader->m_values[key] += "\n";
    reader->m_values[key] += value;
    reader->m_sections.insert(section);
    return 1;
}
