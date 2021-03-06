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
 * @file        strategy.cpp
 * @author      Ariel Kalingking  <akalingking@sequenceresearch.com>
 * @date        August 7, 2016 1:59 AM
 * @copyright   (c) 2016-2026 <www.sequenceresearch.com> 
 */
#include <gtest/gtest.h>
#include <papryka/papryka.h>
#include <papryka/exchange/exchange.h>
#include <papryka/feed/feedsynthetic.h>
#include <papryka/strategy/strategy.h>
#include <exception>
#include <string>
#include <cassert>

using namespace papryka;

class MyStrategy : public Strategy
{
public:
    typedef Strategy base_t;
    typedef base_t::exchange_ptr_t exchange_ptr_t;
    typedef base_t::value_t value_t;
    typedef base_t::values_t values_t;
    std::string symbol;
    
    MyStrategy(exchange_ptr_t exchange, const std::string& symbol) :    
            Strategy(exchange), symbol(symbol)
    {}
    
    void on_bars(const datetime_t& datetime, const values_t& bars)
    {
        try
        {
            const value_t& bar = bars.at(symbol);
            log_debug("MyStrategy::{0:} close={1:0.3f}", __func__, bar.close);
        }
        catch (std::exception& e)
        {
            log_error("Strategy::{} {}", __func__, e.what());
        }
    } 
};

TEST(Strategy, Strategy)
{
    std::string symbol = "GOOG";
    datetime_t end = Clock::now();
    datetime_t start = end - std::chrono::days(100);
    
    typedef FeedSynthetic<Bar> feed_t;
    std::shared_ptr<feed_t> feed(new feed_t(start, end, Frequency::Day));
    Bar goog_spot(771,773,770,772,1350000);
    feed_t::data_t data = {{"GOOG", goog_spot, 0.2, 0.1}};
    feed->add_values_from_generator(data);
    
    Exchange::ptr_t exchange(new Exchange(feed, 1000));
   
    MyStrategy strategy(exchange, symbol); 
    strategy.run();
}