/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @file        date.cpp
 * @author      Ariel Kalingking  <akalingking@sequenceresearch.com>
 * @date        July 23, 2016 1:19 AM
 * @copyright   (c) 2016-2027 <www.sequenceresearch.com>
 */
#include <gtest/gtest.h>
#include <papryka/detail/date.h>
 
using namespace papryka;
TEST(Detail, date)
{
    datetime_t date = Clock::now();
    std::cout << "date " << to_str(date) << std::endl;
}
