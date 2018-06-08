#include <string.h>
#include <time.h>  
#include <stdio.h>  
#include <stdlib.h>

#include “quest.h”

struct settings_ g_settings;

int init_question(struct question * q) {
	memset(q, 0x0, sizeof(struct question));
	q->answer = -1;
	return 0;
}

int get_rnd(double min, double max)  {	
	int rnd = 0;
	int dbg_cnt = 0;

	if (!g_settings.is_rnd_initialed) {
		srand((int)time(0));
		g_settings.is_rnd_initialed = 1;
	}
		
	while (1) {
		dbg_cnt++;

		rnd = ((int)(max*rand()/(RAND_MAX+1.0)));
		if (min > 0.1) {
			if (rnd > (int)min)
				break;
		} else {
			break;
		}

		if ((dbg_cnt % 500) == 0) {
			printf("get_rnd min %lf, max %lf\n", min, max);
		}		
	}

	return rnd;
}

int verify_question(struct question * q) {
	int i;
	int sum;

	sum = q->number[0];	
	for (i = 1; i < 4; i++) {
		if (q->operator[i - 1] == 1) {
			sum -= q->number[i];
		}
		else {
			sum += q->number[i];
		}
		if (sum < 0 || sum > 99)
			return -1;	
	}

	return sum;
}

int dbg_print_question(struct question * q) {
	int i;

	for (i = 0; i < 4; i++) {
		printf("%d", q->number[i]);
		if (i < 3) {
			if (q->operator[i] == 1)
				printf("-");
			else
				printf("+");
		} else {
				printf("= %d..checksum(%d, %d)", q->answer, q->checksum[0], q->checksum[1]);
		}
	}

	printf("\n");

	return 0;
}

int print_one_question(struct question * q, int num_sz) {
	int i;

	printf("(*%2d, +%2d)\n", q->checksum[0], q->checksum[1]);
	printf("     %2d\n", q->number[0]);
	for (i = 0; i < num_sz - 1; i++) {
		if (q->operator[i] == 1) {
			printf("     -%d\n", q->number[i + 1]);
		} else {
			printf("     %2d\n", q->number[i + 1]);
		}
	}
	printf("--------\n");

	return 0;	
}

int print_questions(struct question * q, int q_sz, int num_sz, int eliminate_enter_interval) {
	int i;

	i = 0;
	do {
		print_one_question(&q[i], num_sz);
		i++;
		if (eliminate_enter_interval != 0) {
			if ((i % eliminate_enter_interval))
				printf("\n");
		}
	} while(i < q_sz);

	return 0;	
}


int fill_question(struct question * q) {
	int big_num_pos;
	int minus_pos;	
	int i, j;
	int repeat;
	int sum, int1;
	double min;
	

//	big_num_pos = get_rnd(0.0, 1.5);
	big_num_pos = get_rnd(0.0, 3.5);
	do {
		minus_pos = get_rnd(0.0, 2.5);
	} while(minus_pos == (big_num_pos - 1));

//	printf("fill q: big_num %d, minus_pos %d\n", big_num_pos, minus_pos);

	sum = 0;
	if(0 != big_num_pos) {
		q->number[0] = get_rnd(0.5, 8.0);		
//		printf("littlenum_0: %d\n", q->number[0]);
	} else {
		q->number[0] = get_rnd(9.5, 99.0);		
//		printf("bignum_0: %d\n", q->number[0]);
	}
	
	sum = q->number[0];
	for (i = 0; i < 3; i++) {
		if(i + 1 != big_num_pos) {
			do {
				repeat = 0;	
				if (minus_pos == i) {
					int1 = sum > 10 ? sum % 10 : sum;
					if (int1 > 8.5)
						min = 0.5;
					else
						min = int1;
				} else {	
					int1 = sum > 10 ? sum % 10 : sum;
					min = (10 - int1) + 1;
					if (min > 8.5)
						min = 0.5;		
				}
				q->number[i + 1] = get_rnd(min, 9.5);
				
				//Check repeat;
				for (j = 0; j < i + 1; j++) {
					if (q->number[i + 1] == q->number[j]) {
						repeat = 1;						
//						printf("repeat found, i %d, j %d, %d-%d\n", i + 1, j, q->number[i + 1] ,q->number[j]);
						return -1;
					}
				}
			} while (repeat);	
//			printf("littlenum[%d]: %d\n", i + 1, q->number[i + 1]);
			sum += q->number[i + 1];
		} else {
			q->number[i + 1] = get_rnd(9.5, 99.5);
//			printf("bignum[%d]: %d\n", i + 1, q->number[i + 1]);
			sum += q->number[i + 1];
		}
	}

	q->operator[minus_pos] = 1;

	return 0;
}


