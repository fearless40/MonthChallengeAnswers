struct AABB
{
    int x, y;
    int x2, y2;
};

constexpr bool aabb_collision(const AABB &pos1, const AABB &pos2) noexcept
{
    if (((pos1.x >= pos2.x && pos1.x <= pos2.x2) || (pos1.x2 >= pos2.x && pos1.x2 <= pos2.x2)) &&
        ((pos1.y >= pos2.y && pos1.y <= pos2.y2) || (pos1.y2 >= pos2.y && pos1.y2 <= pos2.y2)))
    {
        return true;
    }
    return false;
}