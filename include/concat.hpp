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

#ifndef _INCLUDE_CONCAT_HPP_
#	define _INCLUDE_CONCAT_HPP_

#	pragma once

#	include "concat/traits.hpp"

#	include <stddef.h>

#	include <array>

#	include <tier0/platform.h>
#	include <tier0/bufferstring.h>
#	include <tier1/utlvector.h>
#	include <mathlib/vector.h>
#	include <mathlib/vector4d.h>

#	define CONCAT_APPEND_VARIABLE(concat, var) concat.Append(#var, var);

class CConcatLineString;

// Globals.
extern const std::array<const CConcatLineString, 8> g_arrEmbedsConcat;

// Aliases.
#define g_aRootConcat   g_arrEmbedsConcat[0]
#define g_aEmbedConcat  g_arrEmbedsConcat[1] // Single indent.
#define g_aEmbed2Concat g_arrEmbedsConcat[2] // Double indent.
#define g_aEmbed3Concat g_arrEmbedsConcat[3] // Triple indent.

template<class T>
struct ConcatLine_t
{
	T m_aStartsWith;
	T m_aBefore;
	T m_aBetween;
	T m_aEnds;
	T m_aEndsAndStartsWith;
}; // ConcatLine_t<T>

template<class T>
class CConcatLineBase : public ConcatLine_t<T>
{
public:
	using Base_t = ConcatLine_t<T>;
	using Base_t::Base_t;

	CConcatLineBase(Base_t &&aBase) : Base_t(Move(aBase)) {}

public:
	template<bool INSERT_BEFORE = true>
	auto GetKey(const T &aKey) const
	{
		std::array<T, 3 + INSERT_BEFORE> arrResult {Base_t::m_aStartsWith, aKey};

		if constexpr (INSERT_BEFORE)
		{
			arrResult[2] = Base_t::m_aBefore;
		}

		arrResult[2 + INSERT_BEFORE] = Base_t::m_aEnds;

		return arrResult;
	}

	template<bool INSERT_BEFORE = true>
	auto GetKeyString(const T &aKey) const
	{
		std::array<T, 5 + INSERT_BEFORE> arrResult {Base_t::m_aStartsWith, "\"", aKey, "\""};

		if constexpr (INSERT_BEFORE)
		{
			arrResult[4] = Base_t::m_aBefore;
		}

		arrResult[4 + INSERT_BEFORE] = Base_t::m_aEnds;

		return arrResult;
	}

	std::array<T, 5> GetKeyValue(const T &aKey, const T &aValue) const
	{
		return {Base_t::m_aStartsWith, aKey, Base_t::m_aBetween, aValue, Base_t::m_aEnds};
	}

	std::array<T, 7> GetKeyValueString(const T &aKey, const T &aValue) const
	{
		return {Base_t::m_aStartsWith, aKey, Base_t::m_aBetween, "\"", aValue, "\"", Base_t::m_aEnds};
	}

	std::array<T, 7> GetKeyStringValue(const T &aKey, const T &aValue) const
	{
		return {Base_t::m_aStartsWith, "\"", aKey, "\"", Base_t::m_aBetween, aValue, Base_t::m_aEnds};
	}

	std::array<T, 9> GetKeyStringValueString(const T &aKey, const T &aValue) const
	{
		return {Base_t::m_aStartsWith, "\"", aKey, "\"", Base_t::m_aBetween, "\"", aValue, "\"", Base_t::m_aEnds};
	}

public:
	template<bool IS_STRING = false, bool INSERT_BEFORE = true>
	std::array<T, 3 + IS_STRING * 2 + INSERT_BEFORE> GetKey2(const T &aKey) const
	{
		if constexpr (IS_STRING)
		{
			return GetKeyString<INSERT_BEFORE>(aKey);
		}

		return GetKey<INSERT_BEFORE>(aKey);
	}

	template<bool KEY_STRING = false, bool VALUE_STRING = false>
	constexpr std::array<T, 5 + (KEY_STRING + VALUE_STRING) * 2> GetKeyValue2(const T &aKey, const T &aValue) const
	{
		if constexpr (KEY_STRING && VALUE_STRING)
		{
			return GetKeyStringValueString(aKey, aValue);
		}
		else if constexpr (KEY_STRING)
		{
			return GetKeyStringValue(aKey, aValue);
		}
		else if constexpr (VALUE_STRING)
		{
			return GetKeyValueString(aKey, aValue);
		}
		else
		{
			return GetKeyValue(aKey, aValue);
		}
	}
}; // CConcatLineBase<T>

