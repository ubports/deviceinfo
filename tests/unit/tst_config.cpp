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

#include <gtest/gtest.h>
#include <memory>

#include "config.h"
#include "logger.h"

#include "mock_platform.h"

class BaseConfigTest : public ::testing::Test
{
public:
    BaseConfigTest()
    {
        Logger::setMode(DeviceInfo::PrintMode::Verbose);
        unsetenv(ENV_CONFIG_PATH);
        unsetenv(ENV_DEFAULT_CONFIG);
        unsetenv(ENV_DEVICE_PATH);
        unsetenv(ENV_DEVICE_NAME);
        unsetenv(ENV_LEGACY_PATH);
    }
};

class DefaultsConfigTest : public BaseConfigTest
{
public:
    DefaultsConfigTest() : BaseConfigTest()
    {
        setenv(ENV_CONFIG_PATH, TEST_DEFAULT_DIR, true);
    }
};

class DeviceConfigTest : public BaseConfigTest
{
public:
    DeviceConfigTest() : BaseConfigTest()
    {
        setenv(ENV_DEVICE_PATH, TEST_DEVICE_DIR, true);
    }
};

class DeviceDefaultConfigTest : public DefaultsConfigTest
{
public:
    DeviceDefaultConfigTest() : DefaultsConfigTest()
    {
        setenv(ENV_DEVICE_PATH, TEST_DEVICE_DIR, true);
    }
};

class CommonConfigTest : public DeviceDefaultConfigTest
{
public:
    CommonConfigTest() : DeviceDefaultConfigTest(),
                         m_platform(std::make_shared<DummyPlatform>("vendor1-device1", "Dummy Device")),
                         m_config(std::make_shared<Config>(m_platform))
    {};

    std::shared_ptr<Config> config() { return m_config; };

private:
    std::shared_ptr<Platform> m_platform;
    std::shared_ptr<Config> m_config;
};

TEST_F(BaseConfigTest, checkNoConfigFallbackValue)
{
    auto platform = std::make_shared<DummyPlatform>();
    auto config = std::make_shared<Config>(platform);

    // As long as we dont provide any default files
    // We should not "contain" anything info
    EXPECT_FALSE(config->contains("Name"));
    EXPECT_FALSE(config->contains("PrettyName"));

    // Test that get retruns "default" provided fallback value
    EXPECT_EQ(config->get("Name", "NoName"), "NoName");
    EXPECT_EQ(config->get("PrettyName", "No Name"), "No Name");
}

TEST_F(DefaultsConfigTest, checkDefaultsNoDeviceFound)
{
    auto platform = std::make_shared<DummyPlatform>();
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name", ""), "generic");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName", ""), "Generic device");
}


TEST_F(BaseConfigTest, checkPlatformDefaults)
{
    auto platform = std::make_shared<DummyPlatform>("Dummy", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name", ""), "Dummy");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName", ""), "Dummy Device");
}

TEST_F(DeviceConfigTest, checkContains)
{
    auto platform = std::make_shared<DummyPlatform>("Dummy", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_TRUE(config->contains("PrettyName"));

    EXPECT_FALSE(config->contains("ddssdsd"));
    EXPECT_FALSE(config->contains("343543257'[;p-;a"));
    EXPECT_FALSE(config->contains("435r 6 23k'le[ =-r0-thioj23d m,"));
}

TEST_F(DeviceConfigTest, testDeviceConfigOnly)
{
    auto platform = std::make_shared<DummyPlatform>("device1", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name", ""), "device1");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName", ""), "Device 1");
}

TEST_F(DeviceConfigTest, testVendorConfigOnly)
{
    auto platform = std::make_shared<DummyPlatform>("vendor1-device1", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name", ""), "vendor1-device1");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName", ""), "Vendor 1 Device 1");
}

TEST_F(DeviceDefaultConfigTest, testDeviceOverridesDefault)
{
    auto platform = std::make_shared<DummyPlatform>("vendor1-device1", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name"), "vendor1-device1");

    EXPECT_TRUE(config->contains("DetectedName"));
    EXPECT_EQ(config->get("DetectedName"), "vendor1-device1");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName"), "Vendor 1 Device 1");
}

TEST_F(DeviceDefaultConfigTest, testDeviceAlias)
{
    auto platform = std::make_shared<DummyPlatform>("device1-alias2", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name"), "device1");

    EXPECT_TRUE(config->contains("DetectedName"));
    EXPECT_EQ(config->get("DetectedName"), "device1-alias2");

    EXPECT_TRUE(config->contains("PrettyName"));
    EXPECT_EQ(config->get("PrettyName"), "Device 1");
}

TEST_F(DeviceDefaultConfigTest, testVectorGet)
{
    auto platform = std::make_shared<DummyPlatform>("vendor1-device1", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("SupportedOrientations"));
    EXPECT_EQ(config->get("SupportedOrientations"), "Portrait,InvertedPortrait,Landscape,InvertedLandscape");
}

TEST_F(DeviceDefaultConfigTest, testGetMoDefault)
{
    auto platform = std::make_shared<DummyPlatform>("vendor1-device1", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("Name"));
    EXPECT_EQ(config->get("Name"), "vendor1-device1");
}


TEST_F(DefaultsConfigTest, testLegacyProps)
{
    setenv(ENV_LEGACY_PATH, TEST_DEFAULT_DIR, true);

    auto platform = std::make_shared<DummyPlatform>("legacy", "Dummy Device");
    auto config = std::make_shared<Config>(platform);

    EXPECT_TRUE(config->contains("GridUnit"));
    EXPECT_EQ(config->get("GridUnit"), "16");

    EXPECT_TRUE(config->contains("WebkitDpr"));
    EXPECT_EQ(config->get("WebkitDpr"), "1.8");

    EXPECT_TRUE(config->contains("PrimaryOrientation"));
    EXPECT_EQ(config->get("PrimaryOrientation"), "InvertedLandscape");

    EXPECT_TRUE(config->contains("DeviceType"));
    EXPECT_EQ(config->get("DeviceType"), "tablet");
}
