/**************************************************************************
   Copyright (c) 2018 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_SO_PREPARATION_UTILS_H
#define SEWENEW_SO_PREPARATION_UTILS_H

#include <cstring>
#include <exception>
#include <string>

namespace sw {

namespace so {

// By now, not all compilers support std::string_view,
// so we make our own implementation.
class StringView {
public:
    constexpr StringView() noexcept = default;

    constexpr StringView(const char *data, std::size_t size) : _data(data), _size(size) {}

    StringView(const char *data) : _data(data), _size(std::strlen(data)) {}

    StringView(const std::string &str) : _data(str.data()), _size(str.size()) {}

    constexpr StringView(const StringView &) noexcept = default;

    StringView& operator=(const StringView &) noexcept = default;

    constexpr const char* data() const noexcept {
        return _data;
    }

    constexpr std::size_t size() const noexcept {
        return _size;
    }

private:
    const char *_data = nullptr;
    std::size_t _size = 0;
};

class Error : public std::exception {
public:
    explicit Error(const std::string &msg) : _msg(msg) {}

    Error(const Error &) = default;
    Error& operator=(const Error &) = default;

    Error(Error &&) = default;
    Error& operator=(Error &&) = default;

    virtual ~Error() = default;

    virtual const char* what() const noexcept {
        return _msg.data();
    }

private:
    std::string _msg;
};

class EofError : public Error {
public:
    explicit EofError() : Error("EOF") {}

    EofError(const EofError &) = default;
    EofError& operator=(const EofError &) = default;

    EofError(EofError &&) = default;
    EofError& operator=(EofError &&) = default;

    virtual ~EofError() = default;
};

}

}

#endif // end SEWENEW_SO_PREPARATION_UTILS_H
