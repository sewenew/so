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

#include <string>
#include <iostream>
#include "record_reader.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: users_parser path_to_xml_file" << std::endl;
        return -1;
    }

    auto fields = {"Id", "Reputation", "DisplayName", "UpVotes", "DownVotes"};
    sw::so::RecordReader reader(argv[1], 1024 * 1024, fields.begin(), fields.end());

    size_t total = 0;
    size_t cnt = 0;
    size_t empty = 0;
    size_t err = 0;
    while (true) {
        try {
            ++total;
            auto record = reader.next();
            if (record.is_null()) {
                ++empty;
                continue;
            }

            std::cout << record << "\n";

            ++cnt;
        } catch (const sw::so::EofError &e) {
            break;
        } catch (const sw::so::Error &e) {
            ++err;
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    std::cerr << "total: " << total << ", cnt: " << cnt
        << ", empty: " << empty << ", err: " << err << std::endl;

    return 0;
}
