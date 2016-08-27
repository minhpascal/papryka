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
 * @file        typename.h
 * @author      Ariel Kalingking  <akalingking@sequenceresearch.com>
 * @date        August 21, 2016 7:16 PM
 * @copyright   (c) 2016-2026 <www.sequenceresearch.com> 
 */
#pragma once
#include <string>
#include <cstdlib>
#include <cxxabi.h>

namespace papryka {

#ifdef _DEBUG
/**
 * @brief extracts type name at runtime, **DO NOT USE** in production memory useage will slow us down.
 * @return 
 */
template<typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), nullptr, nullptr, &status);
    if(status == 0) 
    {
        tname = demangled_name;
        std::free(demangled_name);
    }   
    return tname;
}
#else
template<typename T> std::string type_name() { return ""; }    
#endif

}