#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

#define dir_vehicle "vehicle.dat"
#define dir_rental "rental.dat"
#define dir_loan "loan.dat"
#define dir_client "client.dat"
#define dir_relation "relation_vehicle_rental.dat"






typedef struct Dates
{
	int day;
	int month;
	int year;
}type_date;


typedef struct Vehicles
{
	int fabrication_year;
	char vehicle_plate[8]; // primary key
	char vehicle_model[128];
	char deleted;

}type_vehicle;

typedef struct Rental
{
	int rental_id; //primary key
	char rental_name[128];
	char rental_city[128];
	char deleted;

}type_rental;

typedef struct Loans{
	int loan_id;
	char vehicle_id[8]; //foreing key
	char client_id[12]; //foreing key
	type_date loan_date;
	type_date return_date;
	type_date returned_date;
	char deleted;
}type_loan;

typedef struct relation_vehicle_rental
{
	int rental_id;
	char vehicle_id[8];
	char deleted;

}type_relation_vehicle_rental;


typedef struct Clients{
	char client_cpf[12]; // cpf primary_Key
	char client_name[128];
	char client_tel[14];
	char client_city[128];
	char client_adress[128];
	char client_state[128];
	char deleted;

}type_client;

void clear_screen()
{
	printf("Digite enter para continuar\n");
	getchar();
	system("cls");
}

int generate_rental_id(FILE *pointer_rental)
{
	fseek(pointer_rental, 0, SEEK_END);
	return ftell(pointer_rental)/sizeof(type_rental) + 1;
};

int generate_loan_id(FILE *pointer_loan)
{
	fseek(pointer_loan, sizeof(type_loan), SEEK_END);
	return ftell(pointer_loan)/sizeof(type_loan) + 1;
}

int validate_client_id(char client_cpf[12])
{
	int i;
	int first_digit_sum = 0, second_digit_sum = 0, second_digit;
	char correct_cpf[12];
	strcpy(correct_cpf, client_cpf);
	
	for(i = 1; i < 10; i++)
	{
		first_digit_sum += (client_cpf[i-1]-48) * i;
	}
	correct_cpf[9] = (first_digit_sum % 11)+48;
	for(i = 0; i < 10; i++)
	{
		second_digit_sum += (correct_cpf[i]-48) * i;
	}
	
	second_digit = second_digit_sum % 11;
	
	if(second_digit == 10)
		second_digit = 0;
		
	correct_cpf[10] = second_digit+48;
	
	if(client_cpf[9] == correct_cpf[9] && client_cpf[10] == correct_cpf[10])
		return 1;
	return 0;
}


int get_loan_id(FILE *pointer_loan, int pos)
{
	type_loan loan;
	fseek(pointer_loan, pos, SEEK_SET);
	fread(&loan, sizeof(type_loan), 1, pointer_loan);
	return loan.loan_id;
}


int find_vehicle_by_id(FILE *pointer_vehicle, char vehicle_id[8])
{
	type_vehicle vehicle;
	rewind(pointer_vehicle);
	int pos;

	fread(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);
	while(!feof(pointer_vehicle) && !(strcmp(vehicle.vehicle_plate, vehicle_id) == 0 && vehicle.deleted == 'F'))
		fread(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);

	if(strcmp(vehicle.vehicle_plate, vehicle_id) == 0 && vehicle.deleted == 'F')
	{
		pos = ftell(pointer_vehicle) - sizeof(type_vehicle);
		return pos;
	}
	return -1;

}

int find_client_by_id(FILE *pointer_client, char client_id[12])
{
	type_client client;
	int pos;
	rewind(pointer_client);

	fread(&client, sizeof(type_client), 1, pointer_client);
	while(!feof(pointer_client) && !(strcmp(client.client_cpf, client_id) == 0 && client.deleted == 'F'))
		fread(&client, sizeof(type_client), 1, pointer_client);

	if(strcmp(client.client_cpf, client_id) == 0 && client.deleted == 'F')
	{
		pos = ftell(pointer_client) - sizeof(type_client);
		return pos;
	}
	return -1;

}


