#ifndef XUL_ENDIAN_BYTE_SEQUENCE_H
#define XUL_ENDIAN_BYTE_SEQUENCE_H

#include "byte_cast.h"

#include <cstdint>
#include <type_traits>

namespace xul {

namespace endian {

/**
 * @defgroup byteseq Byte sequence
 * Representation of a sequence of bytes.
 */

/// @{

/// Type function for getting the int type used by a sequence of n bytes.
/// @tparam n Number of bytes in the sequence
/// @ingroup byteseq
/**
 * @note Only valid specializations are for values 2, 4 and 8.
 */
template <int n>
struct sequence_type {
};

/// Specialization for sequence of 2 bytes.
template <>
struct sequence_type<2> {
  /// Int type used for a sequence of 2 bytes.
  using type = uint16_t;
};

/// Specialization for sequence of 4 bytes.
template <>
struct sequence_type<4> {
  /// Int type used for a sequence of 4 bytes.
  using type = uint32_t;
};

/// Specialization for sequence of 8 bytes.
template <>
struct sequence_type<8> {
  /// Int type used for a sequence of 8 bytes.
  using type = uint64_t;
};

///@}

struct byte_swap_t{};

/// Byte sequence with byte swapping support.
/// @tparam N Number of bytes in sequence.
/// @ingroup byteseq
/**
 * A byte_sequence<n> supports representing any type that can fit in n
 * bytes. The class supports byte swapping as a feature for endian management.
 *
 * ~~~{.cpp}
 * byte_sequence<4> s1(x,false); // s1 holds x. No byte swapping.
 * byte_sequence<4> s2(y,true); // s1 holds x. Byte swapping.
 * float z = s1.get_value<float>(false); // Get value from s1. No swapping.
 * int32_t t = s2.get_value<int32_t>(false); // Get value from s2. Swapping.
 * ~~~
 *
 * Byte sequences support reading and writing (eventually binary) to any stream
 * supporting read/write operations (e.g. fstream's).
 */
template <int N>
class byte_sequence {

  static_assert( (N==2) || (N==4) || (N==8),
    "Unsupported byte_sequence lenght");

public:
  /// Type used for underlying representation of values.
  using word_type = typename sequence_type<N>::type;

public:

  /// @name Constructors 
  /// @{
  byte_sequence();

  template <typename T>
    byte_sequence(T x, bool sw);

  template <typename T>
    byte_sequence(T x);
  template <typename T>
    byte_sequence(T x, byte_swap_t);
  /// @}

  /// @name Accessors
  /// @{
  template <typename T>
    T get_value(bool sw);
  template <typename T>
    T get_value();
  template <typename T>
    T get_value(byte_swap_t);
  /// @}

  
  /// @name File operations
  /// @{
  template <typename O>
    void write(O & o);
  template <class I>
    void read(I & i);
  /// @}

private:
 
  /// Swaps bytes in representation.
  /// @returns value representation with bytes swapped. 
  word_type swapped_bytes() const; 

private:

  /// Value representing the byte sequence.
  word_type value_;
};

template <bool SW>
struct byte_sequence_swapper {
  template <typename T>
    static byte_sequence<sizeof(T)> make_byte_sequence(T x);
  template <typename T>
    static T make_value(byte_sequence<sizeof(T)> x);
};

template <>
template <typename T>
byte_sequence<sizeof(T)> byte_sequence_swapper<false>::make_byte_sequence(T x) {
  return byte_sequence<sizeof(T)>(x);
}

template <>
template <typename T>
byte_sequence<sizeof(T)> byte_sequence_swapper<true>::make_byte_sequence(T x) {
  return byte_sequence<sizeof(T)>{x, byte_swap_t{}};
}

template <>
template <typename T>
T byte_sequence_swapper<false>::make_value(byte_sequence<sizeof(T)> x) {
  return x.template get_value<T>();
}

template <>
template <typename T>
T byte_sequence_swapper<true>::make_value(byte_sequence<sizeof(T)> x) {
  return x.template get_value<T>(byte_swap_t{});
}

/// Empty constructor.
template <int N>
byte_sequence<N>::byte_sequence() :
value_{}
{}

/// Constructs a byte_sequence from a value x and eventually byte swaps.
/// @tparam T Type for the initializer value of the byte sequence.
/// @param x Value for the byte sequence.
/// @param sw If true swaps bytes in the representation value.
/// @pre sizeof(T) <= N
template <int N>
template <typename T>
byte_sequence<N>::byte_sequence(T x, bool sw) : 
  value_(underlying_cast<T,word_type>({x}))
{ 
  if (sw) { 
    value_ = swapped_bytes(); 
  } 
  static_assert(sizeof(T)<=sizeof(word_type),"T is too large");
}

template <int N>
template <typename T>
byte_sequence<N>::byte_sequence(T x) :
  value_(underlying_cast<T,word_type>({x}))
{
  static_assert(sizeof(T)<=sizeof(word_type),"T is too large");
}

template <int N>
template <typename T>
byte_sequence<N>::byte_sequence(T x, byte_swap_t) :
  value_(underlying_cast<T,word_type>({x}))
{
  value_ = swapped_bytes(); 
  static_assert(sizeof(T)<=sizeof(word_type),"T is too large");
}


/// Gets value eventually byte swapped.
/// @tparam T Type for the value to be obtained.
/// @param sw If try swaps bytes before returning.
/// @returns Value as a T.
/// @pre sizeof(T) <= N
template <int N>
template <typename T>
T byte_sequence<N>::get_value(bool sw) {
  static_assert(sizeof(T) <= sizeof(word_type), "T is too large");

  return value_cast<T,word_type>({(sw)?swapped_bytes():value_});
}

template <int N>
template <typename T>
T byte_sequence<N>::get_value(byte_swap_t) {
  static_assert(sizeof(T) <= sizeof(word_type), "T is too large");

  return value_cast<T,word_type>({swapped_bytes()});
}

template <int N>
template <typename T>
T byte_sequence<N>::get_value() {
  static_assert(sizeof(T) <= sizeof(word_type), "T is too large");

  return value_cast<T,word_type>({value_});
}


/// Writes a byte sequence in a file.
/// @tparam O Output stream type.
/// @param o Output stream
template <int N>
template <class O>
void byte_sequence<N>::write(O & o) 
{ 
  o.write(reinterpret_cast<char*>(&value_), sizeof(value_)); 
}

/// Reads a byte sequence from a file.
/// @tparam I Input stream type.
/// @param i Input stream.
template <int N>
template <class I>
void byte_sequence<N>::read(I & i) 
{ 
  i.read(reinterpret_cast<char*>(&value_), sizeof(value_)); 
}

/// @returns value representation with 2 bytes swapped. 
template <>
byte_sequence<2>::word_type byte_sequence<2>::swapped_bytes() const {
    return ( 
             (((value_) & 0xff00) >>  8) |
             (((value_) & 0x00ff0) <<  8) );
}

/// @returns value representation with 4 bytes swapped. 
template <>
byte_sequence<4>::word_type byte_sequence<4>::swapped_bytes() const {
    return ( (((value_) & 0xff000000) >> 24) | 
             (((value_) & 0x00ff0000) >>  8) |
             (((value_) & 0x0000ff00) <<  8) | 
             (((value_) & 0x000000ff) << 24) );
}

}
}

#endif