using CConcatLineStringBase = CConcatLineBase<const char *>;

class CConcatLineString : public CConcatLineStringBase
{
public:
	using CBase = CConcatLineStringBase;
	using CBase::CBase;

public:
	const char *GetStartsWith() const        { return m_aStartsWith; }
	const char *GetBefore() const            { return m_aBefore; }
	const char *GetBetween() const           { return m_aBetween; }
	const char *GetEnds() const              { return m_aEnds; }
	const char *GetEndsAndStartsWith() const { return m_aEndsAndStartsWith; }
}; // CConcatLineString

class CConcatLineBuffer
{
public:
	CConcatLineBuffer(const CConcatLineString *pData, CBufferString *pBuffer) : m_pData(pData), m_pBuffer(pBuffer) {}

public:
	const CConcatLineString *GetData() const { return m_pData; }

public:
	template<bool IS_STRING = false, bool INSERT_BEFORE = true>
	const char *Append(const char *pszKey) const
	{
		const auto vecConcat = m_pData->GetKey2<IS_STRING, INSERT_BEFORE>(pszKey);

		return m_pBuffer->AppendConcat(vecConcat.size(), vecConcat.data());
	}

	template<bool KEY_STRING = false, bool VALUE_STRING = false, typename T>
	const char *Append(const char *pszKey, T aValue) const
	{
		CBufferStringN<256> sValue;

		constexpr const char *pszFormat = k_bIsConcatPointer<T> ? k_pConcatFormat<void *> : k_pConcatFormat<T>;

		if constexpr (pszFormat)
		{
			sValue.Format(pszFormat, aValue);
		}
		else if constexpr (k_bIsSameConcatType<T, Vector2D>)
		{
			sValue.Format("%f %f", aValue[0], aValue[1]);
		}
		else if constexpr (k_bIsSameConcatType<T, Vector> || k_bIsSameConcatType<T, QAngle>)
		{
			sValue.Format("%f %f %f", aValue[0], aValue[1], aValue[2]);
		}
		else if constexpr (k_bIsSameConcatType<T, Vector4D>)
		{
			sValue.Format("%f %f %f %f", aValue[0], aValue[1], aValue[2], aValue[3]);
		}
		else
		{
			sValue = "<unkown type>";
		}

		const auto vecConcat = m_pData->GetKeyValue2<KEY_STRING, VALUE_STRING>(pszKey, sValue.String());

		return m_pBuffer->AppendConcat(vecConcat.size(), vecConcat.data());
	}

	template<bool KEY_STRING = false, bool VALUE_STRING = false>
	const char *AppendBytes(const char *pszKey, const byte *pData, uintp nLength) const
	{
		constexpr size_t nStrByteSize = 3;

		char *psDataSet = new char[nLength * nStrByteSize + 1];

		for(int n = 0; n < nLength; n++)
		{
			char *psByte = &psDataSet[n * nStrByteSize];

			snprintf(psByte, nStrByteSize + 1, "%02X ", pData[n]);
		}

		auto *pResult = Append<KEY_STRING, VALUE_STRING>(pszKey, psDataSet);

		delete[] psDataSet;

		return pResult;
	}

	template<bool KEY_STRING = false, bool VALUE_STRING = false, typename T>
	const char *AppendHandle(const char *pszKey, T aHandle) const
	{
		CBufferStringN<16> sValue;

		constexpr const char *pszFormat = k_pConcatFormatHandle<T>;

		if constexpr (pszFormat)
		{
			sValue.Format(pszFormat, k_bIsConcatPointer<T> ? reinterpret_cast<uintp>(aHandle) : aHandle);
		}
		else
		{
			sValue = "<unknown handle type>";
		}

		const auto vecConcat = m_pData->GetKeyValue2<KEY_STRING, VALUE_STRING>(pszKey, sValue.String());

		return m_pBuffer->AppendConcat(vecConcat.size(), vecConcat.data());
	}

	const char *AppendEnds() const              { return m_pBuffer->Append(m_pData->GetEnds(), -1); }
	const char *AppendEndsAndStartsWith() const { return m_pBuffer->Append(m_pData->GetEndsAndStartsWith(), -1); }

private:
	const CConcatLineString *m_pData;
	CBufferString *m_pBuffer;
}; // CConcatLineString

class ConcatLineString final : public CConcatLineString
{
public:
	using CBase = CConcatLineString;
	using CBase::CBase;
}; // ConcatLineString

#endif // _INCLUDE_CONCAT_HPP_
