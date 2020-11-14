#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char ciphertext[128];

void xorcipher (char plaintext[], char key[]) {
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        ciphertext[i] = plaintext[i] ^ key[i];
    }

}

int main() {

    char key[128];
    char plaintext[128];

    printf("Inserire stringa da decifrare: ");
    fgets(plaintext, 128, stdin); // Previene buffer overload leggendo solo 128 bytes

    int menu = 0;
    do {

        printf("\nSi prega di segliere digitando\n1) Cifratura con chiave\n2) Cifratura chiave randomica\n0) Uscita\n");

        int scelta = 0;
        scanf("%d", &scelta);
        switch (scelta) {
            case 1:
                // Inserimento chiave manuale
                while(getchar()!='\n');                             // Pulizia buffer per leggere una nuova stringa
                printf("\nInserisci la chiave: ");
                fgets(key, 128, stdin);

                if (strlen(key)<strlen(plaintext)) {
                    printf("Errore, lunghezza chiave minore!");
                    break;
                }

                xorcipher(plaintext, key);
                printf("\nStringa cifrata --- \n%s", ciphertext);
                

                xorcipher(ciphertext, key);
                printf("\nStringa decifrata --- \n%s", ciphertext);
                
          
       
                break;

            case 2:
                // Generazione chiave randomica
                for (int i = 0; i < strlen(plaintext); i++) {
                    key[i] = 31 + rand()%97;
                }

                
                xorcipher(plaintext, key);
                printf("\nStringa cifrata --- \n%s", ciphertext);
                
              

                xorcipher(ciphertext, key);
                printf("\nStringa decifrata --- \n%s", ciphertext);

         
              
                break;

            case 0:
                menu = 1;
                break;

            default:
                printf("Scelta non possibile, riprovare!");
                break;
        }

    } while (menu == 0);


    return 0;
}