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

#include "deviceinfo.h"
#include "device.h"
#include "logger.h"

#include <memory>
#include <sstream>
#include <string>

#define ENV_DEBUG "DEVICEINFO_DEBUG"

DeviceInfo::DeviceInfo(PrintMode printMode)
{
    Logger::setMode(printMode);

    auto env = getenv(ENV_DEBUG);
    if (env) {
        try {
            Logger::setMode((PrintMode) std::stoi(env));
        } catch(...){}
    }
    
    // Needs to happen after we set print mode
    m_device = std::make_shared<Device>();
}

std::string DeviceInfo::name()
{
    return m_device->name();
}

std::string DeviceInfo::prettyName()
{
    return m_device->prettyName();
}

DeviceInfo::DeviceType DeviceInfo::deviceType()
{
    return m_device->deviceType();
}

DeviceInfo::DriverType DeviceInfo::driverType()
{
    return m_device->driverType();
}

int DeviceInfo::gridUnit() {
    return m_device->gridUnit();
}

std::string DeviceInfo::get(std::string prop, std::string defaultValue)
{
    return m_device->get(prop, defaultValue);
}

bool DeviceInfo::contains(std::string prop)
{
    return m_device->contains(prop);
}

std::vector<std::string> DeviceInfo::supportedOrientations() {
    auto str = get("SupportedOrientations", "Portrait,InvertedPortrait,Landscape,InvertedLandscape");
    std::vector<std::string> ret;
    std::string tmp;
    std::istringstream istr(str);
    while (std::getline(istr, tmp, *","))
    {
        ret.push_back(tmp);
    }
    return ret;
}

std::string DeviceInfo::primaryOrientation() {
    return get("PrimaryOrientation", "Portrait");
}

std::string DeviceInfo::portraitOrientation() {
    return get("PortraitOrientation", "Portrait");
}

std::string DeviceInfo::invertedPortraitOrientation() {
    return get("InvertedPortraitOrientation", "InvertedPortrait");
}

std::string DeviceInfo::landscapeOrientation() {
    return get("LandscapeOrientation", "Landscape");
}

std::string DeviceInfo::invertedLandscapeOrientation() {
    return get("InvertedLandscapeOrientation", "InvertedLandscape");
}

std::string DeviceInfo::deviceTypeToString(DeviceInfo::DeviceType type) {
    switch(type) {
        case DeviceInfo::DeviceType::Phone:
            return "phone";
        case DeviceInfo::DeviceType::Tablet:
            return "tablet";
        case DeviceInfo::DeviceType::Desktop:
        default:
            return "desktop";
    }
}

DeviceInfo::DeviceType DeviceInfo::deviceTypeFromString(std::string str) {
    if (str == "phone") 
        return DeviceInfo::DeviceType::Phone;
    if (str == "tablet") 
        return DeviceInfo::DeviceType::Tablet;
    return DeviceInfo::DeviceType::Desktop;
}

std::string DeviceInfo::driverTypeToString(DeviceInfo::DriverType type) {
    switch(type) {
        case DeviceInfo::DriverType::Halium:
            return "halium";
        case DeviceInfo::DriverType::Linux:
        default:
            return "linux";
    }
}

DeviceInfo::DriverType DeviceInfo::driverTypeFromString(std::string str) {
    if (str == "halium") 
        return DeviceInfo::DriverType::Halium;
    return DeviceInfo::DriverType::Linux;
}
