#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>


import helios.math;

namespace math = helios::math;

struct test_data {
    math::vec3f up;
    math::vec3f eye;
    math::vec3f center;

    glm::vec3 glm_up;
    glm::vec3 glm_eye;
    glm::vec3 glm_center;
};

static test_data setup() {
    // Deterministic, well-conditioned vectors avoid flaky precision artifacts.
    constexpr float x1 = 0.0F;
    constexpr float y1 = 1.0F;
    constexpr float z1 = 0.0F;
    constexpr float x2 = 1.0F;
    constexpr float y2 = 2.0F;
    constexpr float z2 = 3.0F;
    constexpr float x3 = 0.0F;
    constexpr float y3 = 0.0F;
    constexpr float z3 = 0.0F;

    return test_data{
        .up = math::vec3f{x1, y1, z1},
        .eye = math::vec3f{x2, y2, z2},
        .center = math::vec3f{x3, y3, z3},

        .glm_up = glm::vec3{x1, y1, z1},
        .glm_eye = glm::vec3{x2, y2, z2},
        .glm_center = glm::vec3{x3, y3, z3},
    };
}


TEST(CameraTest, lookAt) {

    // init data
    test_data data = setup();

    math::vec3f up = data.up;
    math::vec3f eye = data.eye;
    math::vec3f center = data.center;

    glm::vec3 glm_up = data.glm_up;
    glm::vec3 glm_eye = data.glm_eye;
    glm::vec3 glm_center = data.glm_center;

    // apply rotations
    const math::mat4 lookAt = math::lookAt(eye, center, up);

    const glm::mat4 glm_lookAt = glm::lookAt(glm_eye, glm_center, glm_up);

    const float* ptr = math::value_ptr(lookAt);
    const float* glm_ptr = glm::value_ptr(glm_lookAt);

    for (int i = 0; i < 16; i++) {
        EXPECT_NEAR(ptr[i], glm_ptr[i], 1e-2f);
    }
}


