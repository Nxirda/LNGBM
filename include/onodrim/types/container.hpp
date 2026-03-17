#pragma once

namespace onodrim {
template <typename... Ts, typename Allocator> struct container {
  using types = std::tuple<Ts...>;

  std::size_t size_;
  std::byte *data_;
  types fields;
};
} // namespace onodrim
