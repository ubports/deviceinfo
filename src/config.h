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

#pragma once

#include <memory>
#include <yaml-cpp/yaml.h>

#define DEFAULT_CONFIG_FILE "default.yaml"

#define DEFAULT_SECTION "default"
#define DEVICE_SECTION "device"

#define ENV_CONFIG_PATH "DEVICEINFO_CONFIG_PATH"
#define ENV_DEFAULT_CONFIG "DEVICEINFO_DEFAULT_CONFIG"
#define ENV_DEVICE_PATH "DEVICEINFO_DEVICE_PATH"
#define ENV_DEVICE_NAME "DEVICEINFO_DEVICE_NAME"

#ifdef ENABLE_LEGACY
#define LEGACY_GRID "GRID_UNIT_PX"
#define LEGACY_DPR "QTWEBKIT_DPR"
#define LEGACY_ORI "NATIVE_ORIENTATION"
#define LEGACY_FORM "FORM_FACTOR"

#define LEGACY_PATH "/etc/ubuntu-touch-session.d/"
#define ENV_LEGACY_PATH "DEVICEINFO_LEGACY_PATH"

#endif

class Platform;
class IniParser;
class Config {
public:
    Config(std::shared_ptr<Platform> platform);

    std::string get(std::string prop, std::string defaultValue);
    std::string get(std::string prop);
    bool contains(std::string prop);

private:
    void mergeWithConfigNode(YAML::Node node);

#ifdef ENABLE_LEGACY
    void mergeLegacy(std::string device);
    std::string fromLegacy(std::string str);
#endif

    std::shared_ptr<Platform> m_platform;
    YAML::Node m_configNode;
};
