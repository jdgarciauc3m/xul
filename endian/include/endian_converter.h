#ifndef XUL_ENDIAN_H
#define XUL_ENDIAN_H

#include "byte_sequence.h"


namespace xul {

/// Utilities for endian management.
namespace endian {

/**
 * @defgroup endianconv Endian conversion.
 * Conversion to target endian with detection of host endianness.
 */

/// Endian type.
/// @ingroup endianconv
enum class endian_type 
{
  /// Little endian.
  little,
  /// Big endian.
  big
  /// Mixed endianness.
};


template <endian_type E>
class static_endian_converter {
public:
  template <typename T>
    T to_host(byte_sequence<sizeof(T)> x);
  template <typename T>
    byte_sequence<sizeof(T)> from_host(T x);
private:
  static constexpr endian_type get_endianness();

private:
  static constexpr endian_type HOST_ENDIAN = get_endianness();
  static constexpr endian_type TARGET_ENDIAN = E;
};

template <endian_type E>
constexpr endian_type static_endian_converter<E>::get_endianness() 
{
#ifdef __GNUG__
#if __BYTE_ORDER == __LITTLE_ENDIAN
return endian_type::little;
#elif __BYTE_ORDER == __BIG_ENDIAN
return endian_type::big;
#elif __BYTE_ORDER == __PDP_ENDIAN
static_assert(true, "PDP order unsupported");
#else
static_assert(true, "Unknown GNU byte order");
#endif
#else
static_assert(true, "Unsupported platform");
#endif
}

template <endian_type E>
template <typename T>
T static_endian_converter<E>::to_host(byte_sequence<sizeof(T)> x)
{
  return byte_sequence_swapper<HOST_ENDIAN!=TARGET_ENDIAN>::template make_value<T>(x);
  ////return x.template get_value<T>(TARGET_ENDIAN!=host_);
}

template <endian_type E>
template <typename T>
byte_sequence<sizeof(T)> static_endian_converter<E>::from_host(T x)
{
  return byte_sequence_swapper<HOST_ENDIAN!=TARGET_ENDIAN>::make_byte_sequence(x);
  //return byte_sequence<sizeof(T)>{x, TARGET_ENDIAN!=host_};
}


/// Manages endian conversion to a target model.
/// @ingroup endianconv
template <endian_type E>
class dynamic_endian_converter {
public:
  /// @name Constructor
  /// @{
  //endian_converter() : host_(get_endianness()) {}
  /// @}

  /// @name Host endinanness query
  /// @{
  bool host_is_little() const { return host_ == endian_type::little; }
  bool host_is_big() const { return host_ == endian_type::big; }
  endian_type host_endian() const { return host_; }
  /// @}

  /// @name Value conversion
  /// @{
  template <typename T>
    T to_host(byte_sequence<sizeof(T)> x);
  template <typename T>
    byte_sequence<sizeof(T)> from_host(T x); 
  /// @}

private:
  static endian_type get_endianness(); 

private:
  /// Target endianness.
  static constexpr endian_type TARGET_ENDIAN = E; 
  
  /// Host endianness.
  static const endian_type host_; 
};
       
template <endian_type E>
const endian_type dynamic_endian_converter<E>::host_ = get_endianness();

/// Detects endianness of host platform.
/// @return Endian type of host platform.
template <endian_type E>
endian_type dynamic_endian_converter<E>::get_endianness() {
  return (value_cast<uint8_t, uint16_t>(0x00ff)==0xff)?
    endian_type::little:endian_type::big;
}

/// Converts a value to a byte sequence from a host type.
/// @tparam T Value type to be converted.
/// @param x Value with host endianness to be converted.
/// @returns A sequence of sizeof(T) bytes in target endianness.
template <endian_type E>
template <typename T>
byte_sequence<sizeof(T)> dynamic_endian_converter<E>::from_host(T x) {
  return byte_sequence<sizeof(T)>{x, TARGET_ENDIAN!=host_};
}

/// Converts a value from a byte sequence to a host type.
/// @tparam T Value type to be obtained.
/// @param x Sequence of sizeof(T) bytes with target endiannes to be converted
/// to host value type.
/// @returns A value with host endianness.
template <endian_type E>
template <typename T>
T dynamic_endian_converter<E>::to_host(byte_sequence<sizeof(T)> x) {
  return x.template get_value<T>(TARGET_ENDIAN!=host_);
}

}
}

#endif
