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

int main (int argc, char *argv[]) {

    std::shared_ptr<DeviceInfo> info = std::make_shared<DeviceInfo>();

    std::string argv1 = argv[1];
    if (argv1 == "name") {
        print(info->name());
        return 0;
    }
    if (argv1 == "prettyName") {
        print(info->prettyName());
        return 0;
    }
    if (argv1 == "deviceType") {
        print(DeviceInfo::deviceTypeToString(info->deviceType()));
        return 0;
    }
    if (argv1 == "driverType") {
        print(DeviceInfo::driverTypeToString(info->driverType()));
        return 0;
    }
    if (argv1 == "gridUnit") {
        print(std::to_string(info->gridUnit()));
        return 0;
    }

    if (argv1 == "supportedOrientations") {
        print(info->supportedOrientations());
        return 0;
    }

    if (argv1 == "primaryOrientation") {
        print(info->primaryOrientation());
        return 0;
    }

    if (argv1 == "portraitOrientation") {
        print(info->portraitOrientation());
        return 0;
    }

    if (argv1 == "invertedPortraitOrientation") {
        print(info->invertedPortraitOrientation());
        return 0;
    }

    if (argv1 == "landscapeOrientation") {
        print(info->landscapeOrientation());
        return 0;
    }

    if (info->contains(argv1)) {
        print(info->get(argv1, ""));
    }

    print("Unknown prop");
    return 1;
} 
