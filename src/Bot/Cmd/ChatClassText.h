/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#pragma once

#include <string>

namespace ChatClassText
{
inline std::string FormatClass(std::string const& spec, std::string const& playerClass)
{
    return spec + " " + playerClass;
}
} // namespace ChatClassText
