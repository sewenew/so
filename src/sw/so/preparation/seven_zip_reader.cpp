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

#include "seven_zip_reader.h"
#include <cassert>
#include <vector>

namespace sw {

namespace so {

const std::string SevenZipReader::NEWLINE = "\r\n";

SevenZipReader::SevenZipReader(const std::string &filename,
                                std::size_t block_size) :
                                    _archive(archive_read_new()),
                                    _block_size(block_size) {
    if (_archive == nullptr) {
        throw Error("Bad alloc: failed to create archive reader");
    }

    if (archive_read_support_format_7zip(_archive.get()) != ARCHIVE_OK) {
        throw Error("Failed to add 7zip support");
    }

    if (archive_read_open_filename(_archive.get(), filename.c_str(), _block_size) != ARCHIVE_OK) {
        throw Error("Failed to open " + filename);
    }
}

StringView SevenZipReader::getline() {
    while (true) {
        if (_entry == nullptr) {
            _buffer.clear();
            auto res = archive_read_next_header(_archive.get(), &_entry);
            if (res == ARCHIVE_EOF) {
                throw EofError();
            }
            
            if (res != ARCHIVE_OK) {
                throw Error("Failed to read entry");
            }
        }

        try {
            return _getline();
        } catch (const EofError &e) {
            if (!_buffer.empty()) {
                _entry = nullptr;
                return _buffer;
            }
        }
    }
}

StringView SevenZipReader::_getline() {
    assert(_entry != nullptr);

    while (true) {
        if (_idx == 0) {
            _read();
        }

        auto pos = _buffer.find(NEWLINE, _idx);
        if (pos == std::string::npos) {
            if (_idx > 0) {
                _buffer = _buffer.substr(_idx);
                _idx = 0;
            }
            continue;
        } else {
            auto sv = StringView(_buffer.data() + _idx, pos - _idx);
            _idx = pos + NEWLINE.size();

            return sv;
        }
    }
}

void SevenZipReader::_read() {
    std::vector<char> buffer(_block_size);
    auto size = archive_read_data(_archive.get(), buffer.data(), buffer.size());
    if (size < 0) {
        throw Error("Failed to read data from archive.");
    }

    if (size == 0) {
        // Finish reading.
        throw EofError();
    }

    // size > 0
    _buffer.append(buffer.data(), buffer.data() + size);
}

}

}
