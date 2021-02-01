/*
 * Copyright (C) 2021 UBports foundation.
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

#include <string>
#include <vector>

namespace utils {
namespace path {

std::string join(std::string first, std::string second);
std::string join(std::string first, std::string second, std::string file, std::string suffix);
bool exists(std::string name);

}; // path

namespace env {

std::string get(const char *name, std::string dval);

}; // env

namespace string {

std::vector<std::string> split(std::string strToSplit, char delimeter);

}; // string
}; // utils