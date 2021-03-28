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

#pragma once

#include <memory>

#include "deviceinfo.h"

class Platform {
public:
    Platform() = default;

    virtual std::string name() = 0;
    virtual std::string prettyName() = 0;
    virtual DeviceInfo::DeviceType deviceType() = 0;
    virtual DeviceInfo::DriverType driverType() = 0;

    // Checks if the value returned is valid or default value
    virtual bool hasValidName() { return false; };
    virtual bool hasValidPrettyName() { return false; };
    virtual bool hasValidDeviceType() { return false; };

    static std::shared_ptr<Platform> create();
};
