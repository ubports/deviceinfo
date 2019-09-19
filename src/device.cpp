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
#define CHARA_PROP_KEY "ro.build.characteristics"

// For linux
#define LINUX_MODEL "/proc/device-tree/model"

Device::Device() :
    m_isHalium(hasHaliumProp(DEVICE_PROP_KEY)),
    m_config(std::make_shared<Config>(this))
{
}

std::string Device::name()
{
    if (m_config->contains("Name", false)) {
        return m_config->get("Name", false);
    }

    auto detect = detectName();
    if (detect != "unknown")
        return detect;

    // If all else fails
    return m_config->get("Name", true, "generic");
}

std::string Device::prettyName()
{
    if (m_config->contains("PrettyName", false)) {
        return m_config->get("PrettyName", false);
    }

    if (m_isHalium && hasHaliumProp(MODEL_PROP_KEY)) {
        return getHaliumProp(MODEL_PROP_KEY);
    }

    // If all else fails
    return m_config->get("PrettyName", true, "Generic device");
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
    if (m_config->contains(prop, false)) {
        Log::verbose("get found in device config");
        return m_config->get(prop, false);
    }
    return m_config->get(prop, true, defaultValue);
}

bool Device::contains(std::string prop) {
    if (m_config->contains(prop, false)) {
        return true;
    }
    return m_config->contains(prop, true);
}


DeviceInfo::DeviceType Device::deviceType()
{
    if (m_config->contains("DeviceType", false)) {
        auto typeStr = m_config->get("DeviceType", false);
        return DeviceInfo::deviceTypeFromString(typeStr);
    }

    auto detected = detectType(true);
    if (detected != DeviceInfo::DeviceType::Unknown)
        return detected;

    return DeviceInfo::deviceTypeFromString(m_config->get("DeviceType", true, "desktop"));
}

std::string Device::detectName() {
    if (m_isHalium && hasHaliumProp(DEVICE_PROP_KEY)) {
        return getHaliumProp(DEVICE_PROP_KEY);
    }

    // If it's not halium or does not have device prop, try dtb
    auto model = readFile(LINUX_MODEL);
    if (!model.empty())
        return model;

    return "unknown";
}

DeviceInfo::DeviceType Device::detectType(bool returnUknown) {
    if (m_isHalium) {
        auto chara = getHaliumProp(CHARA_PROP_KEY);
        if (chara.find("tablet") != std::string::npos)
            return DeviceInfo::DeviceType::Tablet;

        // As this is a halium device, the best guess will be phone
        return DeviceInfo::DeviceType::Phone;
    }

    // At this point we guess it's a desktop
    return returnUknown ? DeviceInfo::DeviceType::Unknown : DeviceInfo::DeviceType::Desktop;
}

DeviceInfo::DriverType Device::driverType()
{
    if (m_isHalium)
        return DeviceInfo::DriverType::Halium;

    return DeviceInfo::DriverType::Linux;
}

std::string Device::getHaliumProp(const char* prop)
{
    return getHaliumProp(prop, "");
}

std::string Device::getHaliumProp(const char* prop, const char* default_value)
{
    std::string ret;
#ifdef HAVE_PROPS
    char value[PROP_VALUE_MAX];
    property_get(prop, value, default_value);
    ret = value;
#endif
    return ret;
}

bool Device::hasHaliumProp(const char* key)
{
    bool ret = false;
#ifdef HAVE_PROPS
    char const* default_value = "hasnodevice";
    char value[PROP_VALUE_MAX];
    property_get(key, value, default_value);
    ret = strcmp(value, default_value) != 0;
#endif
    return ret;
}

std::string Device::readFile(std::string file)
{
    std::ifstream model(file);
    std::string ret;
    if (model.good())
        ret = std::string(std::istreambuf_iterator<char>{model},
                          std::istreambuf_iterator<char>());
    return ret;
}
