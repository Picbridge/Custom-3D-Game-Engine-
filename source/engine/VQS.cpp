#include "pch.h"

VQS VQS::operator+(VQS const& other)
{
    return VQS(this->v + other.v, this->q + other.q, this->s + other.s);
}

VQS VQS::operator*(float c)
{
    return VQS(this->v * c, this->q * c, this->s * c);
}

VQS VQS::operator*(VQS const& other)
{
    return VQS(*this * other.v, this->q * other.q, this->s * other.s);
}

glm::vec3 VQS::operator*(glm::vec3 r)
{
    glm::vec3 temp = r * this->s;
    temp = Quaternion::Rotation(this->q, temp);
    temp += this->v;

    return temp;
}

VQS VQS::Inverse()
{
    Quaternion temp = q.Inverse();
    return VQS(Quaternion::Rotation(temp, -v/s), temp, 1/s);
}

VQS VQS::Identity()
{
    return VQS(glm::vec3(0), Quaternion::Identity(), 1);
}
