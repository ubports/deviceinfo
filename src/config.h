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
 */

#pragma once

#include <memory>

class Device;
class IniParser;
class Config {
public:
    Config(Device *device);

    std::string get(std::string prop, bool defaults, std::string defaultValue);
    std::string get(std::string prop, bool defaults);
    bool contains(std::string prop, bool defaults);

    static bool exists(std::string name);
private:
    static std::string nameToConfigPath(std::string name);
    static std::string getEnv(const char *name, std::string dval);

    Device *m_device;
    std::shared_ptr<IniParser> m_deviceIni;
    std::shared_ptr<IniParser> m_defaultIni;
};
