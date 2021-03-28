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

#include "halium.h"

#include <memory>
#include <string>
#include <cstring>

#include <hybris/properties/properties.h>

// Android props
#define DEVICE_PROP_KEY "ro.product.device"
#define MODEL_PROP_KEY "ro.product.model"
#define VENDOR_DEVICE_PROP_KEY "ro.product.vendor.device"
#define VENDOR_MODEL_PROP_KEY "ro.product.vendor.model"
#define CHARA_PROP_KEY "ro.build.characteristics"

std::string platform::Halium::name()
{
    if (hasHaliumProp(VENDOR_DEVICE_PROP_KEY))
        return getHaliumProp(VENDOR_DEVICE_PROP_KEY);
    else if (hasHaliumProp(DEVICE_PROP_KEY))
        return getHaliumProp(DEVICE_PROP_KEY);

    return "unknown";
}

bool platform::Halium::hasValidName()
{
    return hasHaliumProp(VENDOR_DEVICE_PROP_KEY) ||
           hasHaliumProp(DEVICE_PROP_KEY);
}

std::string platform::Halium::prettyName()
{
    if (hasHaliumProp(VENDOR_MODEL_PROP_KEY))
        return getHaliumProp(VENDOR_MODEL_PROP_KEY);
    else if (hasHaliumProp(MODEL_PROP_KEY))
        return getHaliumProp(MODEL_PROP_KEY);

    return "unknown";
}

bool platform::Halium::hasValidPrettyName()
{
    return hasHaliumProp(VENDOR_MODEL_PROP_KEY) ||
           hasHaliumProp(MODEL_PROP_KEY);
}

DeviceInfo::DeviceType platform::Halium::deviceType() {
    auto chara = getHaliumProp(CHARA_PROP_KEY);
    if (chara.find("tablet") != std::string::npos)
        return DeviceInfo::DeviceType::Tablet;

    // As this is a halium device, the best guess will be phone
    return DeviceInfo::DeviceType::Phone;
}

bool platform::Halium::hasValidDeviceType()
{
    return hasHaliumProp(CHARA_PROP_KEY);
}

DeviceInfo::DriverType platform::Halium::driverType()
{
    return DeviceInfo::DriverType::Halium;
}

std::string platform::Halium::getHaliumProp(const char* prop)
{
    return getHaliumProp(prop, "");
}

std::string platform::Halium::getHaliumProp(const char* prop, const char* default_value)
{
    char value[PROP_VALUE_MAX];
    property_get(prop, value, default_value);
    return std::string(value);
}

bool platform::Halium::hasHaliumProp(const char* key)
{
    char const* default_value = "hasnodevice";
    char value[PROP_VALUE_MAX];
    property_get(key, value, default_value);
    return strcmp(value, default_value) != 0;
}

bool platform::Halium::usable()
{
    return hasHaliumProp(VENDOR_DEVICE_PROP_KEY) ||
           hasHaliumProp(DEVICE_PROP_KEY);
}
