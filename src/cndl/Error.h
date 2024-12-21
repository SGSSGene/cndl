// SPDX-FileCopyrightText: 2020 Lutz Freitag
// SPDX-License-Identifier: MIT
#pragma once

#include <stdexcept>

namespace cndl {

struct Error : std::runtime_error {
    Error(int code, char const* message)
    : std::runtime_error(message)
    , m_code{code}
    {}

    Error(int code) : Error(code, "") {}

    int code() const {
        return m_code;
    }
private:
    int m_code;
};

}
