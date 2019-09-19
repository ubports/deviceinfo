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

#include "deviceinfo.h"
#include <cstdarg>

class Logger {
public:
    static void setMode(DeviceInfo::PrintMode mode);
    static void log(DeviceInfo::PrintMode mode,  std::string &format);
    static void logv(DeviceInfo::PrintMode mode, char const* fmt, va_list va);
    static void log(DeviceInfo::PrintMode mode, char const* fmt, ...);
    static const char* modeToStr(DeviceInfo::PrintMode mode);

private:
    static DeviceInfo::PrintMode m_mode;
};

class Log {
public:
    static void error(std::string &m) { Logger::log(DeviceInfo::PrintMode::Error, m); }
    static void info(std::string &m) { Logger::log(DeviceInfo::PrintMode::Info, m); }
    static void debug(std::string &m) { Logger::log(DeviceInfo::PrintMode::Debug, m); }
    static void verbose(std::string &m) { Logger::log(DeviceInfo::PrintMode::Verbose, m); }

    template<typename... Args>
    static void error(const char* fmt, Args&&... args) { Logger::log(DeviceInfo::PrintMode::Error, fmt, std::forward<Args>(args)...); }
    template<typename... Args>
    static void info(const char* fmt, Args&&... args) { Logger::log(DeviceInfo::PrintMode::Info, fmt, std::forward<Args>(args)...); }
    template<typename... Args>
    static void debug(const char* fmt, Args&&... args) { Logger::log(DeviceInfo::PrintMode::Debug, fmt, std::forward<Args>(args)...); }
    template<typename... Args>
    static void verbose(const char* fmt, Args&&... args) { Logger::log(DeviceInfo::PrintMode::Verbose, fmt, std::forward<Args>(args)...); }
};
