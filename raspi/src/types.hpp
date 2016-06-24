// Copyright 2014 Perttu Ahola <celeron55@gmail.com>
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <exception>
#include <cstdint>
#include <cinttypes> // PRId64
#include <sstream>
#include <memory>
#include <stdio.h> // snprintf

typedef   int8_t  s8;
typedef  uint8_t  u8;
typedef  int16_t s16;
typedef uint16_t u16;
typedef  int32_t s32;
typedef uint32_t u32;
typedef  int64_t s64;
typedef uint64_t u64;

#define S32F "%" PRId32

typedef unsigned int uint;
typedef unsigned char uchar;

typedef std::string ss_;
template<typename T> using sv_ = std::vector<T>;
template<typename T> using set_ = std::unordered_set<T>;
template<typename T1, typename T2> using sm_ = std::unordered_map<T1, T2>;
typedef const char cc_;
static inline cc_* cs(const ss_ &s){
	return s.c_str();
}
template<typename T> using up_ = std::unique_ptr<T>;
template<typename T> using sp_ = std::shared_ptr<T>;
template<typename T> using wp_ = std::weak_ptr<T>;
template<typename T> using sil_ = std::initializer_list<T>;

#define IDF "%" PRId64
struct id_ {
	int64_t value;

	id_(): value(0){}
	id_(int64_t v): value(v){}

	id_& operator=(int64_t v){
		value = v;
		return *this;
	}
	bool operator<(const id_ &other) const {
		return value < other.value;
	}
	bool operator==(const id_ &other) const {
		return value == other.value;
	}
	bool operator!=(const id_ &other) const {
		return value != other.value;
	}
};
static inline std::ostream& operator<<(std::ostream &out, const id_ &v){
	return (out<<v.value);
}

struct db_json_ {
	std::string value;
	db_json_(){}
	db_json_(const std::string &value_): value(value_){}
};

struct Exception: public std::exception {
	ss_ msg;
	Exception(const ss_ &msg): msg(msg){}
	virtual const char* what() const throw(){
		return msg.c_str();
	}
};
#define DEFINE_EXCEPTION(name, base) struct name: public base \
{name(const ss_ &msg): base(msg){}}

static inline ss_ itos(int64_t i){
	char buf[22];
	snprintf(buf, 22, "%" PRId64, i);
	return buf;
}
static inline ss_ itos(id_ id){
	return itos(id.value);
}
static inline ss_ itox(int i){
	char buf[30];
	snprintf(buf, 30, "0x%x", i);
	return buf;
}
static inline ss_ ftos(float f){
	char buf[100];
	snprintf(buf, 100, "%f", f);
	return buf;
}

#define SLEN(x) (sizeof(x)/sizeof((x)[0]))

template<typename T>
		static inline ss_ dump(const T &v);

template<>
inline ss_ dump(const double &v){
	return ftos(v);
}

template<>
inline ss_ dump(const int &v){
	return itos(v);
}

template<>
inline ss_ dump(const uint32_t &v){
	return itos(v);
}

template<>
inline ss_ dump(const uint64_t &v){
	return itos(v);
}

template<>
inline ss_ dump(const uint8_t &v){
	return itos(v);
}

template<typename T>
static inline ss_ dump(const sv_<T> &vs){
	std::ostringstream os(std::ios::binary);
	os<<"[";
	bool first = true;
	for(const auto &v : vs){
		if(!first)
			os<<", ";
		first = false;
		os<<v;
	}
	os<<"]";
	return os.str();
}

template<typename T>
static inline ss_ dump(const std::set<T> &vs){
	std::ostringstream os(std::ios::binary);
	os<<"(";
	bool first = true;
	for(const auto &v : vs){
		if(!first)
			os<<", ";
		first = false;
		os<<v;
	}
	os<<")";
	return os.str();
}

template<typename T>
static inline cc_* cs(const T &v){
	return dump(v).c_str();
}

// check()

DEFINE_EXCEPTION(NullptrCatch, Exception);

template<typename T>
static inline T* check(T *v){
	if(v == nullptr)
		throw NullptrCatch("check(): nullptr");
	return v;
}
template<typename T>
static inline const T* check(const T *v){
	if(v == nullptr)
		throw NullptrCatch("check(): nullptr");
	return v;
}

DEFINE_EXCEPTION(FalseValueCatch, Exception);

template<typename T>
static inline T check_true(T v){
	if(!v)
		throw FalseValueCatch("check_true(): false");
	return v;
}
