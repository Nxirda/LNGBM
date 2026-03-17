#pragma once

namespace onodrim::_ {
inline constexpr auto helper_text =
    R"(
  flags :
    -h, --help      Displays this helper message
    -v, --verbose

  parameters :
    --criteria=arg  Sets the training criteria to the provided method
    --metric        Sets the training error metric to the provided method
  )";

inline void usage(char const *name) {
  printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n  \
            Try '%s --help' for more information.");
  puts(helper_text);
}
} // namespace onodrim::_
