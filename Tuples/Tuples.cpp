#include <tuple>
#include <cstdio>
#include <type_traits>

#include "MyTuple.hpp"

struct Constr {
  int x;
  int this_num;

  static int next_num;

  Constr(): x(12), this_num(next_num++) {
    printf("Constructed %d\n", this_num);
  }

  Constr(Constr &&r): x(r.x), this_num(next_num++) {
    r.x = 0;
    printf("Move constructed %d from %d\n", this_num, r.this_num);
  }

  Constr(const Constr &r): x(r.x), this_num(next_num++) {
    printf("Copy constructed %d from %d\n", this_num, r.this_num);
  }

  ~Constr() {
    printf("Destructed %d\n", this_num);
  }
  void foo() const {
    printf("Foo %d from %d\n", x, this_num);
  }
};

int Constr::next_num = 0;

struct Empty {};

using Tpl2 = Tuple<int, Constr, Constr, Constr &, Constr>;
using STpl2 = std::tuple<int, Constr, Constr, Constr &, Constr>;

static_assert(std::is_standard_layout<Tpl2>::value, "Not a standard layout type!");
static_assert(!std::is_standard_layout<STpl2>::value, "Standard layout type!");

int main() {
  Constr c;

  // -------
  {
    printf("\nTuple\n");
    Tpl2 t(2, Constr(), Constr(), c, Constr());
    get<2>(t).foo();
    printf("--------\nCopy:\n");
    Tpl2 t2 = t;
    get<2>(t2).foo();

    printf("--------\nMove:\n");
    Tpl2 t3 = std::move(t2);
    get<2>(t3).foo();
    get<2>(t2).foo();
  }

  // --------
  Constr::next_num = 1;
  {
    printf("\nstd::tuple\n");
    STpl2 t(2, Constr(), Constr(), c, Constr());
    std::get<2>(t).foo();

    printf("--------\nCopy:\n");
    STpl2 t2 = t;
    std::get<2>(t2).foo();

    printf("--------\nMove:\n");
    STpl2 t3 = std::move(t2);
    std::get<2>(t3).foo();
    std::get<2>(t2).foo();
  }

  return 0;
}
