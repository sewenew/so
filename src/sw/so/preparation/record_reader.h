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

#ifndef SEWENEW_SO_PREPARATION_RECORD_READER_H
#define SEWENEW_SO_PREPARATION_RECORD_READER_H

#include <string>
#include <initializer_list>
#include <nlohmann/json.hpp>
#include "seven_zip_reader.h"

namespace sw {

namespace so {

class RecordReader {
public:
    template <typename Input>
    RecordReader(const std::string &filename,
                    std::size_t block_size,
                    Input first,
                    Input last) : _reader(filename, block_size), _fields(first, last) {}

    nlohmann::json next();

private:
    bool _filter(const StringView &line);

    SevenZipReader _reader;

    std::vector<std::string> _fields;
};

}

}

#endif // end SEWENEW_SO_PREPARATION_RECORD_READER_H
