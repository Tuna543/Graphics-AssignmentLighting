#ifndef cylinders_h
#define cylinders_h

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Cylinders {

public:
    int numPoints = 10;
    float radius = 1.0;
    /*
    std::vector<std::pair<double, float>> points;

    float* arr = new float[numPoints * 12];

    int* indices = new int[numPoints * 6];
    void generateVertices() {
        for (int i = 0; i < numPoints; ++i) {
            float theta = i * (360.0 / numPoints); // Calculate theta in degrees
            float radians = theta * (3.1416 / 180.0); // Convert to radians

            // Calculating the x and y coordinates of the point
            float x = radius * cos(radians);
            float y = radius * sin(radians);

            points.push_back(std::make_pair(x, y));
        }

        int j = 0;

        for (int i = 0; i < numPoints; ++i) {
            //std::cout << points[i].first << "f" << ", " << "1.0f" << ", " << points[i].second << "f" << " , 1.0f, 0.0f, 0.0f" << std::endl;
            arr[j++] = points[i].first;
            arr[j++] = 1.0;
            arr[j++] = points[i].second;
            arr[j++] = 1.0;
            arr[j++] = 0.0;
            arr[j++] = 0.0;

            //std::cout << points[i].first<<"f" << ", " << "-1.0f" << ", " << points[i].second<<"f"<< "1.0f, 0.0f, 0.0f"<< std::endl;
        }

        for (int i = 0; i < numPoints; ++i) {
            //std::cout << points[i].first<<"f" << ", " << "1.0f"<< ", " << points[i].second<<"f" << "1.0f, 0.0f, 0.0f"<< std::endl;
            //std::cout << points[i].first << "f" << ", " << "-1.0f" << ", " << points[i].second << "f" << " , 1.0f, 0.0f, 0.0f" << std::endl;
            arr[j++] = points[i].first;
            arr[j++] = -1.0;
            arr[j++] = points[i].second;
            arr[j++] = 1.0;
            arr[j++] = 0.0;
            arr[j++] = 0.0;
        }
        int x = 0, in = 0, y = numPoints + 1;
        for (int k = 0; k < numPoints * 6 - 6; ) {
            indices[k++] = x;
            indices[k++] = y;
            indices[k++] = x + 1;

            indices[k++] = y;
            indices[k++] = x;
            indices[k++] = y - 1;
            x++;
            y++;
            in = k;
        }
        indices[in++] = 9;
        indices[in++] = 10;
        indices[in++] = 0;
        indices[in++] = 10;
        indices[in++] = 9;
        indices[in++] = 19;


    }
    */
};

#endif
#pragma once

#pragma once
