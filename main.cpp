#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <queue>
#include <string>
using namespace std;

const string STEPNAME[9] = {"None", "Light Hit", "Medium Hit", "Heavy Hit", "Draw", "Punch", "Bend", "Upset", "Shrink"};
const int STEPNUM[9] = {0, -3, -6, -9, -15, 2, 7, 13, 16};
const int INF = 0x7fffffff;
int laststep[3], ans, ansnum, low = INF;

HANDLE hout;

struct note {
	int dep, dis, stp;
	string list;
} noteop, noteed;

queue<note> dat, ret;
int flag[257];
int tdep, tdis, tstp = 1;
string tlist = "";

void save() {
	flag[tdis + 128] = tdep;
	noteed.dep = tdep;
	noteed.dis = tdis;
	noteed.stp = tstp;
	noteed.list = tlist;
	dat.push(noteed);
}

bool check() {
	if(tdis < -128 || tdis > 128 || flag[tdis + 128] < tdep) return true;
	return false;
}

string getmethodfirst(string list) {
	int i, len = list.size();
	string met = "";
	for(i = 0; i < len; i++) met += STEPNAME[list[i]] + " -> ";
	return met;
}

string getmethodlast() {
	int i;
	string met = "";
	for(i = 0; i < 3; i++) {
		if(laststep[i] == 0) continue;
		met += STEPNAME[laststep[i]] + " -> ";
	}
	return met;
}

void preinit() {
	SetConsoleTitle("Terra Forge Calculator");
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("---------------------------\n");
	printf("  copyright(c) 2019 Os-Ir\n");
	printf("  Terra Forge Calculator\n");
	printf("---------------------------\n");
}

void init() {
	int i;
	for(i = 0; i < 256; i++) flag[i] = INF;
	low = INF;
	tdep = tdis = ans = ansnum = 0;
	tstp = 1;
	tlist = "";
	while(!dat.empty()) dat.pop();
	SetConsoleTextAttribute(hout, 0x07);
	printf("\nPlease entry last 3 steps (spaces aren't required between two numbers)\n");
	for(i = 0; i < 9; i++) printf("%d:%s\n", i, STEPNAME[i].c_str());
	int idx = 0;
	char ch;
	SetConsoleTextAttribute(hout, 0x0b);
	while(true) {
		scanf("%c", &ch);
		if(ch >= '0' && ch <= '8') {
			ch -= '0';
			laststep[idx++] = ch;
			ans -= STEPNUM[ch];
		}
		if(idx == 3) break;
	}
	SetConsoleTextAttribute(hout, 0x07);
	printf("\nPlease entry the distance between the red and green arrows (between -128 and 128)\n");
	printf("p.s. Entry a negative number if the green arrow is on the left side of the red arrow. On the contrary, it is positive.\n");
	SetConsoleTextAttribute(hout, 0x0b);
	while(true) {
		scanf("%d", &tdis);
		if(!check()) break;
	}
	if(ans == tdis) {
		low = 0;
		ansnum = 1;
	}
	if(laststep[0] == 5 && laststep[1] == 1 && laststep[2] == 4 && tdis == 5) {
		SetConsoleTextAttribute(hout, 0x0d);
		printf("\nSatori: Where are you, Koishi?\n");
		SetConsoleTextAttribute(hout, 0x0a);
		printf("Koishi: I am behind you, sister!\n");
	}
}

void bfs() {
	int i, op = 1, ed = 2;
	save();
	while(op < ed) {
		noteop = dat.front();
		dat.pop();
		if(noteop.dep >= low) break;
		for(i = noteop.stp; i < 9; i++) {
			tdis = noteop.dis + STEPNUM[i];
			tdep = noteop.dep + 1;
			if(check()) continue;
			tlist = noteop.list + (char)i;
			tstp = i;
			save();
			ed++;
			if(tdis == ans) {
				ret.push(noteed);
				low = tdep;
				ansnum++;
			}
		}
		op++;
	}
}

void output() {
	int i;
	for(i = 0; i < 3; i++) if(laststep[i] != 0) low++;
	SetConsoleTextAttribute(hout, 0x0f);
	printf("\nStep number:%d\nMethod number:%d\n", low, ansnum);
	if(low == 0) {
		SetConsoleTextAttribute(hout, 0x0f);
		printf("Method 1: ");
		SetConsoleTextAttribute(hout, 0x0a);
		printf("Finish\n");
		return;
	}
	for(i = 1; i <= ansnum; i++) {
		SetConsoleTextAttribute(hout, 0x0f);
		printf("Method %d: ", i);
		SetConsoleTextAttribute(hout, 0x0c);
		printf("%s", getmethodfirst(ret.front().list).c_str());
		SetConsoleTextAttribute(hout, 0x0e);
		printf("%s", getmethodlast().c_str());
		SetConsoleTextAttribute(hout, 0x0a);
		printf("Finish\n");
		ret.pop();
	}
}

void end() {
	int mode;
	SetConsoleTextAttribute(hout, 0x07);
	printf("\nPlease choose exit[0] or (re)calculate[1]\n");
	SetConsoleTextAttribute(hout, 0x0b);
	while(true) {
		scanf("%d", &mode);
		if(mode == 0) {
			SetConsoleTextAttribute(hout, 0x07);
			exit(0);
		}
		else if(mode == 1) {
			break;
		}
	}
}

int main(int argc, char** argv) {
	preinit();
	while(true) {
		init();
		if(low != 0) bfs();
		output();
		end();
	}
	return 0;
}
