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

#include "logger.h"

#include <iostream>
#include <cstdio>

DeviceInfo::PrintMode Logger::m_mode = DeviceInfo::PrintMode::Info;

void Logger::setMode(DeviceInfo::PrintMode mode) {
     m_mode = mode;
}

void Logger::log(DeviceInfo::PrintMode mode, std::string &format) {
    if (mode > m_mode)
        return;

    std::cout << "[" << modeToStr(mode) << "] " << format << std::endl;
}

void Logger::logv(DeviceInfo::PrintMode mode, char const* fmt, va_list va)
{
    if (mode > m_mode)
        return;

    char message[1024];
    int max = sizeof(message) - 1;
    int len = vsnprintf(message, max, fmt, va);
    if (len > max)
        len = max;
    message[len] = '\0';

    std::cout << "[" << modeToStr(mode) << "] " << message << std::endl;
}

void Logger::log(DeviceInfo::PrintMode mode, char const* fmt, ...)
{
    if (mode > m_mode)
        return;

    va_list va;
    va_start(va, fmt);
    logv(mode, fmt, va);
    va_end(va);
}

const char* Logger::modeToStr(DeviceInfo::PrintMode mode)
{
    switch (mode) {
        case DeviceInfo::PrintMode::Error: return "error";
        case DeviceInfo::PrintMode::Info: return "info";
        case DeviceInfo::PrintMode::Debug: return "debug";
        case DeviceInfo::PrintMode::Verbose: return "verbose";
        default: return "wat";
    }
}
