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

#include <memory>
#include <sstream>
#include <string>

DeviceInfo::DeviceInfo():
    m_device(std::make_shared<Device>())
{
}

std::string DeviceInfo::name()
{
    return m_device->name();
}

std::string DeviceInfo::prettyName()
{
    return m_device->prettyName();
}

DeviceType DeviceInfo::deviceType()
{
    return m_device->deviceType();
}

DriverType DeviceInfo::driverType()
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

std::string DeviceInfo::deviceTypeToString(DeviceType type) {
    switch(type) {
        case DeviceType::Phone:
            return "phone";
        case DeviceType::Tablet:
            return "tablet";
        case DeviceType::Desktop:
        default:
            return "desktop";
    }
}

DeviceType DeviceInfo::deviceTypeFromString(std::string str) {
    if (str == "phone") 
        return DeviceType::Phone;
    if (str == "tablet") 
        return DeviceType::Tablet;
    return DeviceType::Desktop;
}

std::string DeviceInfo::driverTypeToString(DriverType type) {
    switch(type) {
        case DriverType::Halium:
            return "halium";
        case DriverType::Linux:
        default:
            return "linux";
    }
}

DriverType DeviceInfo::driverTypeFromString(std::string str) {
    if (str == "halium") 
        return DriverType::Halium;
    return DriverType::Linux;
}
