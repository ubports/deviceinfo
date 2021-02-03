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

#include <gtest/gtest.h>

#include "deviceinfo.h"

#include "mock_device.h"


#define TEST_RETURN_FUCS(name, func, func2, ret) \
TEST(DeviceInfoTest, test##name)            \
{                                           \
    MockDevice device;                      \
    EXPECT_CALL(device, func2)              \
        .Times(1)                           \
        .WillOnce(::testing::Return(ret));  \
                                            \
    DeviceInfo di(&device);                 \
                                            \
    EXPECT_EQ(di.func, ret);                \
}

TEST_RETURN_FUCS(Name, name(), name(), "a")
TEST_RETURN_FUCS(PrettyName, prettyName(), prettyName(), "a")
TEST_RETURN_FUCS(DeviceType, deviceType(), deviceType(), DeviceInfo::DeviceType::Tablet)
TEST_RETURN_FUCS(DriverType, driverType(), driverType(), DeviceInfo::DriverType::Halium)
TEST_RETURN_FUCS(GridUnit, gridUnit(), gridUnit(), 16)

TEST_RETURN_FUCS(Get, get("", ""), get("", ""), "a")
TEST_RETURN_FUCS(Contains, contains(""), contains(""), true)

TEST(DeviceInfoTest, testSupportedOrientations)
{
    MockDevice device;
    EXPECT_CALL(device, get("SupportedOrientations", "Portrait,InvertedPortrait,Landscape,InvertedLandscape"))
        .Times(1)
        .WillOnce(::testing::Return("Portrait,InvertedPortrait,Landscape,InvertedLandscape"));

    DeviceInfo di(&device);

    EXPECT_EQ(di.supportedOrientations(), (std::vector<std::string>{"Portrait", "InvertedPortrait", "Landscape", "InvertedLandscape"}));
}

TEST_RETURN_FUCS(PrimaryOrientation, primaryOrientation(), get("PrimaryOrientation", "Portrait"), "a")
TEST_RETURN_FUCS(PortraitOrientation, portraitOrientation(), get("PortraitOrientation", "Portrait"), "a")
TEST_RETURN_FUCS(InvertedPortraitOrientation, invertedPortraitOrientation(), get("InvertedPortraitOrientation", "InvertedPortrait"), "a")
TEST_RETURN_FUCS(LandscapeOrientation, landscapeOrientation(), get("LandscapeOrientation", "Landscape"), "a")
TEST_RETURN_FUCS(InvertedLandscapeOrientation, invertedLandscapeOrientation(), get("InvertedLandscapeOrientation", "InvertedLandscape"), "a")

TEST(DeviceInfoTest, testDeviceTypeToString)
{
    EXPECT_EQ(DeviceInfo::deviceTypeToString(DeviceInfo::DeviceType::Phone), "phone");
    EXPECT_EQ(DeviceInfo::deviceTypeToString(DeviceInfo::DeviceType::Tablet), "tablet");
    EXPECT_EQ(DeviceInfo::deviceTypeToString(DeviceInfo::DeviceType::Desktop), "desktop");
}

TEST(DeviceInfoTest, testdeviceTypeFromString)
{
    EXPECT_EQ(DeviceInfo::deviceTypeFromString("phone"), DeviceInfo::DeviceType::Phone);
    EXPECT_EQ(DeviceInfo::deviceTypeFromString("tablet"), DeviceInfo::DeviceType::Tablet);
    EXPECT_EQ(DeviceInfo::deviceTypeFromString("desktop"), DeviceInfo::DeviceType::Desktop);
}

TEST(DeviceInfoTest, testDriverTypeToString)
{
    EXPECT_EQ(DeviceInfo::driverTypeToString(DeviceInfo::DriverType::Halium), "halium");
    EXPECT_EQ(DeviceInfo::driverTypeToString(DeviceInfo::DriverType::Linux), "linux");
}

TEST(DeviceInfoTest, testDriverTypeFromString)
{
    EXPECT_EQ(DeviceInfo::driverTypeFromString("halium"), DeviceInfo::DriverType::Halium);
    EXPECT_EQ(DeviceInfo::driverTypeFromString("linux"), DeviceInfo::DriverType::Linux);
}
