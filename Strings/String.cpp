#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <tuple>
#include <vector>

template<typename CharT, typename Traits, typename Alloc>
static int replace_all_easy(std::basic_string<CharT, Traits, Alloc> &S,
                            const std::basic_string_view<CharT, Traits> &From,
                            const std::basic_string_view<CharT, Traits> &To) {
  using StrType = std::basic_string<CharT, Traits, Alloc>;
  StrType Res;
  Res.reserve(S.size());

  int ReplCnt = 0;
  auto AdvanceBy = From.size();
  decltype(AdvanceBy) SPos = 0;
  for (auto Pos = S.find(From) ; Pos != S.npos;
        Pos = S.find(From, Pos + AdvanceBy)) {
    Res.append(S, SPos, Pos - SPos);
    SPos = Pos + AdvanceBy;
    Res.append(To);
    ++ReplCnt;
  }

  Res.append(S, SPos);

  S.swap(Res);
  return ReplCnt;
}

template<typename CharT, typename Traits, typename Alloc>
static int replace_all_hard(std::basic_string<CharT, Traits, Alloc> &S,
                            const std::basic_string_view<CharT, Traits> &From,
                            const std::basic_string_view<CharT, Traits> &To) {
  // Estimate replacement count.
  auto AdvanceBy = From.size();
  std::vector<decltype(AdvanceBy)> CachedPos;
  // From can not be found in S more that S.size() times.
  CachedPos.reserve(S.size());

  // Find all positions and estimate result size.
  // This allows to avoid reallocations.
  for (auto Pos = S.find(From); Pos != S.npos;
       Pos = S.find(From, Pos + AdvanceBy)) {
    CachedPos.push_back(Pos);
  }

  auto SZDiff = To.size() - From.size();
  std::cerr << "SZDiff = " << SZDiff << ", total size = " << S.size() + SZDiff << std::endl;

  using StrType = std::basic_string<CharT, Traits, Alloc>;
  StrType Res;
  Res.reserve(S.size() + SZDiff * CachedPos.size());
  decltype(AdvanceBy) SPos = 0;
  for (auto Pos : CachedPos) {
    Res.append(S, SPos, Pos - SPos);
    SPos = Pos + AdvanceBy;
    Res.append(To);
  }
  Res.append(S, SPos);

  S.swap(Res);
  return CachedPos.size();
}

template<typename CharT, typename Traits, typename Alloc>
int replace_all(std::basic_string<CharT, Traits, Alloc> &S,
                const std::basic_string_view<CharT, Traits> &From,
                const std::basic_string_view<CharT, Traits> &To) {
  // No additional reallocations can happen.
  if (From.size() >= To.size()) {
    std::cerr << "Easy case\n";
    return replace_all_easy(S, From, To);
  }

  // Hard case is when we are replacing small string on bigger string.
  // Done stupid way (just replace every occurence without any resulting size estimations)
  // it can cause unnecessary reallocations.
  std::cerr << "Hard case\n";
  return replace_all_hard(S, From, To);
}

// Just for checks.
template<typename CharT, typename Traits, typename Alloc>
int replace_all_stupid(std::basic_string<CharT, Traits, Alloc> &S,
                       const std::basic_string_view<CharT, Traits> &From,
                       const std::basic_string_view<CharT, Traits> &To) {
  int ReplCnt = 0;
  auto FSize = From.size();
  auto TSize = To.size();
  for (auto Pos = S.find(From); Pos != S.npos;
       Pos = S.find(From, Pos + TSize)) {
    S.replace(Pos, FSize, To);
    ++ReplCnt;
  }
  return ReplCnt;
}

// Version without separator.
template<typename ResStr, typename... Args>
ResStr combine(Args &&... args) {
  std::basic_ostringstream<typename ResStr::value_type,
                           typename ResStr::traits_type,
                           typename ResStr::allocator_type> Res;

  (Res << ... << args);
  return Res.str();
}

// Combine with no arguments.
template<typename ResStr, typename... Args>
ResStr combine_sep(const std::basic_string_view<typename ResStr::value_type, typename ResStr::traits_type> Sep) {
  return ResStr();
}

// Workaround for gcc complaints about folding expressions.
template<typename X, typename Y>
int output_arg(X &&x, Y &&y) {
  x << y;
  return 0;
}

// Combine with separator.
template<typename ResStr, typename Arg1, typename... Args>
ResStr combine_sep(const std::basic_string_view<typename ResStr::value_type, typename ResStr::traits_type> Sep,
                   Arg1 &&arg1, Args &&... args) {
  std::basic_ostringstream<typename ResStr::value_type,
                           typename ResStr::traits_type,
                           typename ResStr::allocator_type> Res;
  Res << arg1;
  int a[sizeof...(Args)] = {output_arg(Res << Sep, args)...};
  return Res.str();
}

int main(int argc, char **argv) {
  assert(argc == 4 && "Args: string, from, to");
  std::string S = argv[1];
  int RCnt = replace_all(S, std::string_view(argv[2]), std::string_view(argv[3]));
  std::cout << "Replaced " << RCnt << " occurences of " << argv[2] << " with " << argv[3] << ".\nResulting string is: " << S << std::endl;
  std::cout << "Result size is " << S.size() << std::endl;

  std::string S2 = argv[1];
  RCnt = replace_all_stupid(S2, std::string_view(argv[2]), std::string_view(argv[3]));
  std::cout << "Replaced " << RCnt << " occurences of " << argv[2] << " with " << argv[3] << ".\nResulting string is: " << S2 << std::endl;
  std::cout << "Result size is " << S.size() << std::endl;

  assert(S == S2 && "replace_all is bad");

  // Combine test.
  std::cout << "Combine: " << combine_sep<decltype(S)>("<<<SEPARATOR>>>", argv[2], argv[3], "NUMS:", 5,  2.0) << std::endl;
  std::cout << "Combine: " << combine_sep<decltype(S)>("<<<SEPARATOR>>>", argv[2]) << std::endl;
  std::cout << "Combine: " << combine_sep<decltype(S)>("<<<SEPARATOR>>>") << std::endl;
  return 0;
}
