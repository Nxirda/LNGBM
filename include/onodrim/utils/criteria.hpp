#pragma once

namespace onodrim {
struct criteria {
  template <typename CT, typename Ts>
  friend std::basic_ostream<CT, Ts> &operator<<(std::basic_ostream<CT, Ts> &os,
                                                criteria) noexcept {
    // Need a typer to generate the unique name of the symbol
    os << "Criteria : ";
  }
};

struct percentile_t : criteria {};
struct quartile_t : criteria {};
struct random_t : criteria {};
struct uniform_t : criteria {};
struct unique_t : criteria {};

inline constexpr percentile_t percentile{};
inline constexpr quartile_t quartiles{};
inline constexpr random_t random{};
inline constexpr uniform_t uniform{};
inline constexpr unique_t unique{};
} // namespace onodrim
