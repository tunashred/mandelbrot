#include "gtest/gtest.h"
#include <mandelbrot.h>
#include <color_mapping.h>

class ImageGeneration : public ::testing::TestWithParam<std::tuple<int, float>> {
protected:
    color_palette palette;
    image_info* img_info;
    tpool_t* pool;
    worker_task_info* tasks;
    int numThreads;
    bool is_portrait;

    uint8_t (*color_map_red)(int, int) = sin_x_la_4;
    uint8_t (*color_map_green)(int, int) = unu_minus_unu_pe_x;
    uint8_t (*color_map_blue)(int, int) = x_patrat_0_1_to_0_2;

    void SetUp() override {
        int threads;
        float scale;
        std::tie(threads, scale) = this->GetParam();

        is_portrait = false; // Default is landscape

        generate_color_palette(
            &this->palette, NULL, 1,
            sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2
        );
        
        pool = tpool_init(threads);
    }

    void TearDown() override {
        tpool_destroy(pool);
        free(tasks);
        free(img_info->buffer);
        free(img_info);
    }

    void generate_image(float scale, float x_center, float y_center,
                        float zoom, int num_iters, int brightness, int rotate_degrees) {
        if (is_portrait) {
            img_info = mandelbrot_around_center(1080 * scale, 1080 * 1.2 * scale, x_center, y_center,
                                     zoom, num_iters, rotate_degrees, mandelbrot_quadratic);
        } else {
            img_info = mandelbrot_around_center(1080 * scale, 1920 * scale, x_center, y_center,
                                     zoom, num_iters, rotate_degrees, mandelbrot_quadratic);
        }
        tasks = start_workers(pool, img_info, &palette);

        tpool_wait(pool);
    }

    static std::string getTestCaseName(const ::testing::TestParamInfo<std::tuple<int, float>>& info) {
        int threads;
        float scale;
    
        return "threads_" + std::to_string(threads);
    }
};

using ImageGenerationExtra = ImageGeneration;

TEST_P(ImageGeneration, Mandelbrot) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    generate_image(scale, -0.43, -0.1, 1, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Mandelbrot) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    generate_image(scale, -0.43, -0.1, 1, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Galaxy) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0);
    RecordProperty("scale", scale);
}

// Galaxy Extra Tests
TEST_P(ImageGenerationExtra, Galaxy) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.700025 + 0.000000007, -0.26849991525, 0.0000000035, 1500, 0, 0);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Stuff) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.7, -0.26, 0.01, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Stuff) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.7, -0.26, 0.01, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Seastar) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Seastar) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.72413, 0.28644, 0.0004, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGeneration, Shells) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1);
    RecordProperty("scale", scale);
}

TEST_P(ImageGenerationExtra, Shells) {
    int threads;
    float scale;
    std::tie(threads, scale) = this->GetParam();

    is_portrait = true;
    generate_image(scale, -0.75 + 0.00005, -0.02, 0.00025, 1500, 0, 1);
    RecordProperty("scale", scale);
}

std::vector<int> numThreads = {6, 8, 12};

std::vector<float> mandatory_scale = {1, 2, 4};

std::vector<float> extra_scale = {1.5, 2.5, 3, 3.5};

INSTANTIATE_TEST_SUITE_P(
    mandatory_images,
    ImageGeneration,
    ::testing::Combine(
        ::testing::ValuesIn(numThreads),
        ::testing::ValuesIn(mandatory_scale)
    )
);

INSTANTIATE_TEST_SUITE_P(
    extra_images,
    ImageGenerationExtra,
    ::testing::Combine(
        ::testing::ValuesIn(numThreads),
        ::testing::ValuesIn(extra_scale)
    )
);

