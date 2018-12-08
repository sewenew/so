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

#include "record_reader.h"
#include <tinyxml2.h>
#include <string>

namespace sw {

namespace so {

nlohmann::json RecordReader::next() {
    auto line = _reader.getline();

    if (!_filter(line)) {
        return {};
    }

    tinyxml2::XMLDocument doc;
    if (doc.Parse(line.data(), line.size()) != tinyxml2::XML_SUCCESS) {
        throw Error("Failed to parse: " + std::string(line.data(), line.size()));
    }

    auto row = doc.FirstChildElement("row");
    if (row == nullptr) {
        return {};
    }

    nlohmann::json record;
    for (const auto &field : _fields) {
        auto value = row->Attribute(field.c_str());
        if (value != nullptr) {
            record[field] = value;
        }
    }

    return record;
}

bool RecordReader::_filter(const StringView &line) {
    for (auto idx = 0U; idx != line.size(); ++idx) {
        // By now, we only allow ASCII characters.
        auto c = *(line.data() + idx);
        if (static_cast<unsigned char>(c) >= 128) {
            return false;
        }
    }

    return true;
}

}

}
