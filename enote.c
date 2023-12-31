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

const int MAX_CHAR = 255;

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
		fprintf(stderr, "ERROR: Failed to read note\n");
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
		fprintf(stderr, "ERROR: Failed to delete note \n");
		return;
	}

	return;
}

int main(int argc, char *argv[]) {
	char note_name[MAX_CHAR];

	if (argc < 3) {
		fprintf(stderr, "ERROR: Provide option and note \n");
		return 1;
	}

	strncpy(note_name, argv[2], MAX_CHAR);

	switch (argv[1][1]) {
	case 'w':
		write_note(note_name);
		break;
	case 'r':
		read_note(note_name);
		break;
	case 'd':
		delete_note(note_name);		
		break;
	default:
		printf("usage: enote [OPTION] [FILE/NOTE]\n\n");
		printf("-w\tWrite a new note\n-r\tRead a note\n-d\tDelete a note\n");
	}

	return 0;
}
