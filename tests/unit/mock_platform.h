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
#include "platform/platform.h"

class MockPlatform : public Platform
{
public:
    MOCK_METHOD(std::string, name, (), (override));
    MOCK_METHOD(std::string, prettyName, (), (override));
    MOCK_METHOD(DeviceInfo::DeviceType, deviceType, (), (override));
    MOCK_METHOD(DeviceInfo::DriverType, driverType, (), (override));

    MOCK_METHOD(bool, hasValidName, (), (override));
    MOCK_METHOD(bool, hasValidPrettyName, (), (override));
    MOCK_METHOD(bool, hasValidDeviceType, (), (override));
};

class DummyPlatform : public Platform
{
public:
    DummyPlatform() {};

    DummyPlatform(std::string name, std::string pName) :
                  m_name(name),
                  m_pName(pName) {};

    DummyPlatform(std::string name, std::string pName,
                  DeviceInfo::DeviceType deType,
                  DeviceInfo::DriverType drType) :
                  m_name(name),
                  m_pName(pName),
                  m_deType(deType),
                  m_drType(drType) {};

    std::string name() override { return m_name; };
    std::string prettyName() override { return m_pName; };
    DeviceInfo::DeviceType  deviceType() override { return m_deType; };
    DeviceInfo::DriverType driverType() override { return m_drType; };

    bool hasValidName() override { return !m_name.empty(); };
    bool hasValidPrettyName() override { return !m_pName.empty(); };
    bool hasValidDeviceType() override { return m_deType != DeviceInfo::DeviceType::Unknown; };

private:
    std::string m_name;
    std::string m_pName;
    DeviceInfo::DeviceType m_deType;
    DeviceInfo::DriverType m_drType;
};
