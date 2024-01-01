#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kiþi bilgilerini tutan yapý
typedef struct {
    char ad[50];
    char soyad[50];
    char cinsiyet; // 'E' veya 'K'
    int yas;
    double boy;
    double kilo;
} Kisi;

// Aðaç düðümü yapýsý
typedef struct Dugum {
    Kisi kisi;
    struct Dugum* sol;
    struct Dugum* sag;
} Dugum;

// Stack veri yapýsý
typedef struct {
    Dugum** elemanlar;
    int ust;
    int kapasite;
} Stack;

// Diyet listesi yapýsý
typedef struct {
    char* kahvalti;
    char* ogle;
    char* aksam;
    char* atistirmaliklar;
} DiyetListesi;

// Stack fonksiyonlarý
void stackOlustur(Stack* stack, int kapasite) {
    stack->elemanlar = (Dugum**)malloc(sizeof(Dugum*) * kapasite);
    stack->ust = -1;
    stack->kapasite = kapasite;
}

void stackEkle(Stack* stack, Dugum* eleman) {
    if (stack->ust == stack->kapasite - 1) {
        printf("Stack dolu, eleman eklenemiyor.\n");
        return;
    }
    stack->elemanlar[++stack->ust] = eleman;
}

Dugum* stackCikar(Stack* stack) {
    if (stack->ust == -1) {
        printf("Stack bos, eleman cikarilamaz.\n");
        return NULL;
    }
    return stack->elemanlar[stack->ust--];
}

// Aðaç fonksiyonlarý
Dugum* dugumOlustur(Kisi kisi) {
    Dugum* yeniDugum = (Dugum*)malloc(sizeof(Dugum));
    yeniDugum->kisi = kisi;
    yeniDugum->sol = NULL;
    yeniDugum->sag = NULL;
    return yeniDugum;
}

Dugum* agacaEkle(Dugum* agac, Kisi kisi) {
    if (agac == NULL) {
        return dugumOlustur(kisi);
    }

    if (strcmp(kisi.ad, agac->kisi.ad) < 0) {
        agac->sol = agacaEkle(agac->sol, kisi);
    } else {
        agac->sag = agacaEkle(agac->sag, kisi);
    }

    return agac;
}

void agaciGoster(Dugum* agac) {
    if (agac != NULL) {
        agaciGoster(agac->sol);
        printf("%s %s\n", agac->kisi.ad, agac->kisi.soyad);
        agaciGoster(agac->sag);
    }
}

// Merge sýralama algoritmasý
void merge(Kisi dizi[], int sol, int orta, int sag) {
    int n1 = orta - sol + 1;
    int n2 = sag - orta;

    Kisi solDizi[n1], sagDizi[n2];

    for (int i = 0; i < n1; i++) {
        solDizi[i] = dizi[sol + i];
    }
    for (int j = 0; j < n2; j++) {
        sagDizi[j] = dizi[orta + 1 + j];
    }

    int i = 0, j = 0, k = sol;
    while (i < n1 && j < n2) {
        if (solDizi[i].kilo <= sagDizi[j].kilo) {
            dizi[k++] = solDizi[i++];
        } else {
            dizi[k++] = sagDizi[j++];
        }
    }

    while (i < n1) {
        dizi[k++] = solDizi[i++];
    }

    while (j < n2) {
        dizi[k++] = sagDizi[j++];
    }
}

void mergeSort(Kisi dizi[], int sol, int sag) {
    if (sol < sag) {
        int orta = sol + (sag - sol) / 2;

        mergeSort(dizi, sol, orta);
        mergeSort(dizi, orta + 1, sag);

        merge(dizi, sol, orta, sag);
    }
}

// Vücut kitle indeksi hesaplama
double vucutKitleIndeksiHesapla(double boy, double kilo) {
    return kilo / ((boy / 100) * (boy / 100));
}

