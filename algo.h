
namespace sc
{

    template <typename T>
    constexpr const T& max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    constexpr const T& max(T& value)
    {
        return value;
    }

    template <typename T, typename... Ts>
    constexpr const T& max(T& first, Ts&... rest)
    {
        const T& tail_max = max(rest...);
        return first > tail_max ? first : tail_max;
    }

    template <typename T>
    constexpr T max_value(T v)
    {
        return v;
    }

    template <typename T, typename... Ts>
    constexpr T max_value(T v, Ts... vs)
    {
        const T rest_max = max_value(vs...);
        return (v > rest_max) ? v : rest_max;
    }

} // namespace sc
