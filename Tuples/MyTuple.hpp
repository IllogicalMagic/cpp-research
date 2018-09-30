#include <cstddef>
#include <tuple>
#include <algorithm>

// Wrappers for overloading {
template<size_t... Ns>
struct NWrap;

template<size_t I>
struct IWrap {};

template<typename T>
struct TWrap {};
// }

// Wrapper for reference {
template<typename T>
struct RefToPtr {
  using type = T;
};

template<typename T>
struct RefToPtr<T &> {
  using type = T *const;
};

template<typename T>
struct RefToPtr<T &&> {
  using type = T *const;
};
// }

// Base classes {
template<size_t MNum, size_t Align, typename... Ts>
struct TupleImpl;

template<size_t MNum, size_t Align, typename... Ts>
struct TupleRealign;

template<typename... Ts>
struct Tuple;
// }

// Get {
template<size_t N, typename... Ts>
auto &get(Tuple<Ts...> &t) {
  return t.get(IWrap<N>());
}

template<typename T, typename... Ts>
auto &get(Tuple<Ts...> &t) {
  return t.get(TWrap<T>());
}

template<size_t N, typename... Ts>
auto &&get(Tuple<Ts...> &&t) {
  return t.get(IWrap<N>());
}

template<typename T, typename... Ts>
auto &&get(Tuple<Ts...> &&t) {
  return t.get(TWrap<T>());
}
// }

template<typename T>
using r2p = typename RefToPtr<T>::type;

template<size_t MNum, size_t Align, size_t LastOffset, size_t... Offsets, typename T, typename... Ts>
struct TupleRealign<MNum, Align, NWrap<LastOffset, Offsets...>, T, Ts...> :
  TupleImpl<MNum + 1, std::max(Align, alignof(r2p<T>)), NWrap<LastOffset + sizeof(r2p<T>), LastOffset, Offsets...>, Ts...> {

  using Base = TupleImpl<MNum + 1, std::max(Align, alignof(r2p<T>)), NWrap<LastOffset + sizeof(r2p<T>), LastOffset, Offsets...>, Ts...>;

  using Base::_storage;
  using Base::_offs;
  using Base::get;
  using Base::getValStorage;
  using Base::getAlignedOffset;
  using Base::traceInheritance;

  // For debugging purposes.
  void static traceInheritance() {
    printf("%s\n", typeid(TupleRealign).name());
    Base::traceInheritance();
  };
};

template<size_t MNum, size_t Align, size_t LastOffset, size_t... Offsets, typename T, typename... Ts>
struct TupleImpl<MNum, Align, NWrap<LastOffset, Offsets...>, T, Ts...> :
  std::conditional_t<(LastOffset & (alignof(r2p<T>) - 1)) == 0,
    // Aligned case.
    TupleImpl<MNum + 1, std::max(Align, alignof(r2p<T>)), NWrap<LastOffset + sizeof(r2p<T>), LastOffset, Offsets...>, Ts...>,
    // Realign and try again.
    TupleRealign<MNum, Align, NWrap<((LastOffset - 1 + alignof(r2p<T>)) & ~(alignof(r2p<T>) - 1)), Offsets...>, T, Ts...>> {

  using Base = std::conditional_t<(LastOffset & (alignof(r2p<T>) - 1)) == 0,
                                  // Aligned case.
                                  TupleImpl<MNum + 1, std::max(Align, alignof(r2p<T>)), NWrap<LastOffset + sizeof(r2p<T>), LastOffset, Offsets...>, Ts...>,
                                  // Realign and try again.
                                  TupleRealign<MNum, Align, NWrap<(LastOffset & ~(alignof(r2p<T>) - 1)) + alignof(r2p<T>), Offsets...>, T, Ts...>>;
  using Base::_storage;
  using Base::_offs;
  using Base::get;
  using Base::getValStorage;
  using Base::getAlignedOffset;
  using Base::traceInheritance;

  // For debugging purposes.
  void static traceInheritance() {
    printf("%s\n", typeid(TupleImpl).name());
    Base::traceInheritance();
  };


  static constexpr size_t getAlignedOffset(IWrap<MNum>&&) {
    // return _offs[MNum];
    return ((LastOffset - 1 + alignof(r2p<T>)) & ~(alignof(r2p<T>) - 1));
  }

  r2p<T> &getValStorage(IWrap<MNum>&&) {
    constexpr size_t Off = getAlignedOffset(IWrap<MNum>());
    return *reinterpret_cast<r2p<T> *>(_storage + Off);
  }


  T &get(IWrap<MNum>&&) {
    constexpr size_t Off = getAlignedOffset(IWrap<MNum>());
    if constexpr (std::is_reference_v<T>) {
      return **reinterpret_cast<r2p<T> *>(_storage + Off);
    } else {
      return *reinterpret_cast<T *>(_storage + Off);
    }
  }

  const T &get(IWrap<MNum>&&) const {
    constexpr size_t Off = getAlignedOffset(IWrap<MNum>());
    if constexpr (std::is_reference_v<T>) {
        return **reinterpret_cast<const r2p<T> *>(_storage + Off);
      } else {
      return *reinterpret_cast<const T *>(_storage + Off);
    }
  }

  T &get(TWrap<T>&&) {
    return get(IWrap<MNum>());
  }

  const T &get(TWrap<T>&&) const {
    return get(IWrap<MNum>());
  }
};

