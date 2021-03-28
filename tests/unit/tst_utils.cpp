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

#include <gtest/gtest.h>

#include "utils.h"

TEST(UtilsTest, testSplitString)
{
    auto spl = utils::string::split("1:2:3dfdf3:5", ':');

    EXPECT_EQ(spl, (std::vector<std::string>{
        "1",
        "2",
        "3dfdf3",
        "5"
    }));
}

TEST(UtilsTest, testJoin)
{
    auto str = utils::path::join("this", "that");

    EXPECT_EQ(str, "this/that");
}

TEST(UtilsTest, testJoinSuffix)
{
    auto str = utils::path::join("this", "that", ".end");

    EXPECT_EQ(str, "this/that.end");
}

TEST(UtilsTest, testJoinFileSuffix)
{
    auto str = utils::path::join("this", "that", "file", ".end");

    EXPECT_EQ(str, "this/that/file.end");
}

TEST(UtilsTest, testEnvGet)
{
    utils::env::SetUnsetAfter setUnset("DEVICEINFO_TESTING_ENV", "test_this");

    EXPECT_EQ(utils::env::get("DEVICEINFO_TESTING_ENV"), "test_this");
}

TEST(UtilsTest, testEnvSetAndUnset)
{
    EXPECT_TRUE(utils::env::set("DEVICEINFO_TESTING_ENV", "test_this"));
    EXPECT_EQ(utils::env::get("DEVICEINFO_TESTING_ENV"), "test_this");

    EXPECT_TRUE(utils::env::unset("DEVICEINFO_TESTING_ENV"));
    EXPECT_EQ(utils::env::get("DEVICEINFO_TESTING_ENV"), "");
}

TEST(UtilsTest, testEnvGetNoVal)
{
    EXPECT_EQ(utils::env::get("DEVICEINFO_TESTING_ENV", "test_no_val"), "test_no_val");
}
