#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CRC-8 Algorithm
unsigned char crc8(const unsigned char* data, size_t length) {
    unsigned char crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// CRC-16 Algorithm
unsigned short crc16(const unsigned char* data, size_t length) {
    unsigned short crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// CRC-32 Algorithm
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

// Function to measure time taken by a given function
double measure_time(void (*func)(const unsigned char*, size_t), const unsigned char* data, size_t length) {
    clock_t start = clock();
    func(data, length);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    size_t sizes[] = { 1024, 4096, 8192, 32768, 65536, 1048576 }; // 1KB, 4KB, 8KB, 32KB, 64KB, 1MB
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("CRC Performance Test Results (Time in seconds):\n");
    printf("Size\tCRC-8\tCRC-16\tCRC-32\n");

    for (int i = 0; i < num_sizes; i++) {
        size_t size = sizes[i];
        // 强制类型转换为 unsigned char*
        unsigned char* data = (unsigned char*)malloc(size);
        for (size_t j = 0; j < size; j++) {
            data[j] = rand() % 256;
        }

        double time_crc8 = measure_time((void (*)(const unsigned char*, size_t))crc8, data, size);
        double time_crc16 = measure_time((void (*)(const unsigned char*, size_t))crc16, data, size);
        double time_crc32 = measure_time((void (*)(const unsigned char*, size_t))crc32, data, size);

        printf("%zu\t%f\t%f\t%f\n", size, time_crc8, time_crc16, time_crc32);

        free(data);
    }

    return 0;
}
