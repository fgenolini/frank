#pragma once

namespace test::frank {

extern void do_nothing_abort(void *mock_data) noexcept;

extern void do_nothing_exit(int result, void *mock_data) noexcept;

} // namespace test::frank
