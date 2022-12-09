#pragma once

#include "glm/glm.hpp"

namespace zeus
{
    enum class Side
    {
        TOP_LEFT, TOP_RIGHT,
        BOTTOM_LEFT, BOTTOM_RIGHT,
        NONE
    };

    struct Point
    {
        float PosX;
        float PosY;
    };

    class Rectangle
    {
    public:
        float PosX;
        float PosY;
        float CenterX;
        float CenterY;
        float Width;
        float Height;

    public:
        Rectangle(float x, float y, float w, float h)
            : PosX(x), PosY(y), CenterX(x + w / 2), CenterY(y + h / 2), Width(w), Height(h)
        {
        }

        bool IsIntersectingWith(const Rectangle& rect, float range = 0.0f)
        {
            std::vector<glm::vec2> corners = {
                { PosX - Width, PosY - Height },
                { PosX + Width, PosY - Height },
                { PosX + Width, PosY + Height },
                { PosX - Width, PosY + Height }
            };

            for (const glm::vec2& point : corners)
            {
                bool xCollision = rect.PosX + rect.Width + range >= point.x && rect.PosX - rect.Width - range <= point.x;
                bool yCollision = rect.PosY + rect.Height + range >= point.y && rect.PosY - rect.Height - range <= point.y;
            
                if (xCollision && yCollision)
                {
                    return true;
                }
            }

            return false;
        }

        Side IntersectingSide(const Point& point, float range = 0.0f)
        {
            bool right = PosX + Width >= point.PosX && CenterX <= point.PosX && PosY >= point.PosY && PosY - Height <= point.PosY;
            bool left = PosX <= point.PosX && CenterX >= point.PosX && PosY >= point.PosY && PosY - Height <= point.PosY;
            bool top = PosX <= point.PosX && PosX + Width >= point.PosX && PosY >= point.PosY && CenterY <= point.PosY;
            bool bottom = PosX <= point.PosX && PosX + Width >= point.PosX && PosY + Height <= point.PosY && CenterY >= point.PosY;
            
            if (top && right)
            {
                return Side::TOP_RIGHT;
            }
            else if (top && left)
            {
                return Side::TOP_LEFT;
            }
            else if (bottom && right)
            {
                return Side::BOTTOM_RIGHT;
            }
            else if (bottom && left)
            {
                return Side::BOTTOM_LEFT;
            }

            return Side::NONE;
        }
    };
}
