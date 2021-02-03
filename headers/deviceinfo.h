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
#include <vector>

class Device;
class DeviceInfo {
public:
    enum DeviceType {
        Phone,
        Tablet,
        Desktop,
        Unknown = -1
    };

    enum DriverType {
        Linux,
        Halium
    };

    enum PrintMode {
        None = -2,
        Error = -1,
        Info = 0,
        Debug = 1,
        Verbose = 2
    };

    DeviceInfo(PrintMode printMode = PrintMode::Info);

    // For tests
    DeviceInfo(Device *device);

    // Internal logging
    void setPrintMode(PrintMode printMode);

    // Props with auto detections
    std::string name();
    std::string prettyName();
    DeviceType deviceType();
    DriverType driverType();
    int gridUnit();

    // Get props that does not have auto detection
    std::string get(std::string prop, std::string defaultValue);
    bool contains(std::string prop);

    // Handy wrappers around get
    std::vector<std::string> supportedOrientations();
    std::string primaryOrientation();
    std::string portraitOrientation();
    std::string invertedPortraitOrientation();
    std::string landscapeOrientation();
    std::string invertedLandscapeOrientation();

    // Handy helpers
    static std::string deviceTypeToString(DeviceType type);
    static DeviceType deviceTypeFromString(std::string str);
    static std::string driverTypeToString(DriverType type);
    static DriverType driverTypeFromString(std::string str);

private:
    std::shared_ptr<Device> m_device;
};