int generate_exercises_4() {
#if 0	
	struct question q[10010];
	int i;
	int a,b;
	int ret;

	srand((int)time(0));

	for(i = 0; i < 520; i++) {
//		printf("generate for [%d]:\n", i);
		do {
			init_question(&q[i]);
			ret = fill_question(&q[i]);
			if (ret == 0)
				q[i].answer = verify_question(&q[i]);
		} while(q[i].answer == -1);
		a = q[i].answer % 10;
		b = q[i].answer / 10;
		q[i].checksum[0] = a*b;
		q[i].checksum[1] = a+b;
	}

	for(i = 0; i < 520; i++) {
		printf("\t(*%d, +%d)\n", q[i].checksum[0], q[i].checksum[1]);
		print_question(&q[i]);
//		dbg_print_question(&q[i]);
	}
#endif	
	return 0;
}


int generate_one_exercise_2(struct question *q) {
	int number_1, number_2;
	double limit;
	int sign, exchange;
	int sum, checksum;

	if((!g_settings.is_rnd_initialed) || !q)
		return -1;

	number_1 = get_rnd(9.5, 100.0);

	//Check the possibility of the sign.
	int units = number_1 % 10;
	if (units == 9) {
		sign = 0;
	} else if( units == 0 || units == 1) {
		sign = 1;
	} else {
		sign = -1;
	}

	if (sign == -1) {
		sign = get_rnd(0.0, 2.0);
	}

	if (sign == 0) {
		limit = (10.0 - (number_1 % 10) + 0.5);
	} else {
		limit = (number_1 % 10) + 0.5;
	}

	number_2 = get_rnd(limit, 10.0);

	sum = (sign == 0) ? (number_1 + number_2) : (number_1 - number_2);
	if ((sum < 0) || (sum > 100))
		return -2;

	exchange = get_rnd(0.0, 2.0);
	q->operator[0] = sign;
	q->number[0] = number_1;
	q->number[1] = number_2;
	q->checksum[0] = (sum % 10) *  (sum / 10);
	q->checksum[1] = (sum % 10) +  (sum / 10);
	q->answer = sum;

	return 0;
}

/* one XX plus/minus X with flag*/
int generate_exercises_2(struct question *q, int size) {
	int i;
	int ret;

	i = 0;
	do {
		ret = generate_one_exercise_2(&q[i]);
		if (ret < 0) {
			if (ret == -2)
				continue;
			else
				return ret;
		}
		if ((q[i].answer / 10) == (q[i].number[0] / 10)) {
			printf("error detected...\n");
			return -1;
		}
		//printf("[%05d] %2d %c %2d = %d\n", i, q[i].number[0], (q[i].operator[0] == 0) ? '+':'-', q[i].number[1], q[i].answer);
		i++;
	} while(i < size);

	return 0;	
}


int init_global(struct question * q, int item_sz) {
	int i;

	memset(&g_settings, 0x0, sizeof(g_settings));

	if (!q) {
		printf("init_global failed...[q is NULL!!!]/n");
		return -1;
	}

	for(i = 0; i < item_sz; i++) {
		init_question(&q[i]);
	}

	srand((int)time(0));
	g_settings.is_rnd_initialed = 1;

	return 0;
}


int main() {	
	struct question q[MAX_QUEST_NUM];
	int ret;
	int i;

	ret = init_global(q, sizeof(q) / sizeof(struct question));
	if (ret < 0)
		return ret;

	ret = generate_exercises_2(q, sizeof(q) / sizeof(struct question));
	if (ret < 0)
		return ret;

	//printf("%2d %c %2d = %d\n", q[0].number[0], (q[0].operator[0] == 0) ? '+':'-', q[0].number[1], q[0].answer);
	print_questions(q, sizeof(q) / sizeof(struct question), 2, 5);

	return ret;
}