// Diyet listesi oluþturma
DiyetListesi diyetListesiOlustur(Kisi kisi) {
    DiyetListesi diyetListe;

    // Vücut kitle indeksine göre diyet listesi seçimi
    if (vucutKitleIndeksiHesapla(kisi.boy, kisi.kilo) < 25.0) {
        // Birinci diyet listesi
        diyetListe.kahvalti = "Kahvalti:\n1 su bardagi sut\n1 porsiyon meyve\n1 dilim yulaf ezmesi\nOgle Yemegi:\n1 tabak sebze corbasi\n1 porsiyon izgara balik\n1 porsiyon sebze\nAksam Yemegi:\n1 tabak salata\n1 porsiyon tavuk veya hindi\n1 porsiyon sebze\nAtistirmaliklar:\nMeyve\nKuruyemis\nYogurt";

    } else {
        // Ýkinci diyet listesi
        diyetListe.kahvalti = "Kahvalti:\n2 yumurta\n1 dilim tam bugday ekmegi\n1 porsiyon meyve\nOgle Yemegi:\n1 porsiyon izgara et veya tavuk\n1 porsiyon sebze\nAksam Yemegi:\n1 porsiyon balik\n1 porsiyon sebze\nAtistirmaliklar:\nKuruyemis\nTam tahillar\nProtein barlari";
    }

    // Diðer öðünler için ayný diyet listesini kullan
    diyetListe.ogle = diyetListe.kahvalti;
    diyetListe.aksam = diyetListe.kahvalti;
    diyetListe.atistirmaliklar = diyetListe.kahvalti;

    return diyetListe;
}

int main() {
    Dugum* agac = NULL;
    Stack stack;
    stackOlustur(&stack, 100);

    int secim;
    do {
        printf("\n1. Kisi Ekle\n2. Kisi Sil\n3. Kisi Listele\n4. VKI Hesapla ve Diyet Listesini Goster\n0. Cikis\n");
        printf("Seciminizi yapin: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1: {
                Kisi yeniKisi;
                printf("Ad: ");
                scanf("%s", yeniKisi.ad);
                printf("Soyad: ");
                scanf("%s", yeniKisi.soyad);
                printf("Cinsiyet (E/K): ");
                scanf(" %c", &yeniKisi.cinsiyet);
                printf("Yas: ");
                scanf("%d", &yeniKisi.yas);
                printf("Boy (cm): ");
                scanf("%lf", &yeniKisi.boy);
                printf("Kilo (kg): ");
                scanf("%lf", &yeniKisi.kilo);

                agac = agacaEkle(agac, yeniKisi);
                stackEkle(&stack, agac);
                break;
            }
            case 2: {
                if (stack.ust == -1) {
                    printf("Agac bos, silinecek eleman yok.\n");
                } else {
                    Dugum* silinecekDugum = stackCikar(&stack);
                    printf("%s %s isimli kisi silindi.\n", silinecekDugum->kisi.ad, silinecekDugum->kisi.soyad);
                    free(silinecekDugum);
                }
                break;
            }
            case 3: {
                printf("Kisiler:\n");
                agaciGoster(agac);
                break;
            }
            case 4: {
                char arananAd[50];
                printf("Vucut kitle indeksi hesaplanacak kisinin adini girin: ");
                scanf("%s", arananAd);

                // Ýlgili kiþiyi aðaçta ara
                Dugum* temp = agac;
                while (temp != NULL && strcmp(temp->kisi.ad, arananAd) != 0) {
                    if (strcmp(arananAd, temp->kisi.ad) < 0) {
                        temp = temp->sol;
                    } else {
                        temp = temp->sag;
                    }
                }

                if (temp == NULL) {
                    printf("%s isimli kisi bulunamadi.\n", arananAd);
                } else {
                    double vki = vucutKitleIndeksiHesapla(temp->kisi.boy, temp->kisi.kilo);
                    printf("%s %s isimli kisinin vucut kitle indeksi: %.2lf\n", temp->kisi.ad, temp->kisi.soyad, vki);

                    // Diyet listesini oluþtur ve göster
                    DiyetListesi diyetListe = diyetListesiOlustur(temp->kisi);
                    printf("\nDiyet Listesi:\n%s\n", diyetListe.kahvalti);
                }
                break;
            }
            case 0: {
                // Çýkýþ yap
                break;
            }
            default:
                printf("Gecersiz secim. Tekrar deneyin.\n");
        }

    } while (secim != 0);




    return 0;
}