int find_rental_by_id(FILE *pointer_rental, int rental_id)
{
	type_rental rental;
	int pos;
	
	rewind(pointer_rental);
	fread(&rental, sizeof(type_rental), 1, pointer_rental);
	while(!feof(pointer_rental) && !(rental_id == rental.rental_id && rental.deleted == 'F'))
		fread(&rental, sizeof(type_rental), 1, pointer_rental);
	
	if(rental.rental_id == rental_id && rental.deleted == 'F')
	{
		pos = ftell(pointer_rental) - sizeof(type_rental);
		return pos;
	}
	return -1; 
	
}

int find_loan_by_id(FILE *pointer_loan, int loan_id)
{
	type_loan loan;
	int pos;
	
	
	rewind(pointer_loan);
	fread(&loan, sizeof(type_loan), 1, pointer_loan);
	while(!feof(pointer_loan) && !(loan.deleted == 'F' && loan.loan_id == loan_id))
		fread(&loan, sizeof(type_loan), 1, pointer_loan);
	
	if(loan.deleted == 'F' && loan.loan_id == loan_id)
	{
		pos = ftell(pointer_loan) - sizeof(type_loan);
		return pos;
	}
	return -1;
	
}

int find_vehicle_rental(FILE *pointer_relation_vehicle_rental, int rental_id, char vehicle_id[8])
{
	int pos;
	type_relation_vehicle_rental relation_vehicle_rental;
	
	rewind(pointer_relation_vehicle_rental);
	fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	while(!feof(pointer_relation_vehicle_rental) && !(relation_vehicle_rental.rental_id == rental_id && strcmp(relation_vehicle_rental.vehicle_id, vehicle_id) == 0 && relation_vehicle_rental.deleted == 'F'))
		fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);

		
	if(relation_vehicle_rental.rental_id == rental_id && strcmp(relation_vehicle_rental.vehicle_id, vehicle_id) == 0 && relation_vehicle_rental.deleted =='F')
	{
		pos = ftell(pointer_relation_vehicle_rental)- sizeof(type_relation_vehicle_rental);
		return pos;
	}
	return -1;
	
}

int find_loan_by_vehicle(FILE *pointer_loan, char vehicle_id[8])
{
	type_loan loan;
	int pos;
	
	rewind(pointer_loan);
	fread(&loan, sizeof(type_loan), 1, pointer_loan);
	while(!feof(pointer_loan) && !(loan.deleted == 'F' &&strcmp(loan.vehicle_id, vehicle_id) == 0))
		fread(&loan, sizeof(type_loan), 1, pointer_loan);
	
	if(loan.deleted == 'F' &&strcmp(loan.vehicle_id, vehicle_id) == 0)
	{
		pos = ftell(pointer_loan) - sizeof(type_loan);
		return pos;
	}
	return -1;
	
}

int find_relation_by_vehicle(FILE *pointer_relation_vehicle_rental, char vehicle_id[8])
{
	type_relation_vehicle_rental relation_vehicle_rental;
	int pos;
	
	rewind(pointer_relation_vehicle_rental);
	fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	while(!feof(pointer_relation_vehicle_rental) && !(relation_vehicle_rental.deleted == 'F' && strcmp(relation_vehicle_rental.vehicle_id, vehicle_id) == 0))
		fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	
		
	if(relation_vehicle_rental.deleted == 'F' && strcmp(relation_vehicle_rental.vehicle_id, vehicle_id) == 0)
	{
		pos = ftell(pointer_relation_vehicle_rental) - sizeof(type_relation_vehicle_rental);
		return pos;
	}
	return -1;
	
}


int validate_date(type_date date)
{
	if(date.year > 1899 && date.year < 2101)
		if(date.month > 0 && date.month < 13)
			if(date.day > 0 && date.day < 32)
				return 1;

	return 0;
}

