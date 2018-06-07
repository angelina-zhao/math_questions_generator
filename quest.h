#ifndef QUEST_H
#define QUEST_H

struct settings_ {
	int is_rnd_initialed;
};

struct question {
	int operator[3];
	int number[4];
	int answer;
	int checksum[2];
};

#define MAX_QUEST_NUM	10010

#endif