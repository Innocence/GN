#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LISTS 1000
typedef struct boards
{
	char name[100];
	int points;
} board;

int n = 4;
int cnt = 10;
const int total = 100;
const int point = 10;

board B[LISTS];

void menu();
void end();
void game();
void help();
void settings();
void show();
int choose();

int main(void)
{
	int flag = 1;

	while (flag)
	{
		menu();
		int choice = choose();
		switch (choice)
		{
			case 0:
				end();
				flag = 0;
				break;
			case 1:
				game();
				break;
			case 2:
				show();
				break;
			case 3:
				help();
				break;
			case 4:
				settings();
				break;
			default:
				break;
		}
	}
	return 0;
}

void menu()
{
	printf("\n --------------------\n");
	printf("      猜数字游戏     \n");
	printf(" --------------------\n");
	printf("|    1.开始游戏      |\n");
	printf("|    2.积分榜单      |\n");
	printf("|    3.帮助信息      |\n");
	printf("|    4.系统设置      |\n");
	printf("|    0.退出程序      |\n");
	printf(" --------------------\n");
	printf("请选择对应的操作: ");
}

int choose()
{
	int select;
	while (1)
	{
		int result = scanf("%d", &select);
		getchar();
		if (result != 1 || select < 0 || select > 4)
		{
			printf("输入错误,请输入数字[0-4]:");
		}
		else
			return select;
	}
}

void end()
{
	printf("\nGame over!\n");
}

void game()
{
	int pts = total;

	srand((unsigned)time(NULL));
	int num[10] = {0};
	int gen[n];
	for (int i = 0; i < n;)
	{
		int tmp = rand() % 10;
		if (num[tmp] == 1)
			continue;
		else
		{
			num[tmp] = 1;
			gen[i++] = tmp;
		}
	}
	printf("\n计算机已生成%d个数字范围在0-9的随机数\n", n);
	printf("请输入%d个猜测,每输入一个数按回车结束,共有%d次机会\n", n, cnt);

	int guess[n], k;
	for (k = 0; k < cnt; k++)
	{
		printf("\n----第%d次猜测----\n\n", k + 1);

		int input[10] = {0};
		int x = 0, y = 0;
		for (int i = 0; i < n;)
		{
			printf("请输入第%d个数: ", i + 1);
			int t = scanf("%d", &guess[i]);
			while (getchar() != '\n')
			{
				continue;
			}
			if (t != 1 || guess[i] < 0 || guess[i] > 9)
			{
				printf("请输入数字(0-9)\n");
				continue;
			}
			else if (input[guess[i]] == 1)
			{
				printf("已输入过%d,请重新输入\n", guess[i]);
			}
			else
			{
				input[guess[i]] = 1;
				if (guess[i] == gen[i])
				{
					x++;
				}
				else if (num[guess[i]] == 1)
				{
					y++;
				}
				i++;
			}
		}
		printf("\n有%d个数数值和位置都相同!有%d个数数值相同,但位置不同\n", x, y);
		if (x == n)
		{
			printf("\n猜对了!\n");
			break;
		}
		else
		{
			pts -= point;
		}

	}

	if (k < cnt)
	{
		FILE *fp = fopen("board.txt", "r");
		if (fp == NULL)
		{
			perror("打开文件失败");
			FILE *tp = fopen("board.txt", "w");
			if (tp == NULL)
			{
				perror("\n写入排行榜失败\n");
			}
			else
			{
				printf("\n恭喜进入排行榜,请输入姓名: ");
				char nameTmp[100];
				scanf("%s", nameTmp);
				while(getchar() != '\n');
				fprintf(tp, "%s %d\n", nameTmp, pts);
				fclose(tp);
			}
		}
		else
		{
			int count;
			board read[LISTS];
			for (count = 0; count < LISTS; count++)
			{
				int tmp = fscanf(fp, "%s %d", read[count].name, &read[count].points);
				if (tmp != 2)
					break;
			}
			fclose(fp);

			if (pts >= read[count - 1].points || count < 5)
			{
				FILE *xp = fopen("board.txt", "w+");
				if(xp == NULL)
				{
					perror("\n写入排行榜失败\n");
					return;
				}
				printf("\n恭喜进入排行榜,请输入姓名: ");
				char nameTmp[100];
				scanf("%s", nameTmp);
				while(getchar() != '\n');

				int ppt[11] = {0};
				int j, p;
				for (j = 0, p = 0; j < count; j++)
				{
					if (read[j].points > pts)
					{
						fprintf(xp, "%s %d\n", read[j].name, read[j].points);
						if (ppt[read[j].points / 10] == 0)
						{
							p++;
							ppt[read[j].points / 10] = 1;
						}
					}
					else
						break;
				}
				if (p == 4)
				{
					fprintf(xp, "%s %d\n", nameTmp, pts);
					ppt[pts / 10] = 1;
					p++;
					for (; j < count; j++)
						if (read[j].points == pts)
							fprintf(xp, "%s %d\n", read[j].name, read[j].points);
				}
				else
				{
					fprintf(xp, "%s %d\n", nameTmp, pts);
					ppt[pts / 10] = 1;
					p++;
					for (; j < count && p < 5; j++)
					{
						fprintf(xp, "%s %d\n", read[j].name, read[j].points);
						if (ppt[read[j].points / 10] == 0)
						{
							p++;
							ppt[read[j].points / 10] = 1;
						}
					}
					if (j < count)
					{
						for (; j < count; j++)
						{
							if (ppt[read[j].points / 10] == 1)
								fprintf(xp, "%s %d\n", read[j].name, read[j].points);
						}
					}
				}
				fclose(xp);
			}
			else
			{
				printf("\n虽然猜对了,但是积分没能进入前5名,下次继续努力!\n");
			}

		}
	}
	else
	{
		printf("\n很遗憾,没有猜对!\n");
	}
}

void help()
{
	printf("\n随机生成的数均无重复,且数值在0-9范围内\n");
	printf("总分100分,每失败一次扣10分\n");
}

void settings()
{
	printf("\n当前随机生成个数为: %d  尝试次数为: %d\n", n, cnt);
	while (1)
	{
		printf("请输入新的随机生成个数(1-10)和尝试次数(1-10): ");
		int result = scanf("%d%d", &n, &cnt);
		getchar();
		if (result != 2 || n < 1 || n > 10 || n < 1 || n > 10)
			continue;
		else
			break;
	}
	printf("\n设置成功,新的随机生成个数为: %d  尝试次数为: %d\n", n, cnt);
}
void show()
{
	FILE *fp = fopen("board.txt", "r");
	if (fp == NULL)
	{
		perror("\n打开排行榜失败或排行榜为空\n");
	}
	else
	{
		printf("\n-----积分排行榜-----\n\n");
		for (int i = 0, j = 1;; i++)
		{
			int tmp = fscanf(fp, "%s %d", B[i].name, &B[i].points);
			if (tmp != 2)
				break;
			else
			{
				if(i > 0 && B[i].points != B[i - 1].points)
				{
					j++;
				}
				printf("%d\t%-10s   %2d分\n", j, B[i].name, B[i].points);

			}
		}
	}
	fclose(fp);
}
