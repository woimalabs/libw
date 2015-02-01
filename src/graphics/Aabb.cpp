#include "w/graphics/Aabb.hpp"

namespace w
{
namespace graphics
{

Aabb::Aabb() :
    x_(0.0),
    y_(0.0),
    z_(0.0),
    dx_(0.0),
    dy_(0.0),
    dz_(0.0),
    empty_(true)
{
}

Aabb::Aabb(Eigen::Vector4f const& v1, Eigen::Vector4f const& v2) :
    x_(0.0),
    y_(0.0),
    z_(0.0),
    dx_(0.0),
    dy_(0.0),
    dz_(0.0),
    empty_(false)
{
    x_ = std::min(v1[0], v2[0]);
    y_ = std::min(v1[1], v2[1]);
    z_ = std::min(v1[2], v2[2]);

    float x = std::max(v1[0], v2[0]);
    float y = std::max(v1[1], v2[1]);
    float z = std::max(v1[2], v2[2]);

    dx_ = x - x_;
    dy_ = y - y_;
    dz_ = z - z_;
}

Aabb::Aabb(Eigen::Vector3f const& v1, Eigen::Vector3f const& v2) :
    x_(0.0),
    y_(0.0),
    z_(0.0),
    dx_(0.0),
    dy_(0.0),
    dz_(0.0),
    empty_(false)
{
    x_ = std::min(v1[0], v2[0]);
    y_ = std::min(v1[1], v2[1]);
    z_ = std::min(v1[2], v2[2]);

    float x = std::max(v1[0], v2[0]);
    float y = std::max(v1[1], v2[1]);
    float z = std::max(v1[2], v2[2]);

    dx_ = x - x_;
    dy_ = y - y_;
    dz_ = z - z_;
}

Aabb::Aabb(Aabb const& other) :
    x_(other.x_),
    y_(other.y_),
    z_(other.z_),
    dx_(other.dx_),
    dy_(other.dy_),
    dz_(other.dz_),
    empty_(other.empty_)
{
}

Aabb const& Aabb::operator=(Aabb const& other)
{
    if (this == &other)
        return *this;

    empty_ = other.empty_;
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
    dx_ = other.dx_;
    dy_ = other.dy_;
    dz_ = other.dz_;

    return *this;
}

Aabb::~Aabb()
{
}

Aabb Aabb::fromData(void const* data, size_t size, size_t offset,
        size_t stride)
{
    char const* ptr = (char const*) data;
    ptr += offset;

    char const* end = (char const*) data;
    end += size;

    Aabb result;
    while (ptr <= (end - 3*sizeof(float)))
    {
        float const* xyz = (float const*)ptr;
        result += Eigen::Vector3f(xyz[0], xyz[1], xyz[2]);
        ptr += stride;
    }

    return result;
}

bool Aabb::operator==(Aabb const& other) const
{
    if (empty_ && other.empty_)
        return true;

    if (std::abs(x_ - other.x_) > 0.0001)
        return false;
    if (std::abs(y_ - other.y_) > 0.0001)
        return false;
    if (std::abs(z_ - other.z_) > 0.0001)
        return false;
    if (std::abs(dx_ - other.dx_) > 0.0001)
        return false;
    if (std::abs(dy_ - other.dy_) > 0.0001)
        return false;
    if (std::abs(dz_ - other.dz_) > 0.0001)
        return false;

    return true;
}

bool Aabb::operator<(Aabb const& other) const
{
    if (empty_ && !other.empty_)
        return true;
    if (empty_ && other.empty_)
        return false;

    if (x_ < other.x_ - 0.0001)
        return true;
    if (y_ < other.y_ - 0.0001)
        return true;
    if (z_ < other.z_ - 0.0001)
        return true;
    if (dx_ < other.dx_ - 0.0001)
        return true;
    if (dy_ < other.dy_ - 0.0001)
        return true;
    if (dz_ < other.dz_ - 0.0001)
        return true;

    return false;
}

Aabb Aabb::operator+(Aabb const& other) const
{
    float x1 = x_ + dx_;
    float y1 = y_ + dy_;
    float z1 = z_ + dz_;
    float x2 = other.x_ + other.dx_;
    float y2 = other.y_ + other.dy_;
    float z2 = other.z_ + other.dz_;

    float x3 = std::min(x_, other.x_);
    float y3 = std::min(y_, other.y_);
    float z3 = std::min(z_, other.z_);
    float x4 = std::max(x1, x2);
    float y4 = std::max(y1, y2);
    float z4 = std::max(z1, z2);

    Aabb aabb;
    aabb.empty_ = empty_ && other.empty_;
    aabb.x_ = x3;
    aabb.y_ = y3;
    aabb.z_ = z3;
    aabb.dx_ = x4-x3;
    aabb.dy_ = y4-y3;
    aabb.dz_ = z4-z3;

    return aabb;
}

Aabb const& Aabb::operator+=(Aabb const& other)
{
    float x1 = x_ + dx_;
    float y1 = y_ + dy_;
    float z1 = z_ + dz_;
    float x2 = other.x_ + other.dx_;
    float y2 = other.y_ + other.dy_;
    float z2 = other.z_ + other.dz_;

    float x3 = std::min(x_, other.x_);
    float y3 = std::min(y_, other.y_);
    float z3 = std::min(z_, other.z_);
    float x4 = std::max(x1, x2);
    float y4 = std::max(y1, y2);
    float z4 = std::max(z1, z2);

    empty_ = empty_ && other.empty_;
    x_ = x3;
    y_ = y3;
    z_ = z3;
    dx_ = x4-x3;
    dy_ = y4-y3;
    dz_ = z4-z3;

    return *this;
}

Aabb const& Aabb::operator+=(Eigen::Vector4f const& v)
{
    return *this+=(Eigen::Vector3f(v[0], v[1], v[2]));
}

Aabb const& Aabb::operator+=(Eigen::Vector3f const& v)
{
    empty_ = false;
    float x1 = std::min(x_, v[0]);
    float y1 = std::min(y_, v[1]);
    float z1 = std::min(z_, v[2]);
    float x2 = std::max(v[0], x_+dx_);
    float y2 = std::max(v[1], y_+dy_);
    float z2 = std::max(v[2], z_+dz_);
    x_ = x1;
    y_ = y1;
    z_ = z1;
    dx_ = x2-x1;
    dy_ = y2-y1;
    dz_ = z2-z1;

    return *this;
}

Aabb Aabb::operator+(Eigen::Vector4f const& v) const
{
    return *this + Eigen::Vector3f(v[0], v[1], v[2]);
}

Aabb Aabb::operator+(Eigen::Vector3f const& v) const
{
    Aabb aabb;
    aabb.empty_ = false;

    if (empty_)
    {
        aabb.x_ = v[0];
        aabb.y_ = v[1];
        aabb.z_ = v[2];
        aabb.dx_ = 0.0;
        aabb.dy_ = 0.0;
        aabb.dz_ = 0.0;
    }
    else
    {
        float x1 = std::min(x_, v[0]);
        float y1 = std::min(y_, v[1]);
        float z1 = std::min(z_, v[2]);
        float x2 = std::max(v[0], x_+dx_);
        float y2 = std::max(v[1], y_+dy_);
        float z2 = std::max(v[2], z_+dz_);
        aabb.x_ = x1;
        aabb.y_ = y1;
        aabb.z_ = z1;
        aabb.dx_ = x2-x1;
        aabb.dy_ = y2-y1;
        aabb.dz_ = z2-z1;
    }

    return aabb;
}

Aabb Aabb::operator*(Eigen::Matrix4f const& matrix) const
{
    if (empty_)
    {
        return *this;
    }

    Eigen::Vector4f v(x_, y_, z_, 1.0);

    v = matrix*v;

    float dx1 = matrix(0,0)*dx_;
    float dy1 = matrix(1,0)*dx_;
    float dz1 = matrix(2,0)*dx_;
    float dw1 = matrix(3,0)*dx_;

    float dx2 = matrix(0,1)*dy_;
    float dy2 = matrix(1,1)*dy_;
    float dz2 = matrix(2,1)*dy_;
    float dw2 = matrix(3,1)*dy_;

    float dx3 = matrix(0,2)*dz_;
    float dy3 = matrix(1,2)*dz_;
    float dz3 = matrix(2,2)*dz_;
    float dw3 = matrix(3,2)*dz_;

    float w1 = v[3];
    float x1 = v[0] / w1;
    float y1 = v[1] / w1;
    float z1 = v[2] / w1;

    float w2 = v[3] + dw1;
    float x2 = (v[0] + dx1) / w2;
    float y2 = (v[1] + dy1) / w2;
    float z2 = (v[2] + dz1) / w2;

    float w3 = v[3] + dw2;
    float x3 = (v[0] + dx2) / w3;
    float y3 = (v[1] + dy2) / w3;
    float z3 = (v[2] + dz2) / w3;

    //float w4 = v[3] + dw1 + dw2;
    float x4 = (v[0] + dx1 + dx2) / w3;
    float y4 = (v[1] + dy1 + dy2) / w3;
    float z4 = (v[2] + dz1 + dz2) / w3;

    //

    float w5 = (v[3] + dw3);
    float x5 = (v[0] + dx3) / w5;
    float y5 = (v[1] + dy3) / w5;
    float z5 = (v[2] + dz3) / w5;

    float w6 = (v[3] + dw1 + dw3);
    float x6 = (v[0] + dx1 + dx3) / w6;
    float y6 = (v[1] + dy1 + dy3) / w6;
    float z6 = (v[2] + dz1 + dz3) / w6;

    float w7 = (v[3] + dw2 + dw3);
    float x7 = (v[0] + dx2 + dx3) / w7;
    float y7 = (v[1] + dy2 + dy3) / w7;
    float z7 = (v[2] + dz2 + dz3) / w7;

    float w8 = (v[3] + dw1 + dw2 + dw3);
    float x8 = (v[0] + dx1 + dx2 + dx3) / w8;
    float y8 = (v[1] + dy1 + dy2 + dy3) / w8;
    float z8 = (v[2] + dz1 + dz2 + dz3) / w8;

    Aabb aabb;
    aabb.empty_ = empty_;
    aabb.x_ = std::min(std::min(std::min(std::min(std::min(std::min(
                                std::min(x1, x2), x3), x4), x5), x6), x7), x8);

    aabb.y_ = std::min(std::min(std::min(std::min(std::min(std::min(
                                std::min(y1, y2), y3), y4), y5), y6), y7), y8);

    aabb.z_ = std::min(std::min(std::min(std::min(std::min(std::min(
                                std::min(z1, z2), z3), z4), z5), z6), z7), z8);

    float x = std::max(std::max(std::max(std::max(std::max(std::max(
                                std::max(x1, x2), x3), x4), x5), x6), x7), x8);

    float y = std::max(std::max(std::max(std::max(std::max(std::max(
                                std::max(y1, y2), y3), y4), y5), y6), y7), y8);

    float z = std::max(std::max(std::max(std::max(std::max(std::max(
                                std::max(z1, z2), z3), z4), z5), z6), z7), z8);

    aabb.dx_ = x - aabb.x_;
    aabb.dy_ = y - aabb.y_;
    aabb.dz_ = z - aabb.z_;

    return aabb;
}

bool Aabb::isEmpty() const
{
    return empty_;
}

float Aabb::getVolume() const
{
    return dx_ * dy_ * dz_;
}

Eigen::Vector3f Aabb::getMin() const
{
    return Eigen::Vector3f(x_, y_, z_);
}

Eigen::Vector3f Aabb::getMax() const
{
    return Eigen::Vector3f(x_+dx_, y_+dy_, z_+dz_);
}

Eigen::Vector3f Aabb::getCenter() const
{
    return Eigen::Vector3f(x_+dx_/2.0, y_+dy_/2.0, z_+dz_/2.0);
}

Eigen::Vector3f Aabb::getDelta() const
{
    return Eigen::Vector3f(dx_, dy_, dz_);
}

float Aabb::getLongestSide() const
{
    return std::max(dx_, std::max(dy_, dz_));
}

bool Aabb::overlaps(Aabb const& other) const
{
    if (empty_ || other.empty_)
        return false;

    if(abs(x_+dx_/2.0 - (other.x_+other.dx_/2.0)) < (dx_/2.0 + other.dx_/2.0))
        return true;

    if(abs(y_+dy_/2.0 - (other.y_+other.dy_/2.0)) < (dy_/2.0 + other.dy_/2.0))
        return true;

    if(abs(z_+dz_/2.0 - (other.z_+other.dz_/2.0)) < (dz_/2.0 + other.dz_/2.0))
        return true;

    return false;
}

bool Aabb::contains(Eigen::Vector3f const& v) const
{
    if (empty_)
        return false;

    if (v[0] < x_)
        return false;
    if (v[1] < y_)
        return false;
    if (v[2] < z_)
        return false;

    if (v[0] > x_ + dx_)
        return false;
    if (v[1] > y_ + dy_)
        return false;
    if (v[2] > z_ + dz_)
        return false;

    return true;
}

} } // namespace

std::ostream& operator<<(std::ostream& stream, w::graphics::Aabb const& aabb)
{
    return stream << "AABB(" << aabb.getMin() << "->" << aabb.getMax() << ")";
}

