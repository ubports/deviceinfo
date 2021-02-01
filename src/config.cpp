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

// ENABLE_LEGACY set by cmake
// CONFIG_PATH set by cmake
// DEVICES_PATH set by cmake

#include "config.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <dirent.h>
#include <string.h>
#include <yaml-cpp/yaml.h>

#include "device.h"
#include "deviceinfo.h"
#include "logger.h"
#include "utils.h"
#include "platform/platform.h"


namespace {

std::string getConfigPath() {
    return utils::env::get(ENV_CONFIG_PATH, CONFIG_PATH);
}

std::string getDevicePath() {
    return utils::env::get(ENV_DEVICE_PATH, DEVICES_PATH);
}

std::string getDefaultFile() {
    return utils::path::join(getConfigPath(), DEFAULT_CONFIG_FILE);
}

};

Config::Config(std::shared_ptr<Platform> platform)
    : m_platform(platform)
{
    auto detectedName = utils::env::get(ENV_DEVICE_NAME, platform->name().c_str());

    // Transform to lowercase
    std::transform(detectedName.begin(), detectedName.end(), detectedName.begin(), ::tolower);

    Log::debug("Detected name: %s", detectedName.c_str());

    // Lets try to find the devices config
    auto devicePath = getDevicePath();
    if (utils::path::exists(devicePath)) {
        DIR *dir;
        struct dirent *diread;
        std::vector<char *> files;

        if ((dir = opendir(devicePath.c_str())) != nullptr) {
            while ((diread = readdir(dir)) != nullptr && m_configNode.IsNull()) {
                if (!strcmp(diread->d_name, ".") || !strcmp(diread->d_name, ".."))
                    continue;

                auto file = utils::path::join(devicePath, std::string(diread->d_name));
                Log::verbose("Trying to load yaml file: %s", file.c_str());

                auto yaml = YAML::LoadFile(file);
                for (auto a : yaml) {
                    if (a.first.IsDefined() && a.first.as<std::string>() == detectedName) {
                        m_configNode = std::move(a.second);
                        if (!m_configNode["Name"].IsDefined())
                           m_configNode["Name"] = detectedName;
                    } else if (a.second["Names"].IsDefined()) {
                        auto names = a.second["Names"].as<std::vector<std::string>>();
                        if (std::find(std::begin(names), std::end(names), detectedName) != std::end(names)) {
                            m_configNode = std::move(a.second);
                            if (!m_configNode["Name"].IsDefined())
                                m_configNode["Name"] = detectedName;
                        }
                    }
                }
            }
            closedir(dir);
        }
    }

    if (!m_configNode.IsNull())
        Log::debug("Found device yaml with name: %s", m_configNode["Name"].as<std::string>().c_str());
    else
        Log::info("No device yaml config found!");

    // Lets merge platfrom detected names
    // Override if valid
    if (m_platform->hasValidName() && !contains("Name"))
        m_configNode["Name"] = m_platform->name();

    if (m_platform->hasValidPrettyName() && !contains("PrettyName"))
        m_configNode["PrettyName"] = m_platform->prettyName();

#ifdef ENABLE_LEGACY
    mergeLegacy(detectedName);
#endif

    auto defaultPath = getDefaultFile();
    if (utils::path::exists(defaultPath)) {
        Log::debug("Using %s for default config", defaultPath.c_str());
        auto defaultNode = YAML::LoadFile(defaultPath)["defaults"];

        // Lets merge all nodes to one
        // First we merge defaults from driverType
        auto driverType = DeviceInfo::driverTypeToString(m_platform->driverType());
        mergeWithConfigNode(defaultNode[driverType]);

        std::string type = "desktop";
        if (m_configNode["DeviceType"].IsDefined())
            type = m_configNode["DeviceType"].as<std::string>();
        else if (defaultNode["default"]["DeviceType"].IsDefined())
            type = defaultNode["default"]["DeviceType"].as<std::string>();

        mergeWithConfigNode(defaultNode[type]);
        mergeWithConfigNode(defaultNode["default"]);
    } else {
        Log::info("No default config found!");
    }
}

void Config::mergeWithConfigNode(YAML::Node node)
{
    for (auto n : node) {
        if (!m_configNode[n.first.as<std::string>()].IsDefined()) {
            Log::debug("MergeWithConfigNode value: %s", n.first.as<std::string>().c_str());
            m_configNode[n.first.as<std::string>()] = n.second;
        }
    }
}

bool Config::contains(std::string prop)
{
    return m_configNode[prop].IsDefined();
}

std::string Config::get(std::string prop)
{
    return get(prop, "");
}

std::string Config::get(std::string prop, std::string defaultValue)
{
    if (!contains(prop))
        return defaultValue;

    // TODO: allow to return vectors
    if (m_configNode[prop].IsSequence()) {
        std::string ret;
        auto siz = m_configNode[prop].size();
        for (std::size_t i=0;i<siz;i++) {
            ret.append(m_configNode[prop][i].as<std::string>());
            if (i != siz)
                ret.append(",");
        }
        return ret;
    }

    return m_configNode[prop].as<std::string>();
}

#ifdef ENABLE_LEGACY
// Legacy
void Config::mergeLegacy(std::string device)
{
    std::string dfile = LEGACY_PATH;
    dfile.append(device);
    dfile.append(".conf");
    if (!utils::path::exists(dfile)) {
        Log::debug("Did not find legacy file: %s", dfile.c_str());
        return;
    }

    std::ifstream file(dfile);
    std::string str;
    std::map<std::string, std::string> legacyEnv;

    while (std::getline(file, str))
    {
        auto splitted = utils::string::split(str, *"=");
        legacyEnv[splitted[0]] = splitted[1];
        Log::verbose("Found legacy env %s = %s", splitted[0].c_str(), splitted[1].c_str());
    }

    for (auto prop : legacyEnv) {
        auto key = toLegacy(prop.first);
        if (!key.empty() && contains(key)) {
            Log::debug("Merging legacy prop as it's not set by devicefile: %s", key.c_str());
            m_configNode[key] = prop.second;
        }
    }
}

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
#endif
