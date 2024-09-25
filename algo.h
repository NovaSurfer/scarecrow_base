
namespace sc
{

    template <typename T>
    constexpr const T& max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }
} // namespace sc
