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
class Platform;
class Device {
public:
    Device();
    Device(std::shared_ptr<Platform> platform, std::shared_ptr<Config> config);

    virtual std::string name();
    virtual std::string prettyName();
    virtual DeviceInfo::DeviceType deviceType();
    virtual DeviceInfo::DriverType driverType();
    virtual int gridUnit();

    // get props that does not have auto detection
    virtual std::string get(std::string prop, std::string defaultValue);
    virtual bool contains(std::string prop);
private:
    std::shared_ptr<Platform> m_platform;
    std::shared_ptr<Config> m_config;
};
