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
 * @file        detail/order.h
 * @author      Ariel Kalingking  <akalingking@sequenceresearch.com>
 * @date        July 23, 2016 1:40 AM
 * @copyright   (c) 2016-2026 <www.sequenceresearch.com>
 */
#pragma once
#include "../../detail/types.h"
#include "../../detail/date.h"
#include "../../detail/precision.h"
#include "../../detail/logger.h"
#include <memory>
#include <string>
#include <cassert>

namespace papryka {
namespace detail {
    
    namespace {
        template<typename _T> 
        struct order_names_ { 
            static const char* types[];
            static const char* actions[];
            static const char* states[];
            static const char* errors[];
            static const char* events[];
        };
        template<typename _T> const char* order_names_<_T>::types[] =   { "TypeNone", "Market", "Limit", "Stop", "StopLimit" };
        template<typename _T> const char* order_names_<_T>::actions[] = { "StateNone", "Initial", "Submitted", "Accepted", "Canceled", "PartiallyFilled", "Filled" };
        template<typename _T> const char* order_names_<_T>::states[] =  { "ActionNone", "Buy", "BuyToCover", "Sell", "SellShort" };
        template<typename _T> const char* order_names_<_T>::errors[] =  {"ErrorNone", "OrderCanceled"};
        template<typename _T> const char* order_names_<_T>::events[] =  { "TypeNone", "Accepted", "Canceled", "PartiallyFilled" };
        typedef order_names_<void> order_names_t;
    }
    
    /**
     * @brief   Order state machine
     */
    class Order
    {
    public:
        struct Event;
        struct Info;
        
        typedef Bar bar_t;
        typedef Event event_t;
        typedef Info info_t;
        typedef std::shared_ptr<event_t> event_ptr_t;
        typedef std::shared_ptr<info_t> info_ptr_t;
        
        enum Type { TypeNone=0, Market=1, Limit=2, Stop=3, StopLimit=4 };
        enum Action { ActionNone=0, Buy=1, BuyToCover=2, Sell=3, SellShort=4 };
        enum State { StateNone=0, Initial=1, Submitted=2, Accepted=3, Canceled=4, PartiallyFilled=5, Filled=6 };
        static const char* to_str(Type type) { return order_names_t::types[type]; }
        static const char* to_str(Action action) { return order_names_t::actions[action]; }
        static const char* to_str(State state) { return order_names_t::states[state]; }

    public:
        Type type;
        Action action;
        State state;
        std::string symbol;
        size_t quantity;
        info_ptr_t info;
        bool is_good_till_canceled;
        bool is_all_or_none;
        real_t commissions;
        datetime_t accepted_date;
        datetime_t submitted_date;
        uint32_t id;
        real_t filled;
        real_t avg_fill_price;
        
        inline void set_submitted(uint32_t id, const datetime_t& datetime);
        inline void switch_state(State newState);
        inline size_t get_remaining() const { return quantity - filled;  }
        inline void add_info(info_ptr_t& info);
        
        bool is_buy() const { return action == Buy || action == BuyToCover; }
        bool is_sell() const { return action == Sell || action == SellShort; }
        bool is_active() const { return state != State::Canceled && state != State::Filled; }
        bool is_initial() const { return state == State::Initial; }
        bool is_accepted() const { return state == State::Accepted; }
        bool is_submitted() const { return state == State::Submitted; }
        bool is_cancelled() const { return state == State::Canceled; }
        bool is_partially_filled() const { return state == State::PartiallyFilled; }
        bool is_filled() const { return state == State::Filled; }

    protected:
        inline Order(Type type, Action action, const std::string& symbol, size_t quantity);
        
    private:
        inline size_t get_valid_states(State state, std::set<State>& next_states) const;
        inline bool is_valid_state_(State current, State newState) const;
        
        static const constexpr int s_max_transition_state = 5;
        struct StateTable {
            State current;
            State valid_states[s_max_transition_state];
        };
        template <typename _T> struct state_table { static StateTable value[]; };
        typedef state_table<void> state_table_t;
        state_table_t state_table_;
    };
    
    // traits
    struct market_order_tag {};
    struct limit_order_tag {};
    struct stop_order_tag {};
    struct stop_limit_order_tag {};
    template <int _OrderType> struct order_traits;
    template<> struct order_traits<Order::Market> { typedef market_order_tag type; };
    template<> struct order_traits<Order::Limit> { typedef limit_order_tag type; };
    template<> struct order_traits<Order::Stop> { typedef stop_order_tag type; };
    template<> struct order_traits<Order::StopLimit> { typedef stop_limit_order_tag type; };
        
#include "impl/order.ipp"
}}


