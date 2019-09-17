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

#include "deviceinfo.h"

class Config;
class Device {
public:
    Device();

    std::string name();
    std::string prettyName();
    DeviceType deviceType();
    DriverType driverType();
    int gridUnit();

    // get props that does not have auto detection
    std::string get(std::string prop, std::string defaultValue);
    bool contains(std::string prop);

    // Detect logic
    DeviceType detectType(bool returnUknown);
    std::string detectName();

private:
    std::string getHaliumProp(const char* prop, const char* default_value);
    std::string getHaliumProp(const char* prop);
    bool hasHaliumProp(const char* prop);

    bool m_isHalium;
    std::shared_ptr<Config> m_config;
};
