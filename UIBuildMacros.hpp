#define needs_base(x) std::derived_from<x> = T
#define needs_same(x) std::same_as<x> = T

#define setter_n(base, name, oldname, arg, param) \
    template <needs_base(base)> \
    Build<T> name arg {\
        this->m_item->oldname param;\
        return *this;\
    }

#define arg_1(t0) (t0 p0)
#define arg_2(t0, t1) (t0 p0, t1 p1)
#define arg_3(t0, t1, t2) (t0 p0, t1 p1, t2 p2)

#define setter_0(base, name, oldname) setter_n(base, name, oldname, (void), ())
#define setter_1(base, name, oldname, t0) setter_n(base, name, oldname, arg_1(t0), (p0))
#define setter_2(base, name, oldname, t0, t1) setter_n(base, name, oldname, arg_2(t0, t1), (p0, p1))
#define setter_3(base, name, oldname, t0, t1, t2) setter_n(base, name, oldname, arg_3(t0, t1, t2), (p0, p1, p2))


#define NLIST(p0, p1, p2, p3, N, ...) N
#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)
#define setter(base, name, oldname, ...) CONCAT(setter_, NLIST(5, ##__VA_ARGS__, 3, 2, 1, 0))(base, name, oldname, ##__VA_ARGS__)
