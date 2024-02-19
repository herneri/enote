/*
	enote: Write notes on the CLI

	Copyright 2023 Eric Hernandez

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		https://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

const char *dir_name = ".enote/";
const int MAX_CHAR = 255;

int check_dir(char *path) {
	int fd = open(path, O_RDONLY);
	if (fd != -1) {
		close(fd);
		return 0;
	}

	printf("enote: NOTICE: Note directory not found, creating new one\n");

	int result = mkdir(path, 0770);
	if (result == -1) {
		fprintf(stderr, "enote: ERROR: Failed to create note directory\n");
		exit(1);
	}

	return 0;
}

void list_dir(char *path, const char mode) {
	DIR *enote_dir;
	struct dirent *entry;

	enote_dir = opendir(path);
	while ((entry = readdir(enote_dir)) != NULL) {
		if (entry->d_type == DT_DIR) {
			continue;
		}

		if (mode == 'd') {
			printf("%s \n", entry->d_name);
			continue;
		} else if (mode == 'c') {
			printf("%s \t", entry->d_name);
			continue;
		}
	}

	printf("\n");
	closedir(enote_dir);
	return;
}

void write_note(char *path) {
	char temp[MAX_CHAR];
	FILE *f = fopen(path, "w");
	if (f == NULL) {
		return;
	}

	printf("Writing to '%s': \n", path);
	while (1) {
		fgets(temp, MAX_CHAR, stdin);
		if (strcmp(temp, ">E\n") == 0) {
			fclose(f);
			return;
		}

		fprintf(f, temp);
	}

	return;
}

void read_note(char *path) {
	char temp[MAX_CHAR];
	FILE *f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr, "enote: ERROR: Failed to read note\n");
		return;
	}

	printf("Reading from '%s': \n\n", path);
	while (fgets(temp, MAX_CHAR, f) != NULL) {
		printf("%s", temp);
	}

	fclose(f);
	return;
}

void delete_note(char *path) {
	int result = remove(path);
	if (result != 0) {
		fprintf(stderr, "enote: ERROR: Failed to delete note \n");
		return;
	}

	return;
}

int main(int argc, char *argv[]) {
	char *dir_path = strncat(getenv("HOME"), dir_name, MAX_CHAR);
	char note_name[MAX_CHAR];

	if (argc == 1 || (argc == 2 && argv[1][1] == 'h')) {
		printf("usage: enote [OPTION] [FILE/NOTE]\n\n");
		printf("-w\tWrite a new note\n-r\tRead a note\n-d\tDelete a note\n-l\tList enote entries\n");
		return 0;
	}

	check_dir(dir_path);

	if (argc == 2 && argv[1][1] == 'l') {
		if (strlen(argv[1]) != 3) {
			list_dir(dir_path, 'd');
			return 0;
		}

		if (argv[1][2] != 'c') {
			fprintf(stderr, "enote: Invalid listing mode, enter -l or -lc for default or column listing \n");
			return 1;
		}

		list_dir(dir_path, 'c');
		return 0;
	}

	if (argc < 3) {
		fprintf(stderr, "enote: Invalid arguments \n");
		return 1;
	}

	strncpy(note_name, argv[2], MAX_CHAR);
	char *file = strncat(dir_path, note_name, MAX_CHAR);

	switch (argv[1][1]) {
	case 'w':
		write_note(file);
		break;
	case 'r':
		read_note(file);
		break;
	case 'd':
		delete_note(file);		
		break;
	default:
		fprintf(stderr, "enote: ERROR: Invalid option \n\n");
		printf("-w\tWrite a new note\n-r\tRead a note\n-d\tDelete a note\n-l\tList enote entries\n");
	}

	return 0;
}