type_date read_date()
{
	type_date date;
	int flag = 0;
	
	do
	{
		if(flag)
		{
			
			printf("Data invalida!\n");
			printf("Digite novamente [DD/MM/AAAA]\n");
			scanf("%d/%d/%d", &date.day, &date.month, &date.year);
		}
		scanf("%d/%d/%d", &date.day, &date.month, &date.year);
		
		flag = 1;
	}while(!validate_date(date));

	return date;
}



void print_vehicle(FILE *pointer_vehicle, char vehicle_id[8])
{	
	type_vehicle vehicle;
	
	int pos_vehicle = find_vehicle_by_id(pointer_vehicle, vehicle_id);
	
	fseek(pointer_vehicle, pos_vehicle, SEEK_SET);
	fread(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);

	printf("Placa:%s\nModelo:%s\nAno de fabricação:%d\n", vehicle.vehicle_plate, vehicle.vehicle_model, vehicle.fabrication_year);	
}

void print_loan(FILE *pointer_loan, int loan_id)
{
	type_loan loan;
	
	fseek(pointer_loan, find_loan_by_id(pointer_loan, loan_id), SEEK_SET);
	fread(&loan, sizeof(type_loan), 1, pointer_loan);
	
	printf("Id do empréstimo: %d\nCPF do cliente: %s\nData do empréstimo: %d/%d/%d\nData de devolução: %d/%d/%d\nData em que foi devolvido%d/%d/%d\n", loan.loan_id, loan.client_id, loan.loan_date.day, loan.loan_date.month, loan.loan_date.year, loan.return_date.day, loan.return_date.month, loan.return_date.year, loan.returned_date.day, loan.returned_date.month, loan.returned_date.year);	
}

void print_relation(FILE *pointer_relation_vehicle_rental, char vehicle_id[8])
{
	FILE *pointer_rental = fopen(dir_rental, "rb");
	
	type_relation_vehicle_rental relation_vehicle_rental;
	type_rental rental;
	
	
		
	fseek(pointer_relation_vehicle_rental, find_relation_by_vehicle(pointer_relation_vehicle_rental, vehicle_id), SEEK_SET);
	fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	
	fseek(pointer_rental, find_rental_by_id(pointer_rental, relation_vehicle_rental.rental_id), SEEK_SET);
	fread(&rental, sizeof(type_rental), 1, pointer_rental);
	
	printf("Id da locadora: %d\nNome da locadora: %s\nPlaca do veículo:%s\n", relation_vehicle_rental.rental_id, rental.rental_name, relation_vehicle_rental.vehicle_id);
	
	fclose(pointer_rental);
}


void create_vehicle()
{
	FILE* pointer_vehicle = fopen(dir_vehicle, "rb+");
	type_vehicle vehicle;
	fflush(stdin);

	puts("Digite a placa do carro [0 para sair]\n");
	gets(vehicle.vehicle_plate);
	while(strcmp(vehicle.vehicle_plate, "0") != 0)
	{
		if(find_vehicle_by_id(pointer_vehicle, vehicle.vehicle_plate) == -1)
		{

			puts("Digite o modelo do carro\n");
			gets(vehicle.vehicle_model);
			puts("Digite o ano de fabricação\n");
			scanf("%d", &vehicle.fabrication_year);
			fflush(stdin);
			vehicle.deleted = 'F';
			fseek(pointer_vehicle, 0, SEEK_END);
			fwrite(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);
			printf("Veiculo cadastrado com sucesso\n");
			clear_screen();
		}
		else
		{
			printf("Placa ja cadastrada!\n");
			clear_screen();
		}
		printf("Digite a placa do carro [0 para sair]\n");
		gets(vehicle.vehicle_plate);
	}
	fclose(pointer_vehicle);
}

