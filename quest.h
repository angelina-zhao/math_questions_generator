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

int init_question(struct question * q);
int init_global(struct question * q, int item_sz);

int get_rnd(double min, double max);
int verify_question(struct question * q);
int dbg_print_question(struct question * q);
int print_one_question(struct question * q, int num_sz);
int print_questions(struct question * q, int q_sz, int num_sz, int eliminate_enter_interval);
int fill_question(struct question * q);
int generate_exercises_4();
/* one XX plus/minus X with flag*/
int generate_one_exercise_2_1(struct question *q);
int generate_exercises_2_1(struct question *q, int size);
/* one XX plus/minus XX with flag*/
int generate_one_exercise_2_2(struct question *q);
int generate_exercises_2_2(struct question *q, int size);

#endif
