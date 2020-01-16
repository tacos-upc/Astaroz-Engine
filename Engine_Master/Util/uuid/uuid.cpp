#include "uuid.h"

#include <errno.h> // ENOTSUP

#include <cstdio>       // sprintf() scanf()
#include <cstring>      // strlen()
#include <stdexcept>
#include <system_error>
#include <utility> // swap()


using namespace std;

namespace stud
{
	array<char, 37> uuid::
	c_string (bool upper) const
	{
	array<char, 37> r;

	sprintf (r.data (),
				(upper
				? "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"
				: "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"),
				time_low,
				time_mid,
				time_hiv,
				clock_seq_hir,
				clock_seq_low,
				node[0], node[1], node[2], node[3], node[4], node[5]);

	return r;
	}

	void uuid::
	assign (const char* s)
	{
	if (s != nullptr && strlen (s) == 36 && s[8] == '-')
	{
		if (sscanf (s,
					"%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
					&time_low,
					&time_mid,
					&time_hiv,
					&clock_seq_hir,
					&clock_seq_low,
					&node[0], &node[1], &node[2],
					&node[3], &node[4], &node[5]) == 11)
		return;
	}

	throw invalid_argument ("invalid UUID string representation");
	}

	uuid_system_generator uuid::system_generator;

	// Utility function used by platform-specified uuid-*.cxx implementations.
	//
	void
	uuid_throw_weak ()
	{
	throw system_error (ENOTSUP,
						generic_category (),
						"strong UUID uniqueness cannot be guaranteed");
	}

	// NOTE: the order of definitions is important to MinGW GCC (DLL linkage).

	bool uuid::nil() const
	{
		return
			time_low == 0 &&
			time_mid == 0 &&
			time_hiv == 0 &&
			clock_seq_hir == 0 &&
			clock_seq_low == 0 &&
			node[0] == 0 && node[1] == 0 && node[2] == 0 &&
			node[3] == 0 && node[4] == 0 && node[5] == 0;
	}

	uuid::operator bool() const
	{
		return !nil();
	}

	inline void uuid::
		swap(uuid& u)
	{
		std::swap(time_low, u.time_low);
		std::swap(time_mid, u.time_mid);
		std::swap(time_hiv, u.time_hiv);
		std::swap(clock_seq_hir, u.clock_seq_hir);
		std::swap(clock_seq_low, u.clock_seq_low);
		std::swap(node, u.node);
	}

	uuid::uuid(uuid&& u): uuid() // nil
	{
		swap(u);
	}

	uuid& uuid::operator= (uuid&& u)
	{
		if (this != &u)
		{
			uuid n; // nil
			swap(n);
			swap(u);
		}
		return *this;
	}

	void uuid_assign_impl(uuid& r, const std::uint8_t* p)
	{
		{
			std::uint32_t t;

			t = *p++;
			t = (t << 8) | *p++;
			t = (t << 8) | *p++;
			t = (t << 8) | *p++;
			r.time_low = t;
		}

		std::uint16_t t;

		t = *p++;
		t = (t << 8) | *p++;
		r.time_mid = t;

		t = *p++;
		t = (t << 8) | *p++;
		r.time_hiv = t;

		r.clock_seq_hir = *p++;
		r.clock_seq_low = *p++;

		std::memcpy(r.node, p, 6);
	}

	void uuid::assign(const binary_type& d)
	{
		uuid_assign_impl(*this, d.data());
	}

	uuid::uuid(const binary_type& d)
	{
		assign(d);
	}

	uuid::binary_type uuid::binary() const noexcept
	{
		using u8 = std::uint8_t;

		binary_type r;
		std::uint32_t t;

		t = time_low;
		r[3] = static_cast<u8> (t); t >>= 8;
		r[2] = static_cast<u8> (t); t >>= 8;
		r[1] = static_cast<u8> (t); t >>= 8;
		r[0] = static_cast<u8> (t);

		t = time_mid;
		r[5] = static_cast<u8> (t); t >>= 8;
		r[4] = static_cast<u8> (t);

		t = time_hiv;
		r[7] = static_cast<u8> (t); t >>= 8;
		r[6] = static_cast<u8> (t);

		r[9] = clock_seq_low;
		r[8] = clock_seq_hir;

		std::memcpy(r.data() + 10, node, 6);

		return r;
	}

	void uuid::assign(const std::uint8_t(&d)[16])
	{
		uuid_assign_impl(*this, d);
	}

	uuid::uuid(const std::uint8_t(&d)[16])
	{
		assign(d);
	}

	#ifdef _WIN32
	uuid::uuid(const _GUID& g)
	{
		assign(g);
	}
	#endif

	void uuid::assign(const std::string& s)
	{
		assign(s.c_str());
	}

	uuid::uuid(const std::string& s)
	{
		assign(s);
	}

	uuid::uuid(const char* s)
	{
		assign(s);
	}

	std::string uuid::string(bool upper) const
	{
		return c_string(upper).data();
	}

	uuid::variant_type uuid::variant() const
	{
		return nil()
			? variant_type::dce
			: ((clock_seq_hir & 0x80) == 0 ? variant_type::ncs :
			(clock_seq_hir & 0x40) == 0 ? variant_type::dce :
				(clock_seq_hir & 0x20) == 0 ? variant_type::microsoft :
				/*                         */ variant_type::other);
	}

	uuid::version_type uuid::version() const
	{
		return nil()
			? version_type::random
			: static_cast<version_type> ((time_hiv >> 12) & 0x0f);
	}

	int uuid::compare(const uuid& y) const
	{
		int r;
		auto neq = [&r](auto a, auto b) -> bool
		{
			return (r = (a == b ? 0 : (a < b ? -1 : 1))) != 0;
		};

		return (neq(time_low, y.time_low) ? r :
			neq(time_mid, y.time_mid) ? r :
			neq(time_hiv, y.time_hiv) ? r :
			neq(clock_seq_hir, y.clock_seq_hir) ? r :
			neq(clock_seq_low, y.clock_seq_low) ? r :
			std::memcmp(node, y.node, 6));
	}

	bool operator== (const uuid& x, const uuid& y)
	{
		return x.compare(y) == 0;
	}

	bool operator!= (const uuid& x, const uuid& y)
	{
		return x.compare(y) != 0;
	}

	bool operator< (const uuid& x, const uuid& y)
	{
		return x.compare(y) < 0;
	}

	bool operator> (const uuid& x, const uuid& y)
	{
		return x.compare(y) > 0;
	}

	bool operator<= (const uuid& x, const uuid& y)
	{
		return x.compare(y) <= 0;
	}

	bool operator>= (const uuid& x, const uuid& y)
	{
		return x.compare(y) >= 0;
	}

	uuid uuid::generate(bool strong)
	{
		return system_generator.generate(strong);
	}
}

namespace std
{
	inline size_t hash<stud::uuid>::
		operator() (const stud::uuid& u) const noexcept
	{
		// To make sure hashes for the same UUID are the same on the same-width
		// platforms we FNV-hash the portable binary prepresentation.
		//
		// Let's keep the implementation inline hoping the compiler will unroll
		// the loop for us.
		//
		array<uint8_t, 16> d(u.binary());

		size_t h(static_cast<size_t> (2166136261UL));
		for (uint8_t b : d)
		{
			h ^= b;

			// We are using the C-style cast to suppress VC warnings for 32-bit
			// targets (the value is compiled but not used).
			//
			h *= sizeof(size_t) == 4
				? static_cast<size_t> (16777619UL)
				: (size_t)1099511628211ULL;
		}

		return h;
	}
}
