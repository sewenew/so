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

#ifndef SEWENEW_SO_PREPARATION_SEVEN_ZIP_READER_H
#define SEWENEW_SO_PREPARATION_SEVEN_ZIP_READER_H

#include <memory>
#include <string>
#include <archive.h>
#include <archive_entry.h>
#include "utils.h"

namespace sw {

namespace so {

class SevenZipReader {
public:
    SevenZipReader(const std::string &filename, std::size_t block_size);

    StringView getline();

private:
    StringView _getline();

    void _read();

    struct ArchiveDeleter {
        void operator()(archive *ar) const {
            if (ar != nullptr) {
                archive_read_free(ar);
            }
        }
    };

    using ArchiveUPtr = std::unique_ptr<archive, ArchiveDeleter>;

    ArchiveUPtr _archive;

    archive_entry *_entry = nullptr;

    std::size_t _block_size = 0;

    std::string _buffer;

    std::size_t _idx = 0;

    static const std::string NEWLINE;
};

}

}

#endif // end SEWENEW_SO_PREPARATION_SEVEN_ZIP_READER_H
