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
    MOCK_METHOD(std::string, name, (), (override));
    MOCK_METHOD(std::string, prettyName, (), (override));
    MOCK_METHOD(DeviceInfo::DeviceType, deviceType, (), (override));
    MOCK_METHOD(DeviceInfo::DriverType, driverType, (), (override));

    MOCK_METHOD(int, gridUnit, (), (override));

    MOCK_METHOD(std::string, get, (std::string, std::string), (override));
    MOCK_METHOD(bool, contains, (std::string), (override));
};