void create_loan()
{
	FILE *pointer_loan = fopen(dir_loan, "rb+");
	FILE *pointer_vehicle = fopen(dir_vehicle, "rb");
	FILE *pointer_client = fopen(dir_client, "rb");

	type_loan loan;
	fflush(stdin);

	printf("Digite a placa do veículo a ser emprestado: [Digite 0 para sair]\n");
	gets(loan.vehicle_id);

	while(strcmp(loan.vehicle_id, "0") != 0)
	{
		if(find_vehicle_by_id(pointer_vehicle, loan.vehicle_id) != -1)
		{
				printf("Digite o CPF do locatário\n");
				gets(loan.client_id);
				if(find_client_by_id(pointer_client, loan.client_id) != -1)
				{
					printf("Digite a data do emprestimo [DD/MM/AAAA]\n");
					loan.loan_date = read_date();
					printf("Digite a data de devolução [DD/MM/AAAA]\n");
					loan.return_date = read_date();
					printf("Digite a data em que foi devolvido [DD/MM/AAAA]\n");
					loan.returned_date = read_date();

					loan.loan_id = generate_loan_id(pointer_loan);
					loan.deleted = 'F';
					fseek(pointer_loan, 0, SEEK_END);
					fwrite(&loan, sizeof(type_loan), 1, pointer_loan);
					getchar();
					printf("Emprestimo cadastrado com sucesso!\n");
					clear_screen();
				}
				else
				{
					printf("CPF Inválido ou não encontrado\n");
					clear_screen();

				}
		}
		else
		{
			printf("Veículo não encontrado!\n");
			clear_screen();
		}
		printf("Digite a placa do veículo a ser emprestado: [Digite 0 para sair]\n");
		gets(loan.vehicle_id);
	}


	fclose(pointer_loan);
	fclose(pointer_vehicle);
	fclose(pointer_client);
}

void create_client()
{
	FILE *pointer_client = fopen(dir_client, "rb+");
	type_client client;
	
	printf("Digite o CPF do cliente sem acentos ou pontuações [Digite 0 para sair]: \n");
	gets(client.client_cpf);
	while(strcmp(client.client_cpf, "0") != 0 )
	{
			if(validate_client_id(client.client_cpf) && find_client_by_id(pointer_client, client.client_cpf) == -1)
			{
				printf("Digite o nome do cliente: \n");
				gets(client.client_name);
				printf("Digite o endereço do cliente: \n");
				gets(client.client_adress);
				printf("Digite o estado do cliente: \n");
				gets(client.client_state);
				printf("Digite a cidade do cliente: \n");
				gets(client.client_city);
				printf("Digite o telefone do cliente: \n");
				gets(client.client_tel);
				client.deleted = 'F';
				fseek(pointer_client, 0, SEEK_END);
				fwrite(&client, sizeof(type_client), 1, pointer_client);
				printf("Cliente cadastrado com sucesso!\n");
				clear_screen();
			}
			else
			{
				printf("CPF INVÁLIDO OU JÁ EXISTENTE\n");
				clear_screen();
			}
			printf("Digite o CPF do cliente sem acentos ou pontuações [Digite 0 para sair]: \n");
			gets(client.client_cpf);
	}
	fclose(pointer_client);
}


void create_rental()
{
	FILE *pointer_rental = fopen(dir_rental, "rb+");
	
	type_rental rental;
	
	printf("Digite o nome da locadora[Digite 0 para sair]: \n");
	gets(rental.rental_name);
	while(rental.rental_name[0] != '0')
	{
		printf("Digite a cidade da locadora: \n");
		gets(rental.rental_city);
		rental.rental_id = generate_rental_id(pointer_rental);
		rental.deleted = 'F';
		fseek(pointer_rental, 0, SEEK_END);
		fwrite(&rental, sizeof(type_rental), 1, pointer_rental);

		printf("Locadora cadastrada com sucesso!\n");
		clear_screen();
		printf("Digite o nome da locadora[Digite 0 para sair]: \n");
		gets(rental.rental_name);
	}
	fclose(pointer_rental);
	
}


