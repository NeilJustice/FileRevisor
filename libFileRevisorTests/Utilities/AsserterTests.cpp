#include "pch.h"
#include "libFileRevisor/Utilities/Asserter.h"

namespace Utils
{
	TESTS(AsserterTests)
	AFACT(ThrowIfBoolsNotEqual_BoolsAreEqual_DoesNotThrowException)
	AFACT(ThrowIfBoolsNotEqual_BoolsAreNotEqual_ThrowsRuntimeError)
	AFACT(ThrowIfSizeTNotEqual_SizeTAreEqual_DoesNotThrowException)
	AFACT(ThrowIfSizeTNotEqual_SizeTAreNotEqual_ThrowsRuntimeError)
	AFACT(ThrowIfStringsNotEqual_StringsAreEqual_DoesNotThrowException)
	AFACT(ThrowIfStringsNotEqual_StringsAreNotEqual_ThrowsRuntimeError)
	EVIDENCE

	Asserter _asserter;

	TEST(ThrowIfBoolsNotEqual_BoolsAreEqual_DoesNotThrowException)
	{
		const bool expectedAndActualValue = ZenUnit::Random<bool>();
		const string message = ZenUnit::Random<string>();
		//
		_asserter.ThrowIfBoolsNotEqual(expectedAndActualValue, expectedAndActualValue, message);
	}

	TEST(ThrowIfBoolsNotEqual_BoolsAreNotEqual_ThrowsRuntimeError)
	{
		const bool expected = ZenUnit::Random<bool>();
		const bool actual = !expected;
		const string message = ZenUnit::Random<string>();
		//
		const string expectedExceptionMessage = String::Concat("Utils::Asserter::ThrowIfBoolsNotEqual() failed.",
			" expected=", expected, ", actual=", actual, ", message=\"", message, "\"");
		THROWS_EXCEPTION(_asserter.ThrowIfBoolsNotEqual(expected, actual, message),
			runtime_error, expectedExceptionMessage);
	}

	TEST(ThrowIfSizeTNotEqual_SizeTAreEqual_DoesNotThrowException)
	{
		const size_t expected = ZenUnit::Random<size_t>();
		const size_t actual = expected;
		const string message = ZenUnit::Random<string>();
		//
		_asserter.ThrowIfSizeTNotEqual(expected, actual, message);
	}

	TEST(ThrowIfSizeTNotEqual_SizeTAreNotEqual_ThrowsRuntimeError)
	{
		const size_t expected = ZenUnit::RandomNon0<size_t>();
		const size_t actual = expected - 1;
		const string message = ZenUnit::Random<string>();
		//
		const string expectedExceptionMessage = String::Concat("Utils::Asserter::ThrowIfSizeTNotEqual() failed.",
			" expected=", expected, ", actual=", actual, ", message=\"", message, "\"");
		THROWS_EXCEPTION(_asserter.ThrowIfSizeTNotEqual(expected, actual, message),
			runtime_error, expectedExceptionMessage);
	}

	TEST(ThrowIfStringsNotEqual_StringsAreEqual_DoesNotThrowException)
	{
		const string expectedAndActualValue = ZenUnit::Random<string>();
		const string message = ZenUnit::Random<string>();
		//
		_asserter.ThrowIfStringsNotEqual(expectedAndActualValue, expectedAndActualValue, message);
	}

	TEST(ThrowIfStringsNotEqual_StringsAreNotEqual_ThrowsRuntimeError)
	{
		const string expected = ZenUnit::Random<string>();
		const string actual = ZenUnit::Random<string>() + ZenUnit::Random<string>();
		const string message = ZenUnit::Random<string>();
		//
		const string expectedExceptionMessage = String::Concat("Utils::Asserter::ThrowIfStringsNotEqual() failed.",
			" expected=\"", expected, "\", actual=\"", actual, "\", message=\"", message, "\"");
		THROWS_EXCEPTION(_asserter.ThrowIfStringsNotEqual(expected, actual, message),
			runtime_error, expectedExceptionMessage);
	}

	RUN_TESTS(AsserterTests)
}
