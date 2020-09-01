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

#define ENABLE_LEGACY 1

#include "config.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>

#ifdef ENABLE_LEGACY
#include <sstream>
#endif

#include "device.h"
#include "deviceinfo.h"
#include "iniparser.h"
#include "logger.h"
#include "platform/platform.h"

#define CONFIG_PATH "/etc/device-info/"
#define DEFAULT_CONFIG_PATH "default"
#define ALIAS_CONFIG_PATH "alias"
#define ALIAS_SECTION "alias"
#define DEFAULT_SECTION "default"
#define DEVICE_SECTION "device"

#define ENV_PATH "DEVICEINFO_CONFIG_PATH"
#define ENV_DEFAULT_PATH "DEVICEINFO_DEFAULT_CONFIG"
#define ENV_DEVICE_PATH "DEVICEINFO_DEVICE_CONFIG"
#define ENV_ALIAS_PATH "DEVICEINFO_ALIAS_CONFIG"
#define ENV_DEVICE_NAME "DEVICEINFO_DEVICE_NAME"

#ifdef ENABLE_LEGACY
#define LEGACY_GRID "GRID_UNIT_PX"
#define LEGACY_DPR "QTWEBKIT_DPR"
#define LEGACY_ORI "NATIVE_ORIENTATION"
#define LEGACY_FORM "FORM_FACTOR"

#define LEGACY_PATH "/etc/ubuntu-touch-session.d/"
#endif

Config::Config(std::shared_ptr<Platform> platform)
    : m_platform(platform)
{
    auto detectedName = getEnv(ENV_DEVICE_NAME, platform->name().c_str());

    // Transform to lowercase
    std::transform(detectedName.begin(), detectedName.end(), detectedName.begin(), ::tolower);

    Log::debug("Detected name: %s", detectedName.c_str());

    auto aliasPath = getEnv(ENV_ALIAS_PATH, nameToConfigPath(ALIAS_CONFIG_PATH));
    if (exists(aliasPath)) {
        IniParser aliasIni(aliasPath);
        if (aliasIni.contains(ALIAS_SECTION, detectedName)) {
            auto aliasName = aliasIni.get(ALIAS_SECTION, detectedName, detectedName);
            Log::verbose("Found alias %s", aliasName.c_str());
            if (exists(nameToConfigPath(aliasName))) {
                Log::debug("Using %s as alias for %s", aliasName.c_str(), detectedName.c_str());
                detectedName = aliasName;
            }
        }
    }

    auto devicePath = getEnv(ENV_DEVICE_PATH, nameToConfigPath(detectedName));
    if (exists(devicePath)) {
        Log::debug("Using %s for device config", devicePath.c_str());
        m_deviceIni = std::make_shared<IniParser>(devicePath);
    } else {
        Log::info("No device config found!");
    }

#ifdef ENABLE_LEGACY
    if (exists(nameToPath(detectedName, LEGACY_PATH)))
        getLegacyEnv(nameToPath(detectedName, LEGACY_PATH));
#endif

    auto defaultPath = getEnv(ENV_DEFAULT_PATH, nameToConfigPath(DEFAULT_CONFIG_PATH));
    if (exists(defaultPath)) {
        Log::debug("Using %s for default config", defaultPath.c_str());
        m_defaultIni = std::make_shared<IniParser>(defaultPath);
    } else {
        Log::info("No default config found!");
    }
}

bool Config::contains(std::string prop, bool defaults)
{
    if (defaults) {
        if (!m_defaultIni)
            return false;

        auto detected = DeviceInfo::deviceTypeToString(m_platform->deviceType());
        if (m_defaultIni->sections().count(detected)) {
            if (m_defaultIni->contains(detected, prop)) 
                return true;
        }
        auto driverType = DeviceInfo::driverTypeToString(m_platform->driverType());
        if (m_defaultIni->sections().count(driverType)) {
            if (m_defaultIni->contains(driverType, prop)) 
                return true;
        }
        return m_defaultIni->contains(DEFAULT_SECTION, prop);
    } else {
#ifdef ENABLE_LEGACY
        if (!toLegacy(prop).empty() && !!m_legacyEnv.count(toLegacy(prop))) {
            Log::debug("Legacy prop found!");
            return true;
        }
#endif

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

        auto detected = DeviceInfo::deviceTypeToString(m_platform->deviceType());
        if (m_defaultIni->sections().count(detected)) {
            if (m_defaultIni->contains(detected, prop)) 
                return m_defaultIni->get(detected, prop, defaultValue);
        }
        auto driverType = DeviceInfo::driverTypeToString(m_platform->driverType());
        if (m_defaultIni->sections().count(driverType)) {
            if (m_defaultIni->contains(driverType, prop)) 
                return m_defaultIni->get(driverType, prop, defaultValue);
        }
       return m_defaultIni->get(DEFAULT_SECTION, prop, defaultValue);
    } else {
        std::string ret = defaultValue;

#ifdef ENABLE_LEGACY
        if (!toLegacy(prop).empty() && !!m_legacyEnv.count(toLegacy(prop))) {
            Log::debug("Did not find new prop, but found legacy prop, using that");
            ret = m_legacyEnv.at(toLegacy(prop));
        }
#endif
        if (m_deviceIni && m_deviceIni->contains(DEVICE_SECTION, prop))
            ret = m_deviceIni->get(DEVICE_SECTION, prop, defaultValue);

        return ret;
    } 
}

bool Config::exists(std::string name)
{
    Log::verbose("Seeing if %s exists", name.c_str());
    std::ifstream infile(name);
    return infile.good();   
}

std::string Config::nameToConfigPath(std::string name)
{   
    std::string path(getEnv(ENV_PATH, CONFIG_PATH));
    path.append(name);
    path.append(".conf");
    return path;
}

std::string Config::nameToPath(std::string name, std::string confpath)
{
    std::string path(confpath);
    path.append(name);
    path.append(".conf");
    return path;
}

std::string Config::getEnv(const char *name, std::string dval)
{
    const char* env = getenv(name);
    if (env) {
         Log::verbose("Using provided env %s", env);
        return std::string(env);
    }

    return dval;
}

#ifdef ENABLE_LEGACY
// Legacy
std::string Config::toLegacy(std::string str)
{
    Log::verbose("toLegacy %s", str.c_str());
    if (str == "GridUnit")
        return LEGACY_GRID;
    if (str == "WebkitDpr")
        return LEGACY_DPR;
    if (str == "PrimaryOrientation")
        return LEGACY_ORI;
    if (str == "DeviceType")
        return LEGACY_FORM;
    return std::string();
}

std::vector<std::string> Config::split(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}

void Config::getLegacyEnv(std::string dfile)
{
    std::ifstream file(dfile);
    std::string str;
    while (std::getline(file, str))
    {
        auto splitted = split(str, *"=");
        m_legacyEnv[splitted[0]] = splitted[1];
        Log::verbose("Found legacy env %s = %s", splitted[0].c_str(), splitted[1].c_str());
    }
}
#endif
