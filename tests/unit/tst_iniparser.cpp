/*
 * Copyright (C) 2020 UBports foundation.
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
#include <memory>

#include "iniparser.h"

class IniParserTest : public ::testing::Test
{
public:
    IniParserTest()
    {
    }

    const std::shared_ptr<IniParser> m_iniParser{std::make_shared<IniParser>(TEST_DATA_DIR"/valid_inifile.conf")};
};

TEST(IniParser, checkValidIni)
{
    IniParser ini(TEST_DATA_DIR"/valid_inifile.conf");

    EXPECT_FALSE(ini.parseError());
}

TEST(IniParser, checkInvalidIni)
{
    IniParser ini(TEST_DATA_DIR"/invalid_inifile.conf");

    EXPECT_TRUE(ini.parseError());
}

TEST_F(IniParserTest, checkSections)
{
    EXPECT_EQ(m_iniParser->sections(), std::set<std::string>({
        "section", "section1", "section2", "section3"
    }));
}

TEST_F(IniParserTest, checkContains)
{
    EXPECT_TRUE(m_iniParser->contains("section", "var1"));
    EXPECT_TRUE(m_iniParser->contains("section", "var1"));
    EXPECT_TRUE(m_iniParser->contains("section1", "var5"));
}

TEST_F(IniParserTest, checkNotContains)
{
    EXPECT_FALSE(m_iniParser->contains("no-section", "novar"));
    EXPECT_FALSE(m_iniParser->contains("section", "novar"));

    EXPECT_FALSE(m_iniParser->contains("section", "var5"));
    EXPECT_FALSE(m_iniParser->contains("section", "var5"));
    EXPECT_FALSE(m_iniParser->contains("section1", "var1"));
}
