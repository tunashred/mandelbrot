#include <mandelbrot.h>
#include <color_mapping.h>

#include <gtest/gtest.h>

// TODO: Reuse same image for overwritting + change its save location + sanitizer integration
/* Later TODO:
 * Maybe organize the tests better with TEST_F, INSTANTIATE_TEST_SUITE_P
 * Maybe create vectors with image configs
*/
class Environment : public ::testing::Environment {
    public:
    ~Environment() override {}

    void SetUp() override { }

    void TearDown() override {}
};

TEST(image_generation, image_1k) {
    float scale = 1;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, mandelbrot_2k) {
    float scale = 2;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, mandelbrot_4k) {
    float scale = 4;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1920 * scale, -0.43, -0.1, 1, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, galaxy_1k) {
    float scale = 1;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, galaxy_2k) {
    float scale = 2;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, galaxy_4k) {
    float scale = 4;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, stuff_1k) {
    float scale = 1;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, stuff_2k) {
    float scale = 2;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, stuff_4k) {
    float scale = 4;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.7, -0.26, 0.01, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, seastar_1k) {
    float scale = 1;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, seastar_2k) {
    float scale = 2;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, seastar_4k) {
    float scale = 4;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, shells_1k) {
    float scale = 1;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, shells_2k) {
    float scale = 2;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}

TEST(image_generation, shells_4k) {
    float scale = 4;
    mandelbrot_around_center("test_image.ppm", 1080 * scale, 1080 * 1.2 * scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1, sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2);
}
