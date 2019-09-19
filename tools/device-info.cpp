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
#include <string>
#include <memory>
#include <iostream>
#include <vector>

void print(std::string str) {
    std::cout << str << std::endl;
}

int get(std::string arg, std::shared_ptr<DeviceInfo> info) {
    if (arg == "Name") {
        print(info->name());
        return 0;
    }
    if (arg == "PrettyName") {
        print(info->prettyName());
        return 0;
    }
    if (arg == "DeviceType") {
        print(DeviceInfo::deviceTypeToString(info->deviceType()));
        return 0;
    }
    if (arg == "DriverType") {
        print(DeviceInfo::driverTypeToString(info->driverType()));
        return 0;
    }
    if (arg == "GridUnit") {
        print(std::to_string(info->gridUnit()));
        return 0;
    }
    if (arg == "SupportedOrientations") {
        for (auto r : info->supportedOrientations())
            print(r);
        return 0;
    }
    if (arg == "PrimaryOrientation") {
        print(info->primaryOrientation());
        return 0;
    }
    if (arg == "PortraitOrientation") {
        print(info->portraitOrientation());
        return 0;
    }
    if (arg == "InvertedPortraitOrientation") {
        print(info->invertedPortraitOrientation());
        return 0;
    }
    if (arg == "LandscapeOrientation") {
        print(info->landscapeOrientation());
        return 0;
    }
    if (arg == "GridUnit") {
        print(std::to_string(info->gridUnit()));
        return 0;
    }
    if (info->contains(arg)) {
        print(info->get(arg, ""));
        return 0;
    }

    print("Unknown prop");
    return 1;
}

void help() {
    print("device-info usage:");
    print("device-info get [prop]");
}

int main (int argc, char *argv[]) {
    // We shoud not log anything as that might mess with scripts using this
    std::shared_ptr<DeviceInfo> info = std::make_shared<DeviceInfo>(DeviceInfo::PrintMode::None);

    if (argc == 3) {
        std::string argv1 = argv[1];
        if (argv1 == "get") {
            return get(std::string(argv[2]), info);
        }
        // TODO: add set function
    }

    help();
    return 1;
} 
