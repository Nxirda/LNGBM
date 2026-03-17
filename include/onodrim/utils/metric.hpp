#pragma once

namespace onodrim {
struct metric {
  template <typename CT, typename Ts>
  friend std::basic_ostream<CT, Ts> &operator<<(std::basic_ostream<CT, Ts> &os,
                                                metric) noexcept {
    // Need a typer to generate the unique name of the symbol
    os << "Metric : ";
  }
};

struct mae_t : metric {};
struct mape_t : metric {};
struct rmse_t : metric {};
struct rv_t : metric {};

inline constexpr mae_t mae{};
inline constexpr mape_t mape{};
inline constexpr rmse_t rmse{};
inline constexpr rv_t rv{};
} // namespace onodrim
