#include "CppUnitTest.h"
#include "Utilities.h"
#include "TestUtilities.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(UtilitiesTests)
	{
	public:
		TEST_METHOD(for_each_iterator) {
			std::vector<int> vec_ints = { 0, 5, 10, 15 };
			int i_iteration_count = 0;

			util::for_each_iterator(vec_ints.begin(), vec_ints.end(), 0, [&](int i_iterator, int i_int) {
				i_iteration_count++;
				});

			Assert::AreEqual(4, i_iteration_count);
		}

		TEST_METHOD(is_between) {
			int i_min = test_util::generate_random_int_range(1, 20);
			int i_max = test_util::generate_random_int_range(40, 60);
			int i_compare = test_util::generate_random_int_range(21, 39);

			Assert::IsTrue(util::is_between(i_min, i_max, i_compare));
		}

		TEST_METHOD(is_less_than_or_equal_to) {
			int i_number = test_util::generate_random_int_range(1, 40);
			int i_compare = test_util::generate_random_int_range(40, 60);

			Assert::IsTrue(util::is_less_than_or_equal_to(i_number, i_compare));
		}

		TEST_METHOD(is_more_than_or_equal_to) {
			int i_number = test_util::generate_random_int_range(60, 120);
			int i_compare = test_util::generate_random_int_range(10, 60);

			Assert::IsTrue(util::is_more_than_or_equal_to(i_number, i_compare));
		}

		TEST_METHOD(is_more_than) {
			int i_number = test_util::generate_random_int_range(61, 120);
			int i_compare = test_util::generate_random_int_range(10, 60);

			Assert::IsTrue(util::is_more_than(i_number, i_compare));
		}

		TEST_METHOD(is_less_than) {
			int i_number = test_util::generate_random_int_range(1, 59);
			int i_compare = test_util::generate_random_int_range(60, 120);

			Assert::IsTrue(util::is_less_than(i_number, i_compare));
		}

		TEST_METHOD(get_current_datetime) {
			std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct std::tm tm_compare;
			localtime_s(&tm_compare, &date);

			std::tm tm_result = util::get_current_datetime();

			// Can't really safely compare more specific than min as this runs risk of the test failing if the time between getting the compare date and the current date takes too long.
			Assert::AreEqual(tm_compare.tm_year, tm_result.tm_year);
			Assert::AreEqual(tm_compare.tm_mon, tm_result.tm_mon);
			Assert::AreEqual(tm_compare.tm_mday, tm_result.tm_mday);
			Assert::AreEqual(tm_compare.tm_hour, tm_result.tm_hour);
			Assert::AreEqual(tm_compare.tm_min, tm_result.tm_min);
		}

		TEST_METHOD(tm_to_filesafe_string) {
			std::tm tm = util::get_current_datetime();
			std::stringstream ssBuffer;
			ssBuffer << std::put_time(&tm, "%F_%I-%M-%S_%p");
			std::string str_expected = ssBuffer.str();

			std::string str_actual = util::tm_to_filesafe_str(tm);
			Assert::AreEqual(str_expected, str_actual);
		}
	};
}