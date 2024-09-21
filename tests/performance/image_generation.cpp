#include "gtest/gtest.h"
#include <mandelbrot.h>
#include <color_mapping.h>

// Global configurations
int worker_thread_count = 6;

class ImageGeneration : public ::testing::TestWithParam<float> {
protected:
    bool is_portrait;

    int (*color_map_red)(int, int) = sin_x_la_4;
    int (*color_map_green)(int, int) = unu_minus_unu_pe_x;
    int (*color_map_blue)(int, int) = x_patrat_0_1_to_0_2;

    void SetUp() override {
        is_portrait = false; // Default is landscape
    }

    void TearDown() override {}

    void generate_image(const std::string& name, float scale, float x_center, float y_center,
                        float zoom, int num_iters, int brightness, int rotate_degrees) {
        if (is_portrait) {
            mandelbrot_around_center(name.c_str(), 1080 * scale, 1080 * 1.2 * scale, x_center, y_center,
                                     zoom, num_iters, rotate_degrees, brightness, color_map_red,
                                     color_map_green, color_map_blue, mandelbrot_quadratic, worker_thread_count);
        } else {
            mandelbrot_around_center(name.c_str(), 1080 * scale, 1920 * scale, x_center, y_center,
                                     zoom, num_iters, rotate_degrees, brightness, color_map_red,
                                     color_map_green, color_map_blue, mandelbrot_quadratic, worker_thread_count);
        }
    }
};

using ImageGenerationExtra = ImageGeneration;

TEST_P(ImageGeneration, Mandelbrot) {
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.43, -0.1, 1, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Mandelbrot) {
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.43, -0.1, 1, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Galaxy) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.700025 + 0.000000007, -0.26849991525, 
                    0.0000000035, 1500, 0, 0);
    RecordProperty("scale", scale);
}

// Galaxy Extra Tests
TEST_P(ImageGenerationExtra, Galaxy) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.700025 + 0.000000007, -0.26849991525, 
                    0.0000000035, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Stuff) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.7, -0.26, 0.01, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Stuff) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.7, -0.26, 0.01, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Seastar) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Seastar) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Shells) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Shells) {
    is_portrait = true;
    float scale = GetParam();
    generate_image("test_image.ppm", scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1);
    RecordProperty("scale", scale);
}

INSTANTIATE_TEST_SUITE_P(
    mandatory_images,
    ImageGeneration,
    ::testing::Values(1, 2, 4)
);

INSTANTIATE_TEST_SUITE_P(
    extra_images,
    ImageGenerationExtra,
    ::testing::Values(1.5, 2.5, 3, 3.5)
);

