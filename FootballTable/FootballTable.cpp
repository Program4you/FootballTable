#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct command_t {
	char *name;
	int score;
	int balls_count;
	int games_count;

	int draws, wins, defeats;
} command_t;

typedef struct pair_t {
	char *command_name1;
	char *command_name2;
	int score2;
	int score1;
} pair_t;

// получение названий команд
command_t *get_command_names(int n) {
	command_t *commands = (command_t *)malloc(n * sizeof(command_t)); // заводим массив для футбольных команд

	int i;
	for (i = 0; i < n; i++) {
		printf("Enter command name #%d: ", i + 1); // предлагаем пользователю ввести названия команд
		commands[i].name = (char *)malloc(256); // заводим строку для названия команды
		commands[i].score = 0;
		commands[i].balls_count = 0;
		commands[i].games_count = 0;

		commands[i].wins = 0;
		commands[i].draws = 0;
		commands[i].defeats = 0;

		scanf("%s", commands[i].name);
	}

	return commands;
}

// распределение команд в группы
pair_t* get_pairs(command_t *commands, int n) {
	pair_t *pairs = (pair_t *)malloc(n / 2 * sizeof(pair_t));
	int i;

	for (i = 0; i < n / 2; i++)
		pairs[i].command_name1 = pairs[i].command_name2 = NULL;

	int index = 0;

	for (i = 0; i < n / 2; i++) {
		int x = rand() % (n / 2);

		while (pairs[x].command_name1 != NULL)
			x = rand() % (n / 2);

		pairs[x].command_name1 = commands[index++].name;

		x = rand() % (n / 2);

		while (pairs[x].command_name2 != NULL)
			x = rand() % (n / 2);

		pairs[x].command_name2 = commands[index++].name;
	}

	return pairs;
}

// ввод результатов об играх
void get_results(command_t *commands, pair_t *pairs, int n, int tournir) {
	printf("\nTournir %d:\n", tournir);
	int i;

	for (i = 0; i < n; i++) {
		printf("\tResult for %s ws %s: ", pairs[i].command_name1, pairs[i].command_name2);
		scanf("%d:%d", &pairs[i].score1, &pairs[i].score2);

		int index1 = 0;
		while (strcmp(commands[index1].name, pairs[i].command_name1))
			index1++;

		int index2 = 0;
		while (strcmp(commands[index2].name, pairs[i].command_name2))
			index2++;

		if (pairs[i].score1 == pairs[i].score2) {
			commands[index1].score++;
			commands[index2].score++;

			commands[index1].draws++;
			commands[index2].draws++;
		}
		else if (pairs[i].score1 > pairs[i].score2) {
			commands[index1].score += 3;

			commands[index1].wins++;
			commands[index2].defeats++;
		}
		else {
			commands[index2].score += 3;

			commands[index2].wins++;
			commands[index1].defeats++;
		}

		commands[index1].balls_count += pairs[i].score1;
		commands[index2].balls_count += pairs[i].score2;
		commands[index1].games_count++;
		commands[index2].games_count++;
	}
}

// вывод на экран таблицы с командами и результатами игр
void print_table(command_t *commands, pair_t *pairs, int n, int tournir) {
	int i, j;

	for (i = 0; i < n + 1; i++)
		printf("+-----------------");

	printf("+\n|                 ");
	for (i = 0; i < n; i++)
		printf("| %15s ", commands[i].name);

	printf("|\n");
	for (i = 0; i < n + 1; i++)
		printf("+-----------------");
	printf("+");

	for (i = 0; i < n; i++) {
		printf("\n| %15s |", commands[i].name);

		for (j = 0; j < n; j++) {
			if (i == j)
				printf(" ############### |");
			else {
				int i1 = 0, j1 = 0;
				while (i1 < n / 2 && strcmp(pairs[i1].command_name1, commands[i].name) && strcmp(pairs[i1].command_name2, commands[i].name))
					i1++;

				while (j1 < n / 2 && strcmp(pairs[j1].command_name1, commands[j].name) && strcmp(pairs[j1].command_name2, commands[j].name))
					j1++;

				if (j1 == i1) {
					int left_score = strcmp(pairs[i1].command_name1, commands[i].name) ? pairs[i1].score2 : pairs[i1].score1;
					int right_score = strcmp(pairs[j1].command_name1, commands[j].name) ? pairs[j1].score2 : pairs[j1].score1;

					printf(" %7d:%-7d |", left_score, right_score);
				}
				else
					printf("     ---:---     |");
			}
		}

		printf("\n");
		for (j = 0; j < n + 1; j++)
			printf("+-----------------");
		printf("+");
	}

	printf("\n\n");
}

void print_results(command_t *commands, int n) {
	int is_sorted = 0;
	int i;

	while (!is_sorted) {
		is_sorted = 1;

		for (i = 0; i < n - 1; i++)
			if (commands[i].score < commands[i + 1].score) {
				command_t tmp = commands[i];
				commands[i] = commands[i + 1];
				commands[i + 1] = tmp;
				is_sorted = 0;
			}
	}

	printf("+--------------------------------------------------------------------------+\n");
	printf("|                              Football table                              | \n");
	printf("+-----+-----------------+-------+------+---------+-------+-------+---------+\n");
	printf("|  #  |     Command     | Games | Wins | Defeats | Draws | Balls | Results |\n");
	printf("+-----+-----------------+-------+------+---------+-------+-------+---------+\n");

	for (i = 0; i < n; i++)
		printf("| %3d | %15s | %5d | %4d | %7d | %5d | %5d | %7d |\n", i + 1, commands[i].name, commands[i].games_count,
			commands[i].wins, commands[i].defeats, commands[i].draws, commands[i].balls_count, commands[i].score); // вывод самих команд

	printf("+-----+-----------------+-------+------+---------+-------+-------+---------+\n");
}

int main() {
	srand(time(NULL)); // сбрасываем генератор случайных чисел

	int n;
	printf("Enter number of commands (n must be even): ");
	scanf("%d", &n); // получаем число команд

					 // проверяем на некорректный ввод
	while (n % 2) {
		printf("Incorerect input. Try again: ");
		scanf("%d", &n);
	}

	command_t *commands = get_command_names(n); // получаем список названий команд

	int roundes;
	printf("enter number of tournirs: ");
	scanf("%d", &roundes);

	pair_t **tournirs = (pair_t **)malloc(roundes * sizeof(pair_t *));
	int i;

	for (i = 0; i < roundes; i++) {
		tournirs[i] = get_pairs(commands, n); // получаем пары команд
		get_results(commands, tournirs[i], n / 2, i + 1); // получаем результаты
		print_table(commands, tournirs[i], n, i + 1); // выводим таблицу на экран
	}

	print_results(commands, n);

	system("pause");

	return 0;
}