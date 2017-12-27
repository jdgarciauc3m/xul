#ifndef XUL_ENDIAN_BYTE_CAST_H
#define XUL_ENDIAN_BYTE_CAST_H

#include <type_traits>

namespace xul {

namespace endian {

/**
 * @defgroup bytecasting Byte casting
 * Facilities for performing byte casting a value to/from an underlying type.
 */

/// Support type for byte casting a value to/from un underlying type.
/// @tparam V type of the value type.
/// @tparam U type of the underlying type.
/// @pre V must be a a scalar type of the same or smaller size than U.
/// @ingroup bytecasting
template <typename V, typename U>
union byte_cast_type {
  static_assert(std::is_scalar<V>::value,"T must be scalar"); 
  static_assert(std::is_scalar<U>::value,"T must be scalar"); 
  static_assert(sizeof(V)<=sizeof(U), "T must be same size as value_type");

public:
  /// @name Constructors
  /// @{
  byte_cast_type(V v);
  byte_cast_type(U u);
  /// @}
  
  /// @name Conversion operators.
  /// @{
  explicit operator V() const;
  explicit operator U() const;
  /// @}
private:
  /// Value for the cast.
  V value_;
  /// Underlying representation of the value.
  U underlying_;
};

/// Constructs from a value.
/// @param v Value for the cast.
template <typename V, typename U>
byte_cast_type<V,U>::byte_cast_type(V v) :
  value_(v)
{}

/// Constructs from an underlying representation.
/// @param u Underlying representation.
template <typename V, typename U>
byte_cast_type<V,U>::byte_cast_type(U u) : 
  underlying_(u) 
{ }

/// Converts to value type.
/// @returns The value as V type.
template <typename V, typename U>
byte_cast_type<V,U>::operator V() const
{
  return value_;
}

/// Converts to underlying type.
/// @returns The value as U type.
template <typename V, typename U>
byte_cast_type<V,U>::operator U() const
{
  return underlying_;
}

/// Converts a value to an underlying type.
/// @tparam V Value type.
/// @tparam U Underlying type.
/// @param v Value to be casted.
/// @return The value bytes in a U type.
/// @ingroup bytecasting
template <typename V, typename U>
typename std::enable_if<!std::is_same<U,V>::value,U>::type
underlying_cast(V v) {
  U u{byte_cast_type<V,U>{v}};
  return u;
}

template <typename V, typename U>
typename std::enable_if<std::is_same<U,V>::value,U>::type
underlying_cast(V v) {
  return v;
}


/// Converts an underlying value to a value type.
/// @tparam V Value type.
/// @tparam U Underlying type.
/// @param u Underlying value previously casted throung underlying_cast.
/// @return The value bytes in a V type.
/// @ingroup bytecasting
template <typename V, typename U>
typename std::enable_if<!std::is_same<U,V>::value, V>::type
value_cast(U u) {
  V v{byte_cast_type<V,U>{u}};
  return v;
}

template <typename V, typename U>
typename std::enable_if<std::is_same<U,V>::value, V>::type
value_cast(U u) {
  return u;
}

}
}

#endif 
