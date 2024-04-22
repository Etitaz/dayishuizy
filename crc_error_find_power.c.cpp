#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // 确保包含 memcpy
#include <time.h>  // 为了使用 rand

// CRC-32 算法
unsigned int crc32(const unsigned char* data, size_t length) {
    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            }
            else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFF;
}

// 翻转一个字节中的某个位
void flip_bit(unsigned char* data, size_t index, unsigned char bit) {
    data[index] ^= (1 << bit);
}

// 测试CRC错误检测能力
void test_crc32_error_detection(const char* description, const unsigned char* data, size_t length, size_t error_index, unsigned char error_bit) {
    unsigned char* copy = (unsigned char*)malloc(length);  // 添加强制类型转换
    memcpy(copy, data, length);  // 使用 memcpy 复制数据

    flip_bit(copy, error_index, error_bit);  // 介绍错误

    unsigned int original_crc = crc32(data, length);
    unsigned int altered_crc = crc32(copy, length);

    printf("%s - 原始CRC: %08X, 错误CRC: %08X\n", description, original_crc, altered_crc);

    free(copy);
}

int main() {
    unsigned char data[16];
    for (int i = 0; i < 16; i++) {
        data[i] = rand() % 256;  // 使用 rand 生成随机数据
    }

    // 测试单个位翻转
    test_crc32_error_detection("单个位翻转", data, sizeof(data), 5, 3);  // 翻转第6个字节的第4个位

    // 测试多位翻转
    test_crc32_error_detection("多位翻转 - 1", data, sizeof(data), 3, 2);  // 翻转第4个字节的第3个位
    test_crc32_error_detection("多位翻转 - 2", data, sizeof(data), 7, 1);  // 翻转第8个字节的第2个位

    return 0;
}
