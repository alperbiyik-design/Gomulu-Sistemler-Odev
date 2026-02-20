# TinyML C Tensor Library for Microcontrollers 🚀

Bu proje, bellek kısıtlı gömülü sistemler (Arduino, ESP32) için optimize edilmiş, C tabanlı dinamik bir Tensör kütüphanesidir.

## 🎯 Özellikler
* **Polimorfik Yapı:** Tek bir struct içinde Float32, Float16 ve Int8 desteği.
* **Union Kullanımı:** Bellek verimliliği için `union` ile tip yönetimi.
* **Quantization:** 32-bit kayan noktalı sayıları 8-bit tamsayıya dönüştürme (Post-training quantization simülasyonu).

## 🛠️ Kullanılan Teknolojiler
* **Dil:** C (C99 Standardı)
* **IDE:** VS Code
* **AI Partner:** Gemini 3 Pro (Agentic Coding)

## 📊 Nasıl Çalışır?
Proje, `Tensor` struct'ı üzerinden dinamik bellek yönetimi (malloc) yapar. Quantization fonksiyonu, float verileri analiz ederek `scale` faktörü belirler ve veri kaybını minimize ederek `int8_t` formatına dönüştürür.

## 🎥 Demo Videosu
[Buraya YouTube'a yüklediğin videonun linkini yapıştıracaksın]