void create_relation_vehicle_rental()
{
	FILE *pointer_relation_vehicle_rental = fopen(dir_relation, "rb+");
	FILE *pointer_vehicle = fopen(dir_vehicle, "rb+");
	FILE *pointer_rental = fopen(dir_rental, "rb+");
	
	type_relation_vehicle_rental relation_vehicle_rental;
	char vehicle_id[8];
	int rental_id;
	
	
	printf("Digite a placa do carro [Digite 0 para sair]: \n");
	gets(vehicle_id);
	while(vehicle_id[0] != '0')
	{		
		if(find_vehicle_by_id(pointer_vehicle, vehicle_id) != -1)
		{
			printf("Digite o ID da locadora: \n");
			scanf("%d", &rental_id);
			getchar();
			if(find_rental_by_id(pointer_rental, rental_id) != -1 && find_vehicle_rental(pointer_relation_vehicle_rental, rental_id, vehicle_id) == -1)
			{
				relation_vehicle_rental.deleted = 'F';
				relation_vehicle_rental.rental_id = rental_id;
				strcpy(relation_vehicle_rental.vehicle_id, vehicle_id);
				fseek(pointer_relation_vehicle_rental, 0, SEEK_END);
				fwrite(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
				printf("Relação Cadastrada com sucesso!\n");
				clear_screen();
			}
			else
			{
				printf("Locadora não encontrada ou relação já cadastrada!\n");
				clear_screen();
			}
		
		}
		else
		{
			printf("Placa não encontrada!\n");
			clear_screen();
		}
		printf("Digite a placa do carro [Digite 0 para sair]: \n");
		gets(vehicle_id);
	}
	
	fclose(pointer_relation_vehicle_rental);
	fclose(pointer_vehicle);

}

void aux_logic_delete_loan_by_id(FILE *pointer_loan, int loan_id)
{
	type_loan loan;
	int loan_pos = find_loan_by_id(pointer_loan, loan_id);
	
	fseek(pointer_loan, loan_pos, SEEK_SET);
	fread(&loan, sizeof(type_loan), 1, pointer_loan);
	loan.deleted = 'T';
	fseek(pointer_loan, loan_pos, SEEK_SET);
	fwrite(&loan, sizeof(type_loan), 1, pointer_loan);

}

void aux_logic_delete_relation_by_vehicle(FILE *pointer_relation_vehicle_rental, char vehicle_id[8])
{
	type_relation_vehicle_rental relation_vehicle_rental;
	
	fseek(pointer_relation_vehicle_rental, find_relation_by_vehicle(pointer_relation_vehicle_rental, vehicle_id), SEEK_SET);
	fread(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	relation_vehicle_rental.deleted = 'T';
	
	fseek(pointer_relation_vehicle_rental, -sizeof(type_relation_vehicle_rental), SEEK_CUR);
	fwrite(&relation_vehicle_rental, sizeof(type_relation_vehicle_rental), 1, pointer_relation_vehicle_rental);
	
}

void logic_delete_vehicle_by_id()
{
	FILE *pointer_vehicle = fopen(dir_vehicle, "rb+");
	FILE *pointer_loan = fopen(dir_loan, "rb+");
	FILE *pointer_relation_vehicle_rental = fopen(dir_relation, "rb+");
	type_vehicle vehicle;
	int vehicle_pos, loan_pos, relation_pos;
	char resp, resp2, resp3;
	
	
	printf("Digite a placa do veículo a ser excluído[Digite 0 para sair]: \n");
	gets(vehicle.vehicle_plate);
	while(vehicle.vehicle_plate[0] != '0')
	{
		vehicle_pos = find_vehicle_by_id(pointer_vehicle, vehicle.vehicle_plate);
		if(vehicle_pos != -1)
		{
			print_vehicle(pointer_vehicle, vehicle.vehicle_plate);
			printf("Confirme a exclusão deste veículo[S/N]: \n");
			scanf("%c", &resp);
			getchar();
			resp = toupper(resp);
			if(toupper(resp) == 'S')
			{
				loan_pos = find_loan_by_vehicle(pointer_loan, vehicle.vehicle_plate);
				resp2 = 'S';
				while(loan_pos != -1 && resp2 == 'S')
				{
					printf("Existe um empréstimo relacionado a este veículo\n");
					print_loan(pointer_loan, get_loan_id(pointer_loan, loan_pos));
					printf("Deseja continuar com a exclusão deste emprestímo[S/N]: \n");
					scanf("%c", &resp2);
					getchar();
					resp2 = toupper(resp2);
					if(resp2 == 'S')
					{
						aux_logic_delete_loan_by_id(pointer_loan, get_loan_id(pointer_loan, loan_pos));
						printf("Exclusão do empréstimo realizada com sucesso\n");
						loan_pos = find_loan_by_vehicle(pointer_loan, vehicle.vehicle_plate);
						clear_screen();
					}
					
				}
				if(resp2 == 'S')
				{
					relation_pos = find_relation_by_vehicle(pointer_relation_vehicle_rental, vehicle.vehicle_plate);
					resp3 = 'S';
					while(relation_pos != -1 && resp3 == 'S')
					{
						printf("Existe uma locadora relacionada a este veículo\n");
						print_relation(pointer_relation_vehicle_rental, vehicle.vehicle_plate);
						printf("Deseja continuar com a exclusão desta relação[S/N]: \n");
						resp3 = toupper(getchar());
						getchar();
												
						if(resp3 == 'S')
						{
							aux_logic_delete_relation_by_vehicle(pointer_relation_vehicle_rental, vehicle.vehicle_plate);
							printf("Exclusão da relação realizada com sucesso\n");
							relation_pos = find_relation_by_vehicle(pointer_relation_vehicle_rental, vehicle.vehicle_plate);
							clear_screen();
						}
						
						
					}
					if(relation_pos == -1 && loan_pos == -1)
					{
						fseek(pointer_vehicle, vehicle_pos, SEEK_SET);
						fread(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);
						vehicle.deleted = 'T';
						
						fseek(pointer_vehicle, vehicle_pos, SEEK_SET);
						fwrite(&vehicle, sizeof(type_vehicle), 1, pointer_vehicle);
						printf("Veículo excluido com suesso\n");
					}
				}
			}
		}
		else
		{
			printf("Veículo não encontrado!\n");
		}
		clear_screen();
		printf("Digite a placa do veículo a ser excluído[Digite 0 para sair]: \n");
		gets(vehicle.vehicle_plate);
	}
	
	fclose(pointer_vehicle);	
	fclose(pointer_loan);
	fclose(pointer_relation_vehicle_rental);
}





 



void create_files()
{
	FILE *vehicle;
	FILE *rental;
	FILE *loan;
	FILE *relation_vehicle_rental;
	FILE *client;

	vehicle = fopen(dir_vehicle, "rb");
	rental = fopen(dir_rental, "rb");
	loan = fopen(dir_loan,"rb");
	relation_vehicle_rental = fopen(dir_relation, "rb");
	client = fopen(dir_client, "rb");


	if(vehicle == NULL)
		vehicle = fopen(dir_vehicle, "wb");
	if(rental == NULL)
		rental = fopen(dir_rental, "wb");
	if(loan == NULL)
		loan = fopen(dir_loan, "wb");
	if(relation_vehicle_rental == NULL)
		relation_vehicle_rental = fopen(dir_relation, "wb");
	if(client == NULL)
		client = fopen(dir_client, "wb");


	fclose(vehicle);
	fclose(rental);
	fclose(loan);
	fclose(relation_vehicle_rental);
	fclose(client);
}



int main()
{
	create_files();
	// CORRIGIR RELACIONAMENTO DE VEICULO VARIOS PARA UM LOCADORA
	setlocale(LC_ALL, "Portuguese");
	
	create_rental();
	clear_screen();
	
	create_vehicle();
	clear_screen();
	
	create_relation_vehicle_rental();
	clear_screen();
	
	create_client();
	clear_screen();
	
	create_loan();
	clear_screen();
	
	logic_delete_vehicle_by_id();

	return 0;
}

