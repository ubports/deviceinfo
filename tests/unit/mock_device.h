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

#include <gmock/gmock.h>

#include "device.h"

class MockDevice : public Device
{
public:
    MOCK_METHOD0(name, std::string());
    MOCK_METHOD0(prettyName, std::string());
    MOCK_METHOD0(deviceType, DeviceInfo::DeviceType());
    MOCK_METHOD0(driverType, DeviceInfo::DriverType());

    MOCK_METHOD0(gridUnit, int());

    MOCK_METHOD2(get, std::string(std::string, std::string));
    MOCK_METHOD1(contains, bool(std::string));
};
