# Lexical Analyzer

Bu proje, C dili ile yazılmış temel bir leksik analizör (lexical analyzer) uygulamasıdır. Program, verilen bir kaynak dosyadaki (ör: code.sta) anahtar kelimeleri, tanımlayıcıları, sabitleri, operatörleri ve diğer tokenları analiz ederek ayrıştırır ve sonuçları bir çıktı dosyasına (code.lex) yazar.

## Dosyalar

- **proje_1.c** : Leksik analizörün ana kaynağı.
- **proje_2.c** : Alternatif veya ek analizör kodu.
- **code.sta** : Analiz edilecek örnek giriş dosyası.
- **code.lex** : Analiz sonucu oluşturulan çıktı dosyası.

## Derleme

Windows için GCC ile derlemek için:
```sh
gcc proje_1.c -o proje_1.exe
gcc proje_2.c -o proje_2.exe
```

## Çalıştırma

```sh
./proje_1.exe
# veya
./proje_2.exe
```

## Özellikler

- Tanımlayıcı, anahtar kelime, sabit, operatör, parantez ve string sabitlerini tanır.
- Hatalı parantez kullanımlarını tespit eder.
- Yorum satırlarını atlar.
- Hatalı karakterlerde kullanıcıyı uyarır.

## Katkı

Katkıda bulunmak için lütfen bir fork oluşturun ve pull request gönderin.