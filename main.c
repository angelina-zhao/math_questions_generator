#include <string.h>
#include <time.h>  
#include <stdio.h>  
#include <stdlib.h>  


struct question {
	int operator[3];
	int number[4];
	int answer;
	int checksum[2];
};

int init_question(struct question * q) {
	memset(q, 0x0, sizeof(struct question));
	return 0;
}

int get_rnd(double min, double max)  {	
	int rnd = 0;
	int cnt = 1;
	while (1) {
		cnt++;
		rnd = ((int)(max*rand()/(RAND_MAX+1.0)));
		if (min > 0.1) {
			if (rnd > (int)min)
				break;
		} else {
			break;
		}
//		if ((cnt % 1000) == 0)
//			printf("get_rnd min %lf, max %lf...rnd %d\n", min, max, rnd);			
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

int print_question(struct question * q) {
	int i;

	printf("\t\t%2d\n", q->number[0]);
	for (i = 0; i < 3; i++) {
		if (q->operator[i] == 1) {
			printf("\t\t-%d\n", q->number[i + 1]);
		} else {
			printf("\t\t%2d\n", q->number[i + 1]);
		}
	}

	printf("\n\n");

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


int main() {
	struct question q[10010];
	int i;
	int a,b;
	int ret;

    srand((int)time(0));

	for(i = 0; i < 520; i++) {
//		printf("generate for [%d]:\n", i);
		do {
			init_question(&q[i]);
			q[i].answer = -1;
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
	return 0;
}

