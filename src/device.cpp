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

#include "device.h"

#include "config.h"
#include "logger.h"
#include "platform/platform.h"

#include <memory>
#include <string>
#include <iterator>
#include <fstream>
#include <string.h>

#if HAVE_PROPS
#include <hybris/properties/properties.h>
#endif

// For hybris
#define DEVICE_PROP_KEY "ro.product.device"
#define MODEL_PROP_KEY "ro.product.model"
#define VENDOR_DEVICE_PROP_KEY "ro.product.vendor.device"
#define VENDOR_MODEL_PROP_KEY "ro.product.vendor.model"
#define CHARA_PROP_KEY "ro.build.characteristics"

// For linux
#define LINUX_MODEL "/proc/device-tree/model"

Device::Device() :
    m_platform(Platform::create()),
    m_config(std::make_shared<Config>(m_platform))
{
}

std::string Device::name()
{
    // If all else fails, platform returns default value for the
    // current platform use this if config has no set value
    return m_config->get("Name", m_platform->name());
}

std::string Device::prettyName()
{
    // If all else fails, platform returns default value for the
    // current platform use this if config has no set value
    return m_config->get("PrettyName", m_platform->prettyName());
}

int Device::gridUnit()
{
    auto str = get("GridUnit", "8");
    Log::verbose("Got gridunit str: %s", str.c_str());
    try {
        return stoi(str);
    } catch(...) {
        Log::error("GridUnit IS NOT A VALID NUMBER got %s", str.c_str());
        return 8;
    }
}

std::string Device::get(std::string prop, std::string defaultValue) {
    return m_config->get(prop, defaultValue);
}

bool Device::contains(std::string prop) {
    return m_config->contains(prop);
}

DeviceInfo::DeviceType Device::deviceType()
{
    auto defaultValue = DeviceInfo::deviceTypeToString(m_platform->deviceType());
    return DeviceInfo::deviceTypeFromString(m_config->get("DeviceType", defaultValue));
}

DeviceInfo::DriverType Device::driverType()
{
    return m_platform->driverType();
}
