
#include <random>
#include <ctime>
#include <chrono>
#include "base/uuid.h"

#include <iostream>

namespace CNORXZ
{

    constexpr std::tm mkTRef()
    {
	std::tm o = {};
	o.tm_sec = 0;
	o.tm_min = 0;
	o.tm_hour = 0;
	o.tm_mday = 1;
	o.tm_mon = 0;
	o.tm_year = 70;
	o.tm_wday = 4;
	o.tm_yday = 0;
	o.tm_isdst = 0;
	return o;
    }
    
    // the node bits are created randomly (no search for mac or similar)
    Uuid mkUuid()
    {
	static constexpr uint64_t diff_1970_1582_ns =
	    141427ull * 86400ull * 1000ull * 1000ull * 1000ull;
	static std::tm tref_1970 = mkTRef();
	static uint16_t version = 1;
	std::tm tref_1970_tmp = tref_1970;
	const auto ref = std::chrono::system_clock::from_time_t(std::mktime(&tref_1970_tmp));
	const auto now = std::chrono::system_clock::now();
	const uint64_t diff_ns = std::chrono::duration_cast<std::chrono::duration<uint64_t,std::nano>>
	    (now - ref).count() + diff_1970_1582_ns;

	// following nomenclature of wikipedia
	const uint32_t time_low = diff_ns & 0xffffffff;
	const uint16_t time_mid = (diff_ns >> 32) & 0xffff;
	const uint16_t time_hi_and_version = ((diff_ns >> 48) & 0x0fff) | ( version << 12 );
	const uint8_t clock_seq_high_and_reserved = ((diff_ns >> 8) & 0x3f) | ( 0x02 << 6 );
	const uint8_t clock_seq_low = diff_ns & 0xff;

	static std::mt19937_64 gen {diff_ns};
	const uint64_t node = gen();

	const Uuid id =
	    { ( static_cast<uint64_t>(time_low) << 32 ) |
	      ( static_cast<uint64_t>(time_mid) << 16 ) |
	      ( static_cast<uint64_t>(time_hi_and_version) ),
	      ( static_cast<uint64_t>(clock_seq_high_and_reserved) << 56 ) |
	      ( static_cast<uint64_t>(clock_seq_low) << 48 ) |
	      ( node & 0xffffffffffff )
	    };
	return id;
    }
}
