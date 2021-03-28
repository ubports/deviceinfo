/*
 * Copyright (C) 2020 UBports foundation.
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

#include "linux.h"

#include <memory>
#include <string>
#include <iterator>
#include <fstream>
#include <unistd.h>

// dts based model
#define LINUX_MODEL "/proc/device-tree/model"

std::string platform::Linux::readFile(std::string file)
{
    std::ifstream model(file);
    std::string ret;
    if (model.good())
        ret = std::string(std::istreambuf_iterator<char>{model},
                          std::istreambuf_iterator<char>());
    return ret;
}

std::string platform::Linux::name()
{
    if (hasValidName()) {
        auto model = readFile(LINUX_MODEL);
        if (!model.empty())
            return model;
    }

    return "generic-linux";
}

bool platform::Linux::hasValidName()
{
    return access(LINUX_MODEL, F_OK) != -1;
}

// Just returns default value
std::string platform::Linux::prettyName()
{
    return "Generic linux device";
}

DeviceInfo::DeviceType platform::Linux::deviceType()
{
    return DeviceInfo::DeviceType::Unknown;
}

DeviceInfo::DriverType platform::Linux::driverType()
{
    return DeviceInfo::DriverType::Linux;
}
