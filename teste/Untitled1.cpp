#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <locale.h>

#define dir_rental "rental.dat"
#define dir_loan "loan.dat"

typedef struct {
    int day, month, year;
} type_date;

typedef struct {
    int rental_id;
    char rental_responsible[50];
    float rental_value;
    type_date rental_date;
    char deleted; // 'F' para falso, 'T' para verdadeiro
} type_rental;

typedef struct {
    int loan_id;
    char loan_client[50];
    float loan_value;
    type_date loan_date;
    char deleted; // 'F' para falso, 'T' para verdadeiro
} type_loan;

void draw_frame(int x1, int y1, int x2, int y2) {
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y1);
        printf("%c", i == x1 ? 201 : (i == x2 ? 187 : 205)); // Top border
        gotoxy(i, y2);
        printf("%c", i == x1 ? 200 : (i == x2 ? 188 : 205)); // Bottom border
    }
    for (int i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i);
        printf("%c", 186); // Left border
        gotoxy(x2, i);
        printf("%c", 186); // Right border
    }
}

type_date read_date() {
    type_date date;
    do {
        gotoxy(6, wherey());
        printf("Digite a data (dd mm aaaa): ");
        scanf("%d %d %d", &date.day, &date.month, &date.year);

        if (date.day < 1 || date.day > 31 || date.month < 1 || date.month > 12 || date.year < 1900) {
            gotoxy(6, wherey() + 1);
            textcolor(12); // Vermelho
            printf("Data inválida! Tente novamente.\n");
            textcolor(7); // Branco
        }
    } while (date.day < 1 || date.day > 31 || date.month < 1 || date.month > 12 || date.year < 1900);
    return date;
}

int find_rental_by_id(FILE *file, int id) {
    type_rental rental;
    rewind(file);
    while (fread(&rental, sizeof(type_rental), 1, file) == 1) {
        if (rental.rental_id == id && rental.deleted == 'F') {
            return 1; // Encontrado
        }
    }
    return -1; // Não encontrado
}

int find_loan_by_id(FILE *file, int id) {
    type_loan loan;
    rewind(file);
    while (fread(&loan, sizeof(type_loan), 1, file) == 1) {
        if (loan.loan_id == id && loan.deleted == 'F') {
            return 1; // Encontrado
        }
    }
    return -1; // Não encontrado
}

void clear_screen() {
    gotoxy(6, wherey() + 2);
    textcolor(7); // Branco
    printf("Pressione qualquer tecla para continuar...");
    getch();
}

void create_rental() {
    FILE *pointer_rental = fopen(dir_rental, "rb+");
    if (pointer_rental == NULL) {
        pointer_rental = fopen(dir_rental, "wb+");
    }
    type_rental rental;

    clrscr();
    draw_frame(3, 1, 76, 24); // Moldura ao redor da tela
    gotoxy(30, 2);
    textcolor(11); // Azul claro
    printf("Cadastro de Aluguéis");

    gotoxy(6, 4);
    textcolor(7); // Branco
    printf("Digite o ID do aluguel [0 para sair]: ");
    scanf("%d", &rental.rental_id);

    while (rental.rental_id != 0) {
        if (find_rental_by_id(pointer_rental, rental.rental_id) == -1) {
            gotoxy(6, wherey() + 2);
            printf("Digite o nome do responsável pelo aluguel: ");
            scanf(" %[^\n]", rental.rental_responsible);

            gotoxy(6, wherey() + 2);
            printf("Digite o valor do aluguel: ");
            scanf("%f", &rental.rental_value);

            gotoxy(6, wherey() + 2);
            rental.rental_date = read_date();

            rental.deleted = 'F';
            fseek(pointer_rental, 0, SEEK_END);
            fwrite(&rental, sizeof(type_rental), 1, pointer_rental);

            gotoxy(6, wherey() + 2);
            textcolor(10); // Verde para sucesso
            printf("Aluguel cadastrado com sucesso!");

            clear_screen();
        } else {
            gotoxy(6, wherey() + 2);
            textcolor(12); // Vermelho para erro
            printf("ID do aluguel já existente!");

            clear_screen();
        }

        clrscr();
        draw_frame(3, 1, 76, 24);
        gotoxy(30, 2);
        textcolor(11); // Azul claro
        printf("Cadastro de Aluguéis");

        gotoxy(6, 4);
        textcolor(7); // Branco
        printf("Digite o ID do aluguel [0 para sair]: ");
        scanf("%d", &rental.rental_id);
    }

    fclose(pointer_rental);
}

void main_menu() {
    int option;

    do {
        clrscr();
        draw_frame(3, 1, 76, 15); // Moldura ao redor do menu
        gotoxy(30, 2);
        textcolor(11); // Azul claro
        printf("Sistema de Gerenciamento");

        gotoxy(6, 4);
        textcolor(7); // Branco
        printf("1 - Cadastrar Aluguel");
        gotoxy(6, 5);
        printf("2 - Cadastrar Empréstimo");
        gotoxy(6, 6);
        printf("0 - Sair");

        gotoxy(6, 8);
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                create_rental();
                break;
            case 2:
                create_rental();
                break;
            case 0:
                clrscr();
                draw_frame(3, 1, 76, 5);
                gotoxy(30, 2);
                textcolor(10); // Verde
                printf("Saindo do sistema...");
                break;
            default:
                gotoxy(6, 10);
                textcolor(12); // Vermelho
                printf("Opção inválida!");
                clear_screen();
        }
    } while (option != 0);
}

int main() {
    main_menu();
    return 0;
}

