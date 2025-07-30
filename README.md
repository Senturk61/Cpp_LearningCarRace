# C++ ile Kendi Kendine Öğrenen Araba Yarışı

Bu proje, C++ ve SFML kütüphanesi kullanılarak sıfırdan geliştirilmiş bir yapay zeka simülasyonudur. Genetik algoritma ve yapay sinir ağları kullanılarak, arabaların bir yarış pistinde kendi kendine sürmeyi "öğrenmesi" hedeflenmiştir.

<img width="1287" height="815" alt="image" src="https://github.com/user-attachments/assets/8ef42ba8-dc0f-44d5-8c3e-b2ff0b1aee15" />


<img width="1321" height="814" alt="image" src="https://github.com/user-attachments/assets/143e0f82-aa53-4a15-a47d-49f45651053e" />

---

## 🚀 Proje Hakkında

Simülasyon, yüzlerce arabadan oluşan bir popülasyonla başlar. Her arabanın, çevresini algılayan sensörleri ve bu verilere göre karar veren rastgele bir "beyni" (yapay sinir ağı) vardır.

Genetik algoritma sayesinde, her nesilde en başarılı (pisti en uzun süre hatasız tamamlayan) arabalar seçilir. Bu başarılı arabaların beyinleri çaprazlanarak ve küçük mutasyonlara uğratılarak yeni, daha "akıllı" bir nesil oluşturulur. Yüzlerce nesil sonra, arabaların pisti başarıyla tamamlayabildiği gözlemlenir.

### ✨ Temel Özellikler

* **Genetik Algoritma:** Öğrenme sürecinin temelini oluşturur (Seçilim, Çaprazlama, Mutasyon).
* **Yapay Sinir Ağı:** Her arabanın karar mekanizmasıdır. C++ ile sıfırdan kodlanmıştır.
* **Sensörler (Ray-Casting):** Arabaların çevresini "görmesini" sağlar.
* **SFML 3.0 Kütüphanesi:** Tüm grafiksel işlemler, pencere yönetimi ve kullanıcı girdileri için kullanılmıştır.
* **OOP Tasarımı:** Kod, `Car` ve `NeuralNetwork` gibi sınıflarla daha modüler ve anlaşılır hale getirilmiştir.

---

## 🛠️ Kullanılan Teknolojiler

* **Dil:** C++17
* **Grafik Kütüphanesi:** [SFML 3.0](https://www.sfml-dev.org/)
* **Derleyici:** MinGW (g++)

---

## ⚙️ Kurulum ve Çalıştırma

Projeyi yerel makinenizde çalıştırmak için aşağıdaki adımları izleyin.

1.  **Gerekli Kütüphaneler:**
    * Bir C++ derleyicisi (g++ önerilir).
    * Proje ile uyumlu [SFML 3.0](https://www.sfml-dev.org/download/sfml/3.0.0/) kütüphanesi.

2.  **Derleme:**
    Proje, VS Code için ayarlanmış `tasks.json` dosyası ile derlenebilir. Terminal üzerinden derlemek için aşağıdaki gibi bir komut kullanılabilir (SFML yollarını kendinize göre düzenleyin):

    ```bash
    g++ main.cpp -o app.exe -I"path/to/SFML/include" -L"path/to/SFML/lib" -lsfml-graphics -lsfml-window -lsfml-system
    ```

3.  **Çalıştırma:**
    Derleme sonrası oluşan `.exe` dosyasını, SFML'in `.dll` dosyaları ile aynı klasörde çalıştırın.

---

## 🧠 Nasıl Çalışıyor?

Öğrenme süreci 3 ana bileşenden oluşur:

1.  **Algılama (Sensörler):** Her araba, farklı açılarla etrafına ışınlar göndererek duvarlara olan mesafeyi ölçer. Bu mesafe bilgileri, beynin girdilerini oluşturur.

2.  **Karar Verme (Sinir Ağı):**
    * **Girdi Katmanı:** 5 sensörden gelen mesafe verileri.
    * **Gizli Katman:** Girdileri işleyen ara katman (8 nöron).
    * **Çıktı Katmanı:** Arabanın ne yapması gerektiğini belirten 2 çıktı (hız ve dönüş yönü).

3.  **Evrim (Genetik Algoritma):**
    * Her nesildeki tüm arabalar, bir zaman sınırı içinde veya kaza yapana kadar yarışır.
    * Başarı (fitness), geçilen kontrol noktası sayısına ve hayatta kalma süresine göre ölçülür.
    * Nesil bittiğinde, en başarılı arabalar "ebeveyn" olarak seçilir.
    * Yeni nesil, bu başarılı ebeveynlerin beyinlerinin çaprazlanması ve mutasyona uğratılmasıyla oluşturulur. Bu döngü sürekli tekrarlanır.
