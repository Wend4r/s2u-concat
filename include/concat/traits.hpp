/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Source 2 Utility - Concat
 * Written by Wend4r (Vladimir Ezhikov).
 * ======================================================

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _INCLUDE_CONCAT_TRAITS_HPP_
#	define _INCLUDE_CONCAT_TRAITS_HPP_

#	include <tier0/platform.h>

class CUtlString;
class CBufferString;

template<typename, typename>    inline constexpr bool k_bIsSameConcatType = false;
template<typename T>            inline constexpr bool k_bIsSameConcatType<T, T> = true;

template<class>                 inline constexpr bool k_bIsConcatPointer = false;
template<>                      inline constexpr bool k_bIsConcatPointer<char *> = false;
template<>                      inline constexpr bool k_bIsConcatPointer<const char *> = false;
template<typename T>            inline constexpr bool k_bIsConcatPointer<T *> = true;
template<typename T>            inline constexpr bool k_bIsConcatPointer<T * const> = true;
template<typename T>            inline constexpr bool k_bIsConcatPointer<T * volatile> = true;
template<typename T>            inline constexpr bool k_bIsConcatPointer<T * const volatile> = true;

template<typename>              inline constexpr const char *k_pConcatFormat = nullptr;
template<typename>              inline constexpr const char *k_pConcatFormatHandle = nullptr;

// Signed number.
template<> inline constexpr const char *k_pConcatFormat<int8> =             "%hhd";
template<> inline constexpr const char *k_pConcatFormat<int16> =            "%hd";
template<> inline constexpr const char *k_pConcatFormat<int32> =            "%d";
template<> inline constexpr const char *k_pConcatFormat<int64> =            "%lld";

// Unsigned number.
template<> inline constexpr const char *k_pConcatFormat<uint8> =            "%hhu";
template<> inline constexpr const char *k_pConcatFormat<uint16> =           "%hu";
template<> inline constexpr const char *k_pConcatFormat<uint32> =           "%d";
template<> inline constexpr const char *k_pConcatFormat<uint64> =           "%llu";
template<> inline constexpr const char *k_pConcatFormatHandle<uint8> =      "%02X";
template<> inline constexpr const char *k_pConcatFormatHandle<uint16> =     "%04X";
template<> inline constexpr const char *k_pConcatFormatHandle<uint32> =     "%08X";
template<> inline constexpr const char *k_pConcatFormatHandle<uint64> =     "%016X";

// Float point number.
template<> inline constexpr const char *k_pConcatFormat<float32> =          "%f";
template<> inline constexpr const char *k_pConcatFormat<float64> =          "%lf";

// String.
template<> inline constexpr const char *k_pConcatFormat<char> =             "%c";
template<> inline constexpr const char *k_pConcatFormat<char *> =           "%s";
template<> inline constexpr const char *k_pConcatFormat<const char *> =     "%s";
template<> inline constexpr const char *k_pConcatFormat<CUtlString> =       "%s";
template<> inline constexpr const char *k_pConcatFormat<CBufferString> =    "%s";

// Pointer.
template<> inline constexpr const char *k_pConcatFormat<void *> =           "%p";
template<> inline constexpr const char *k_pConcatFormatHandle<void *> =     "%p";

#endif // _INCLUDE_CONCAT_TRAITS_HPP_
