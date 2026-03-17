#pragma once

namespace onodrim
{
  template<typename... Ts, typename Allocator>
  struct dataset 
  {
    using types = std::tuple<Ts...>;
    
    constexpr dataset(std::size_t size, types t, Allocator a) : size_{size}, fields{t}
    {
      // Need to handle padding
      data_ = a.allocate(size * sizeof...(Ts));
    }

    constexpr decltype(auto) operator[](label, std::size_t i)
    {
      // compute offset based on label and the associated value in the tuple
      return data[i];
    };

    constexpr std::size_t size() const noexcept
    {
      return size_;
    }

    constexpr std::size_t labels() const noexcept
    {
      return ...;
    }
      
    constexpr bool empty() const noexcept
    {
      return size_ == 0;
    }

    std::size_t size_; 
    std::byte   *data_;
    types       fields;
  };
}
