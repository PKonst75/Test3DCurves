// curves-3d-console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <vector>

#include "object-3d.h"

const long double PI = 3.141592653589793;
const long double PI4 = PI / 4;



int main()
{

    // Вариант первый
    using Curve3DPtr = std::shared_ptr<Curve3D>;

    std::vector<Curve3DPtr> curves;

    for (int i = 1; i < 5; ++i) {
        Curve3DPtr new_ptr = Curve3DPtr(new Circle(5 - i));
        curves.push_back(new_ptr);
    }

    for (int i = 1; i < 5; ++i) {
        Curve3DPtr new_ptr = Curve3DPtr(new Ellipse(i, 5 - i));
        curves.push_back(new_ptr);
    }

    for (int i = 1; i < 5; ++i) {
        Curve3DPtr new_ptr = Curve3DPtr(new Helix(i, 5 - i));
        curves.push_back(new_ptr);
    }


    std::cout << "Row data in PI / 4 point" << std::endl; // 
    // Печать данных по всем элементам массива
    for (Curve3DPtr curve : curves) {
        std::cout << (curve)->TimeFunction(PI4) << std::endl;
        std::cout << (curve)->FirstDerivative(PI4) << std::endl;
    }

    // Выбор всех Кругов
    std::vector<Curve3DPtr> circles;
    for (Curve3DPtr curve : curves) {
        if (typeid(*curve) == typeid(Circle)) {
            circles.push_back(curve);
        }
    }

   // std::sort(circles.begin(), circles.end(), [](const Curve3DPtr& lh, const Curve3DPtr& rh) { return dynamic_cast<Circle*>(&(*lh))->Radius() < dynamic_cast<Circle*>(&(*rh))->Radius(); });
    std::sort(circles.begin(), circles.end(), [](const Curve3DPtr& lh, const Curve3DPtr& rh) { return *dynamic_cast<Circle*>(&(*lh)) < *dynamic_cast<Circle*>(&(*rh)); });
    int sum = std::accumulate(circles.begin(), circles.end(), 0, [](int prev, const Curve3DPtr& val) {return dynamic_cast<Circle*>(&(*val))->Radius() + prev ;});

    std::cout << "Sort circle vector" << std::endl; // 
    for (Curve3DPtr circle : circles) {
        std::cout << (circle)->TimeFunction(PI4) << std::endl;
    }
    std::cout << "Summ of radii = " << sum << std::endl; // 
   

    std::cout << "End of process 1" << std::endl; // 

    // Второй вариант - может быть более удобен с точки зрения ссоздания и хранения объектов
    // Так как в этом случае вся работа с указателями скрыта от пользователя
    // Объект3Д сам передается не указателем, но он маленький

    std::vector<Object3D> curves_2;
    for (int i = 1; i < 30; ++i) {
        curves_2.push_back(Object3D::CreateCircle(i));
        curves_2.push_back(Object3D::CreateEllipse(4, 4));
    }

    for (Object3D& curve : curves_2) {
        std::cout << curve.TimeFunction(PI4) << std::endl;
        std::cout << curve.FirstDerivative(PI4) << std::endl;
    }

    std::cout << "Sort vector" << std::endl; // 
    std::sort(curves_2.begin(), curves_2.end());
    for (Object3D& curve : curves_2) {
        std::cout << curve.TimeFunction(PI4) << std::endl;
    }

    std::vector<Object3D> curves_3;
    std::vector<Circle*> circle_2;
    for (Object3D& curve : curves_2) {
        if (curve.IsCircle()) {
            curves_3.push_back(curve);
            circle_2.push_back(curve.GetCirclePtr());
        }
    }
    std::cout << "Only circles" << std::endl; // 
    for (Object3D& curve : curves_3) {
        std::cout << curve.TimeFunction(PI4) << std::endl;
    }

    sum = std::accumulate(curves_3.begin(), curves_3.end(), 0, [](int prev, Object3D& val) {return dynamic_cast<Circle*>(&(val.GetObject()))->Radius() + prev; });
    std::cout << "Summ of radii 2 = " << sum << std::endl; // 

    // Еще вариант
    std::sort(circle_2.begin(), circle_2.end());
    sum = std::accumulate(circle_2.begin(), circle_2.end(), 0, [](int prev, Circle* val) {return val->Radius() + prev; });
    std::cout << "Summ of radii 3 = " << sum << std::endl; // 
}
