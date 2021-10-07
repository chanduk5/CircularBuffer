
#include "gtest/gtest.h"
#include "pch.h"
#include <stdexcept>
#include <cctype>

#include "../circularBuffer.hpp"


// Circular-buffer exercise test case data version 1.2.0
TEST(test_reading_empty_buffer_should_fail, reading_empty_buffer_should_fail)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_THROW(buffer.read(), std::domain_error);
}

TEST(test_can_read_an_item_just_written, can_read_an_item_just_written)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_each_item_may_only_be_read_once, each_item_may_only_be_read_once)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_THROW(buffer.read(), std::domain_error);
}

TEST(test_items_are_read_in_the_order_they_are_written, items_are_read_in_the_order_they_are_written)
{
    circular_buffer::circular_buffer<int> buffer(2);
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_NO_THROW(buffer.write(2));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    expected = 2;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_full_buffer_cant_be_written, full_buffer_cant_be_written)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_THROW(buffer.write(2), std::domain_error);
}

TEST(test_a_read_frees_up_capacity_for_another_write, a_read_frees_up_capacity_for_another_write)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_NO_THROW(buffer.write(2));
    expected = 2;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_read_position_is_maintained_even_across_multiple_writes, read_position_is_maintained_even_across_multiple_writes)
{
    circular_buffer::circular_buffer<int> buffer(3);
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_NO_THROW(buffer.write(2));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_NO_THROW(buffer.write(3));
    expected = 2;
    EXPECT_TRUE(expected == buffer.read());
    expected = 3;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_items_cleared_out_of_buffer_cant_be_read, items_cleared_out_of_buffer_cant_be_read)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    buffer.clear();
    EXPECT_THROW(buffer.read(), std::domain_error);
}

TEST(test_clear_frees_up_capacity_for_another_write, clear_frees_up_capacity_for_another_write)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    buffer.clear();
    EXPECT_NO_THROW(buffer.write(2));
    int expected = 2;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_clear_does_nothing_on_empty_buffer, clear_does_nothing_on_empty_buffer)
{
    circular_buffer::circular_buffer<int> buffer(1);
    buffer.clear();
    EXPECT_NO_THROW(buffer.write(1));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_overwrite_acts_like_write_on_non_full_buffer, overwrite_acts_like_write_on_non_full_buffer)
{
    circular_buffer::circular_buffer<int> buffer(2);
    EXPECT_NO_THROW(buffer.write(1));
    buffer.overwrite(2);
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    expected = 2;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_overwrite_replaces_the_oldest_item_on_full_buffer, overwrite_replaces_the_oldest_item_on_full_buffer)
{
    circular_buffer::circular_buffer<int> buffer(2);
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_NO_THROW(buffer.write(2));
    buffer.overwrite(3);
    int expected = 2;
    EXPECT_TRUE(expected == buffer.read());
    expected = 3;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_overwrite_replaces_the_oldest_item_remaining_in_buffer_following_a_read, overwrite_replaces_the_oldest_item_remaining_in_buffer_following_a_read)
{
    circular_buffer::circular_buffer<int> buffer(3);
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_NO_THROW(buffer.write(2));
    EXPECT_NO_THROW(buffer.write(3));
    int expected = 1;
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_NO_THROW(buffer.write(4));
    buffer.overwrite(5);
    expected = 3;
    EXPECT_TRUE(expected == buffer.read());
    expected = 4;
    EXPECT_TRUE(expected == buffer.read());
    expected = 5;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_full_buffer_cant_be_written_after_overwrite, full_buffer_cant_be_written_after_overwrite)
{
    circular_buffer::circular_buffer<int> buffer(1);
    EXPECT_NO_THROW(buffer.write(1));
    buffer.overwrite(2);
    EXPECT_THROW(buffer.write(3), std::domain_error);
    int expected = 2;
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_check_correctness_with_string_type, check_correctness_with_string_type)
{
    circular_buffer::circular_buffer<std::string> buffer(3);
    EXPECT_NO_THROW(buffer.write("hello"));
    EXPECT_NO_THROW(buffer.write("world"));
    EXPECT_NO_THROW(buffer.write("zombies"));
    std::string expected = "hello";
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_NO_THROW(buffer.write("pants"));
    buffer.overwrite("banana");
    expected = "zombies";
    EXPECT_TRUE(expected == buffer.read());
    expected = "pants";
    EXPECT_TRUE(expected == buffer.read());
    expected = "banana";
    EXPECT_TRUE(expected == buffer.read());
}

TEST(test_initial_clear_does_not_affect_wrapping_around, initial_clear_does_not_affect_wrapping_around)
{
    circular_buffer::circular_buffer<int> buffer(2);
    buffer.clear();
    EXPECT_NO_THROW(buffer.write(1));
    EXPECT_NO_THROW(buffer.write(2));
    buffer.overwrite(3);
    buffer.overwrite(4);
    int expected = 3;
    EXPECT_TRUE(expected == buffer.read());
    expected = 4;
    EXPECT_TRUE(expected == buffer.read());
    EXPECT_THROW(buffer.read(), std::domain_error);
} 
