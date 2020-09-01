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

#include "platform.h"

#include <memory>

namespace platform {
class Linux : public Platform {
public:
    Linux() = default;

    std::string name() override;
    std::string prettyName() override;
    DeviceInfo::DeviceType deviceType() override;
    DeviceInfo::DriverType driverType() override;

    bool hasValidName() override;

    static bool usable() { return true; };
private:
    std::string readFile(std::string file);
};
}