// End of recursion.
template<size_t MNum, size_t Align, size_t TotalLen, size_t... Offsets>
struct TupleImpl<MNum, Align, NWrap<TotalLen, Offsets...>> {
  static constexpr size_t _offs[] = {Offsets...};
  union {
    std::aligned_storage_t<TotalLen, Align> _align;
    std::byte _storage[TotalLen];
  };

  void get(...);
  void getValStorage(...);
  static constexpr size_t getAlignedOffset(...);

  // For debugging purposes.
  void static traceInheritance() {
    printf("%s\n", typeid(TupleImpl).name());
  };
};

// Wrapper for reference argument {
template<typename T>
struct refToPtr {
  static T &&f(T &&arg) { return std::forward<T>(arg); }
  static T &f(T &arg) { return arg; }
  static const T &f(const T &arg) { return arg; }
};

template<typename T>
struct refToPtr<T &> {
  static T *const f(T &arg) { return &arg; }
  static const T *const f(const T &arg) { return &arg; }
};

template<typename T>
struct refToPtr<T &&> {
  static T *const f(T &&arg) { return &arg; }
};
// }

template<typename... Ts>
struct Tuple : TupleImpl<0, 0, NWrap<0>, Ts...> {
private:
  using Base = TupleImpl<0, 0, NWrap<0>, Ts...>;

  using Base::getAlignedOffset;
  using Base::_storage;
  using Base::getValStorage;
  using Base::traceInheritance;

  template<size_t... Ns, typename... As>
  void tupleInitHelper(std::index_sequence<Ns...>&&, As&&... args) {
    int a[] = {(new (_storage + getAlignedOffset(IWrap<Ns>())) r2p<Ts>(refToPtr<Ts>::f(std::forward<As>(args))), 0)...};
  }

  template<size_t... Ns>
  void tupleDestroyHelper(std::index_sequence<Ns...>&&) {
    // This call to destructor can break compiler.
    int a[] = {(getValStorage(IWrap<Ns>()).~r2p<Ts>(), 0)...};
  }

  // Helper ctor for copy ctor.
  template<size_t... Ns>
  Tuple(std::index_sequence<Ns...>&&, const Tuple &Rhs):
    Tuple(Rhs.get(IWrap<Ns>())...) {}

  // Helper ctor for move ctor.
  template<size_t... Ns>
  Tuple(std::index_sequence<Ns...>&&, Tuple &&Rhs):
    Tuple(std::forward<Ts>(Rhs.get(IWrap<Ns>()))...) {}

  using IdxSeq = std::make_index_sequence<sizeof...(Ts)>;

public:
  // Dump class hierarchy.
  // For debugging purposes.
  void static traceInheritance() {
    printf("%s\n", typeid(Tuple).name());
    Base::traceInheritance();
  };

  using Base::get;

  // Main ctor.
  template<typename... As,
           typename = std::enable_if_t<(... && std::is_constructible_v<Ts, As>), void>>
  Tuple(As&&...args) { tupleInitHelper(IdxSeq(), std::forward<As>(args)...); }

  // Default ctor: use main ctor with default values.
  Tuple(): Tuple(Ts()...) {}
  // Copy ctor.
  Tuple(const Tuple &Rhs): Tuple(IdxSeq(), Rhs) {}
  // Move ctor.
  Tuple(Tuple &&Rhs): Tuple(IdxSeq(), std::forward<Tuple>(Rhs)) {}

  ~Tuple() { tupleDestroyHelper(IdxSeq()); }
};
