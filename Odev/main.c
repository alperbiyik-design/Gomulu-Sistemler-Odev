#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// Desteklenen Veri Tipleri
typedef enum {
    DTYPE_FLOAT32,
    DTYPE_FLOAT16, // Simule edilecek (uint16_t container olarak)
    DTYPE_INT8
} DataType;

// Polimorfik Tensör Yapısı (Bellek Dostu)
typedef struct {
    int rows;
    int cols;
    DataType type;
    // Union: Aynı bellek alanını paylaşan pointerlar
    union {
        float* f32;
        uint16_t* f16; 
        int8_t* i8;
        void* raw; // Genel erişim için
    } data;
} Tensor;

// Tensör Oluşturma Fonksiyonu (Memory Allocation)
Tensor* create_tensor(int rows, int cols, DataType type) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->rows = rows;
    t->cols = cols;
    t->type = type;

    int num_elements = rows * cols;

    // Tipe göre bellek ayırma
    if (type == DTYPE_FLOAT32) {
        t->data.f32 = (float*)malloc(num_elements * sizeof(float));
    } else if (type == DTYPE_FLOAT16) {
        t->data.f16 = (uint16_t*)malloc(num_elements * sizeof(uint16_t));
    } else if (type == DTYPE_INT8) {
        t->data.i8 = (int8_t*)malloc(num_elements * sizeof(int8_t));
    }
    
    return t;
}

// Tensörü Ekrana Yazdırma (Videoda Debugger ile göstereceğin yer)
void print_tensor(Tensor* t) {
    printf("\n--- Tensor (Type: %d, Size: %dx%d) ---\n", t->type, t->rows, t->cols);
    for (int i = 0; i < t->rows; i++) {
        for (int j = 0; j < t->cols; j++) {
            int index = i * t->cols + j;
            if (t->type == DTYPE_FLOAT32) {
                printf("%.4f\t", t->data.f32[index]);
            } else if (t->type == DTYPE_INT8) {
                printf("%d\t", t->data.i8[index]);
            }
        }
        printf("\n");
    }
}

// Basit Quantization (Float32 -> Int8)
// Formül: int_val = clip(round(real_val / scale))
Tensor* quantize_tensor(Tensor* input) {
    if (input->type != DTYPE_FLOAT32) {
        printf("Hata: Sadece Float32 quantize edilebilir!\n");
        return NULL;
    }

    // 1. Mutlak maksimum değeri bul (Scaling Factor için)
    float max_val = 0.0f;
    int num_elements = input->rows * input->cols;
    for(int i=0; i<num_elements; i++) {
        if(fabs(input->data.f32[i]) > max_val) max_val = fabs(input->data.f32[i]);
    }

    // Basit simetrik quantization scale
    float scale = max_val / 127.0f;
    printf("\n[Quantization Info] Max Val: %.4f, Scale: %.4f\n", max_val, scale);

    // 2. Yeni Int8 Tensör oluştur
    Tensor* q_tensor = create_tensor(input->rows, input->cols, DTYPE_INT8);

    // 3. Dönüşümü uygula
    for(int i=0; i<num_elements; i++) {
        float scaled = input->data.f32[i] / scale;
        int8_t quantized = (int8_t)round(scaled);
        q_tensor->data.i8[i] = quantized;
    }

    return q_tensor;
}

// Örnek Uygulama
int main() {
    printf("TinyML Tensor Projesi Basliyor...\n");

    // 1. Float Tensör Oluştur
    Tensor* t_float = create_tensor(2, 3, DTYPE_FLOAT32);
    
    // Veri doldur (Dummy Data)
    float sample_data[] = {-0.5, 1.2, 0.0, 3.5, -2.1, 0.8};
    for(int i=0; i<6; i++) t_float->data.f32[i] = sample_data[i];

    print_tensor(t_float);

    // 2. Quantization İşlemi (32-bit -> 8-bit sıkıştırma)
    Tensor* t_int8 = quantize_tensor(t_float);

    print_tensor(t_int8);

    // Temizlik
    free(t_float->data.raw); free(t_float);
    free(t_int8->data.raw); free(t_int8);

    return 0;
}