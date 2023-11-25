#pragma once

#include <iostream>

// Вспомогательные структуры


struct Point3D {
    Point3D() = default;
    Point3D(long double x0, long double y0, long double z0)
        : x(x0), y(y0), z(z0)
    {

    }
    long double x = 0;
    long double y = 0;
    long double z = 0;
};

// Вспомогательные операторы для упрощения кода
std::ostream& operator<< (std::ostream& o, const Point3D& point) {
    o << "{" << point.x << "," << point.y << "," << point.z << "}";
    return o;
}

struct Vector3D {

    Point3D beg;
    Point3D end;
};

std::ostream& operator<< (std::ostream& o, const Vector3D& vector) {
    o << "{" << vector.beg << "=>" << vector.end << "}";
    return o;
}
// Интерфейс для последующих реализаций конкретных кривых
class Curve3D {

public:
    virtual Point3D TimeFunction(long double t) const = 0;
    virtual Vector3D FirstDerivative(long double t) const = 0;

    ~Curve3D() {
    }
};

class Circle : public Curve3D {
    Circle() = delete;
public:
    Circle(long double radius)
        : radius_(radius)
    {
        if (radius_ <= 0) {
            // Или стандартный параметр
            radius_ = 1;
            // Или выбросить исключение
            throw(std::domain_error("Invalid radii"));
        }
    }
    long double Radius() const {
        return radius_;
    }
    Point3D TimeFunction(long double t) const override {
        return Point3D(radius_ * std::cos(t), radius_ * std::sin(t), 0);
    }
    Vector3D FirstDerivative(long double t) const override {
        return { Point3D(), Point3D(-radius_ * std::sin(t), radius_ * std::cos(t), 0) };
    }
    ~Circle() {
    }
private:
    long double radius_;
};

bool operator< (const Circle& lh, const Circle& rh) {
    return lh.Radius() < rh.Radius();
}

class Ellipse : public Curve3D {
    Ellipse() = delete;
public:
    Ellipse(long double radius_x, long double radius_y)
        : radius_x_(radius_x), radius_y_(radius_y)
    {
        if (radius_x_ <= 0 || radius_y_ <= 0) {
            // Или стандартный параметр
            radius_x_ = 1;
            radius_y_ = 1;
            // Или выбросить исключение
            throw(std::domain_error("Invalid radii"));
        }
    }
    Point3D TimeFunction(long double t) const override {
        return Point3D(radius_x_ * std::cos(t), radius_y_ * std::sin(t), 0);
    }
    Vector3D FirstDerivative(long double t) const override {
        return { Point3D(), Point3D(-radius_x_ * std::sin(t), radius_y_ * std::cos(t), 0) };
    }
    ~Ellipse() {
    }
private:
    long double radius_x_;
    long double radius_y_;
};

class Helix : public Curve3D {
    Helix() = delete;
public:
    Helix(long double radius, long double step)
        : radius_(radius), step_(step)
    {
        if (radius_ <= 0) {
            // Или стандартный параметр
            radius_ = 1;
            // Или выбросить исключение
            throw(std::domain_error("Invalid radii"));
        }
    }
    Point3D TimeFunction(long double t) const override {
        return Point3D(radius_ * std::cos(t), radius_ * std::sin(t), step_ * t);
    }
    Vector3D FirstDerivative(long double t) const override {
        return { Point3D(), Point3D(-radius_ * std::sin(t), radius_ * std::cos(t), step_) };
    }
    ~Helix() {
    }
private:
    long double radius_;
    long double step_;
};


class Object3D {
    Object3D(std::shared_ptr<Curve3D> ptr) {
        object_ptr_ = ptr;
    }

public:
    Object3D& operator=(const Object3D& src) {
        if (this == &src) {
            return *this;
        }
        object_ptr_ = src.object_ptr_;
        return *this;
    }
    Object3D(const Object3D& src) {
        object_ptr_ = src.object_ptr_;
    }
    Point3D TimeFunction(long double t) const {
        return object_ptr_->TimeFunction(t);
    }
    Vector3D FirstDerivative(long double t) const {
        return object_ptr_->FirstDerivative(t);
    }
    static Object3D CreateCircle(long double radius) {
        std::shared_ptr<Curve3D> ptr = std::shared_ptr<Curve3D>(new Circle(radius));
        return Object3D(ptr);
    }
    static Object3D CreateEllipse(long double radius_x, long double radius_y) {
        std::shared_ptr<Curve3D> ptr = std::shared_ptr<Curve3D>(new Ellipse(radius_x, radius_y));
        return Object3D(ptr);
    }
    Curve3D& GetObject() {
        return *object_ptr_;
    }
    const Curve3D& GetObjectC() {
        return *object_ptr_;
    }
    bool IsCircle() {
        return typeid(*object_ptr_) == typeid(Circle);
    }
    Circle* GetCirclePtr() {
        return dynamic_cast<Circle*>((object_ptr_.get()));
    }
private:
    std::shared_ptr<Curve3D> object_ptr_;
};

bool operator < (Object3D& lh, Object3D& rh) {
    if (typeid(lh.GetObject()) == typeid(rh.GetObject()) && typeid(lh.GetObject()) == typeid(Circle)) {
        return *dynamic_cast<Circle*>(&(lh.GetObject())) < *dynamic_cast<Circle*>(&(rh.GetObject()));
    }
    return typeid(lh.GetObject()).name() < typeid(rh.GetObject()).name();
}