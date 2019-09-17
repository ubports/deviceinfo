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

#include "config.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>

#include "device.h"
#include "deviceinfo.h"
#include "iniparser.h"

#define CONFIG_PATH "/etc/device-info/"
#define DEFAULT_CONFIG_PATH "/etc/device-info/default.conf"
#define DEFAULT_SECTION "default"
#define DEVICE_SECTION "device"

Config::Config(Device *device)
    : m_device(device)
{
    if (exists(nameToConfigPath(device->detectName()))) {
        m_deviceIni = std::make_shared<IniParser>(nameToConfigPath(device->name()));
    }

    std::string path = DEFAULT_CONFIG_PATH;
    const char* env = getenv("DEVICE_INFO_CONFIG");
    if (env) {
        if (exists(env)) {
            path = std::string(env);
        } else {
            std::cout << "Did not find provided config, using default" << std::endl;
        }
    }
    
    if (exists(path)) {
        m_defaultIni = std::make_shared<IniParser>(path);
    }
}

bool Config::contains(std::string prop, bool defaults)
{
    if (defaults) {
        if (!m_defaultIni)
            return false;

        auto detected = DeviceInfo::deviceTypeToString(m_device->detectType(false));
        if (m_defaultIni->sections().count(detected)) {
            if (m_defaultIni->contains(detected, prop)) 
                return true;
        }
        auto driverType = DeviceInfo::driverTypeToString(m_device->driverType());
        if (m_defaultIni->sections().count(driverType)) {
            if (m_defaultIni->contains(driverType, prop)) 
                return true;
        }
        return m_defaultIni->contains(DEFAULT_SECTION, prop);
    } else {
        if (!m_deviceIni)
            return false;
        return m_deviceIni->contains(DEVICE_SECTION, prop); 
    }

}

std::string Config::get(std::string prop, bool defaults)
{
    return get(prop, defaults, "");
}

std::string Config::get(std::string prop, bool defaults, std::string defaultValue)
{
    if (defaults) {
        if (!m_defaultIni)
            return defaultValue;

        auto detected = DeviceInfo::deviceTypeToString(m_device->detectType(false));
        if (m_defaultIni->sections().count(detected)) {
            if (m_defaultIni->contains(detected, prop)) 
                return m_defaultIni->get(detected, prop, defaultValue);
        }
        auto driverType = DeviceInfo::driverTypeToString(m_device->driverType());
        if (m_defaultIni->sections().count(driverType)) {
            if (m_defaultIni->contains(driverType, prop)) 
                return m_defaultIni->get(driverType, prop, defaultValue);
        }
       return m_defaultIni->get(DEFAULT_SECTION, prop, defaultValue);
    } else {
        if (!m_deviceIni)
            return defaultValue;
        return m_deviceIni->get(DEVICE_SECTION, prop, defaultValue);
    } 
}

bool Config::exists(std::string name)
{
    std::ifstream infile(name);
    return infile.good();   
}

std::string Config::nameToConfigPath(std::string name)
{   
    std::string path(CONFIG_PATH);
    path.append(name);
    return path;
}
