// SPDX-FileCopyrightText: 2020 Lutz Freitag
// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <string_view>

#include <time.h>

namespace cndl {

std::string mkdatestr(struct tm const& tm);
std::string mkdatestr(struct timespec const& ts);

struct tm parsedatestr(std::string_view str);

}
