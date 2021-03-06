#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "enumType.hpp"
#include "tokenscanner.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include <cmath>
using namespace std;

const int SIZE = 4 * 1024 * 1024;
static int reg[33], heapLen = 0, conti = 0;
static unsigned char Memory[SIZE];
static bool shutdown = false, hReg = false;
static REGISTER preReg = none;
struct cmd
{
	INSRTUCT type;
	REGISTER regi[3];
	int Lable, num, regiSize;
	bool flag;
};
struct command
{
	INSRTUCT type;
	int Size;
	string v[4];
};
struct REG
{
	bool MUL;
	REGISTER Rtype;
	INSRTUCT Itype;
	int result, low, high;
};
struct MEM
{
	int pos;
	REGISTER Rtype;
	INSRTUCT Itype;
	int result, low, high;
	bool MUL;
};
static vector<string> code;
static vector<cmd> CMD;
static map<string, int> lable;
static map<string, REGISTER> regType;
static map<string, INSRTUCT> insType;
static cmd COMMAND;
static string instruct;
static REG Reg;
static MEM Mem;
static queue<REG> regi;
static queue<MEM> memo;
static queue<cmd> comm;
static queue<int> line;

void init()
{
	regType["$zero"] = regType["$0"] = zero;
	regType["$at"] = regType["$1"] = at;
	regType["$v0"] = regType["$2"] = v0;
	regType["$v1"] = regType["$3"] = v1;
	regType["$a0"] = regType["$4"] = a0;
	regType["$a1"] = regType["$5"] = a1;
	regType["$a2"] = regType["$6"] = a2;
	regType["$a3"] = regType["$7"] = a3;
	regType["$t0"] = regType["$8"] = t0;
	regType["$t1"] = regType["$9"] = t1;
	regType["$t2"] = regType["$10"] = t2;
	regType["$t3"] = regType["$11"] = t3;
	regType["$t4"] = regType["$12"] = t4;
	regType["$t5"] = regType["$13"] = t5;
	regType["$t6"] = regType["$14"] = t6;
	regType["$t7"] = regType["$15"] = t7;
	regType["$s0"] = regType["$16"] = s0;
	regType["$s1"] = regType["$17"] = s1;
	regType["$s2"] = regType["$18"] = s2;
	regType["$s3"] = regType["$19"] = s3;
	regType["$s4"] = regType["$20"] = s4;
	regType["$s5"] = regType["$21"] = s5;
	regType["$s6"] = regType["$22"] = s6;
	regType["$s7"] = regType["$23"] = s7;
	regType["$t8"] = regType["$24"] = t8;
	regType["$t9"] = regType["$25"] = t9;
	regType["$gp"] = regType["$28"] = gp;
	regType["$sp"] = regType["$29"] = sp;
	regType["$fp"] = regType["$30"] = fp;
	regType["$ra"] = regType["$31"] = ra;
	regType["$hi"] = hi;
	regType["$lo"] = lo;
	regType["$pc"] = pc;

	insType[".align"] = Align;
	insType[".ascii"] = ascii;
	insType[".asciiz"] = asciiz;
	insType[".byte"] = byte;
	insType[".half"] = half;
	insType[".word"] = word;
	insType[".space"] = space;
	insType[".data"] = Data;
	insType[".text"] = text;
	insType["add"] = add;
	insType["addu"] = addu;
	insType["addiu"] = addiu;
	insType["sub"] = sub;
	insType["subu"] = subu;
	insType["mul"] = mul;
	insType["mulu"] = mulu;
	insType["div"] = Div;
	insType["divu"] = divu;
	insType["xor"] = Xor;
	insType["xoru"] = xoru;
	insType["neg"] = neg;
	insType["negu"] = negu;
	insType["rem"] = rem;
	insType["remu"] = remu;
	insType["li"] = li;
	insType["seq"] = seq;
	insType["sge"] = sge;
	insType["sgt"] = sgt;
	insType["sle"] = sle;
	insType["slt"] = slt;
	insType["sne"] = sne;
	insType["b"] = b;
	insType["beq"] = beq;
	insType["bne"] = bne;
	insType["bge"] = bge;
	insType["ble"] = ble;
	insType["bgt"] = bgt;
	insType["blt"] = blt;
	insType["beqz"] = beqz;
	insType["bnez"] = bnez;
	insType["blez"] = blez;
	insType["bgez"] = bgez;
	insType["bgtz"] = bgtz;
	insType["bltz"] = bltz;
	insType["j"] = j;
	insType["jr"] = jr;
	insType["jal"] = jal;
	insType["jalr"] = jalr;
	insType["la"] = la;
	insType["lb"] = lb;
	insType["lh"] = lh;
	insType["lw"] = lw;
	insType["sb"] = sb;
	insType["sh"] = sh;
	insType["sw"] = sw;
	insType["move"] = Move;
	insType["mfhi"] = mfhi;
	insType["mflo"] = mflo;
	insType["nop"] = nop;
	insType["syscall"] = syscall;
}

inline command parse(string &s)
{
	tokenscanner ts;
	ts.set(s);
	command ret;
	ret.type = insType[ts.nextToken()];
	ret.Size = 0;
	while (ts.hasMoreTokens()) ret.v[ret.Size++] = ts.nextToken();
	return ret;
}

inline void dataExcute(command &CMD)
{
	switch (CMD.type) {
	case Align: {
		int n = to_int(CMD.v[0]);
		n = pow(2, n);
		heapLen = ((heapLen - 1) / n + 1) * n;
		break;
	}
	case ascii: {
		string str = CMD.v[0];
		int len = str.size();
		for (int i = 0; i < len; ++i) Memory[heapLen++] = str[i];
		break;
	}
	case asciiz: {
		string str = CMD.v[0];
		int len = str.size();
		for (int i = 0; i < len; ++i) Memory[heapLen++] = str[i];
		Memory[heapLen++] = '\0';
		break;
	}
	case byte: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 1; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case half: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 2; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case word: {
		int len = CMD.Size;
		for (int i = 0; i < len; ++i) {
			int num = to_int(CMD.v[i]);
			for (int j = 0; j < 4; ++j) {
				Memory[heapLen++] = num & 255;
				num >>= 8;
			}
		}
		break;
	}
	case space: {
		heapLen += to_int(CMD.v[0]);
		break;
	}
	}
}

inline void IF()
{
	++conti;
	if (line.empty() && conti >= 0) {
		line.push(reg[pc]);
		if (CMD[reg[pc]].type >= 30 && CMD[reg[pc]].type <= 46) conti = -5;
		++reg[pc];
	}
}

inline void IDDP()
{
	if (!line.empty()) {
		comm.push(CMD[line.front()]);
		line.pop();
	}
}

inline void WB();

inline void E()
{
	if (!comm.empty()) {
		COMMAND = comm.front();
		switch (COMMAND.type) {
		case add:
		case addu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (COMMAND.regiSize == 3) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp + reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp + COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case addiu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			Mem.result = tmp + COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case sub:
		case subu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (COMMAND.regiSize == 3) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp - reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp - COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case mul:
		case mulu: {
			Mem.Itype = COMMAND.type;
			if (COMMAND.regiSize + COMMAND.flag == 3) {
				Mem.MUL = false;
				Mem.Rtype = COMMAND.regi[0];
				if (COMMAND.regi[1] == preReg) WB();
				int tmp = reg[COMMAND.regi[1]];
				if (!COMMAND.flag) {
					if (COMMAND.regi[2] == preReg) WB();
					Mem.result = tmp * reg[COMMAND.regi[2]];
				}
				else Mem.result = tmp * COMMAND.num;
				preReg = Mem.Rtype;
				hReg = false;
			}
			else {
				Mem.MUL = true;
				if (!COMMAND.flag) {
					if (COMMAND.regi[0] == preReg) WB();
					if (COMMAND.regi[1] == preReg) WB();
					long long tmp = reg[COMMAND.regi[0]] * reg[COMMAND.regi[1]];
					Mem.low = tmp;
					Mem.high = tmp >> 32;
					preReg = lo;
					hReg = true;
				}
				else {
					if (COMMAND.regi[0] == preReg) WB();
					long long tmp = reg[COMMAND.regi[0]] * COMMAND.num;
					Mem.low = tmp;
					Mem.high = tmp >> 32;
					preReg = lo;
					hReg = true;
				}
			}
			break;
		}
		case Div:
		case divu: {
			Mem.Itype = COMMAND.type;
			if (COMMAND.regiSize + COMMAND.flag == 3) {
				Mem.MUL = false;
				Mem.Rtype = COMMAND.regi[0];
				if (COMMAND.regi[1] == preReg) WB();
				int tmp = reg[COMMAND.regi[1]];
				if (!COMMAND.flag) {
					if (COMMAND.regi[2] == preReg) WB();
					Mem.result = tmp / reg[COMMAND.regi[2]];
				}
				else Mem.result = tmp / COMMAND.num;
				preReg = Mem.Rtype;
				hReg = false;
			}
			else {
				Mem.MUL = true;
				if (!COMMAND.flag) {
					if (COMMAND.regi[0] == preReg) WB();
					if (COMMAND.regi[1] == preReg) WB();
					int tmp1 = reg[COMMAND.regi[0]], tmp2 = reg[COMMAND.regi[1]];
					Mem.low = tmp1 / tmp2;
					Mem.high = tmp1 % tmp2;
					preReg = lo;
					hReg = true;
				}
				else {
					if (COMMAND.regi[1] == preReg) WB();
					int tmp1 = reg[COMMAND.regi[0]], tmp2 = COMMAND.num;
					Mem.low = tmp1 / tmp2;
					Mem.high = tmp1 % tmp2;
					preReg = lo;
					hReg = true;
				}
			}
			break;
		}
		case Xor:
		case xoru: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp ^ reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp ^ COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case neg: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.result = -reg[COMMAND.regi[1]];
			}
			else Mem.result = -COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case negu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) {
			if (COMMAND.regi[1] == preReg) WB();
				Mem.result = ~reg[COMMAND.regi[1]];
			}
			else Mem.result = ~COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case rem:
		case remu: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp % reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp % COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case li: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			Mem.result = COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case la: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (!COMMAND.flag) Mem.result = COMMAND.Lable;
			else Mem.result = COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case seq: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp == reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp == COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case sge: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp >= reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp >= COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case sgt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp > reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp > COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case sle: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp <= reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp <= COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case slt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp < reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp < COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case sne: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			int tmp = reg[COMMAND.regi[1]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[2] == preReg) WB();
				Mem.result = tmp != reg[COMMAND.regi[2]];
			}
			else Mem.result = tmp != COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case b:
		case j: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case beq: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp == reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp == COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bne: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp != reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp != COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bge: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp >= reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp >= COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case ble: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp <= reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp <= COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bgt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp > reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp > COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case blt: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			int tmp = reg[COMMAND.regi[0]];
			if (!COMMAND.flag) {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.MUL = tmp < reg[COMMAND.regi[1]];
			}
			else Mem.MUL = tmp < COMMAND.num;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case beqz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] == 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bnez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] != 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case blez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] <= 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bgez: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] >= 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bgtz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] > 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case bltz: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.MUL = reg[COMMAND.regi[0]] < 0;
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case jr: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.result = reg[COMMAND.regi[0]];
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case jal: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			Mem.result = COMMAND.Lable;
			Mem.low = reg[pc];
			preReg = ra;
			hReg = false;
			break;
		}
		case jalr: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = pc;
			if (COMMAND.regi[0] == preReg) WB();
			Mem.result = reg[COMMAND.regi[0]];
			Mem.low = reg[pc];
			preReg = ra;
			hReg = false;
			break;
		}
		case lb:
		case lh:
		case lw:
		case sb:
		case sh:
		case sw: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regiSize == 1) Mem.pos = COMMAND.Lable;
			else {
				if (COMMAND.regi[1] == preReg) WB();
				Mem.pos = COMMAND.num + reg[COMMAND.regi[1]];
			}
			if (Mem.Itype == lb || Mem.Itype == lh || Mem.Itype == lw) preReg = Mem.Rtype;
			else preReg = none;
			hReg = false;
			break;
		}
		case Move: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (COMMAND.regi[1] == preReg) WB();
			Mem.result = reg[COMMAND.regi[1]];
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case mfhi: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (hReg) WB();
			Mem.result = reg[hi];
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case mflo: {
			Mem.Itype = COMMAND.type;
			Mem.Rtype = COMMAND.regi[0];
			if (lo == preReg) WB();
			Mem.result = reg[lo];
			preReg = Mem.Rtype;
			hReg = false;
			break;
		}
		case nop: {
			Mem.Itype = COMMAND.type;
			preReg = none;
			hReg = false;
			break;
		}
		case syscall: {
			Mem.Itype = COMMAND.type;
			Mem.MUL = false;
			if (v0 == preReg) WB();
			hReg = false;
			switch (reg[v0]) {
			case 1: {
				if (a0 == preReg) WB();
				printf("%d", reg[a0]);
				Mem.low = 1;
				preReg = none;
				break;
			}
			case 4: {
				if (a0 == preReg) WB();
				for (int i = reg[a0]; Memory[i] != '\0'; ++i) printf("%c", Memory[i]);
				Mem.low = 4;
				preReg = none;
				break;
			}
			case 5: {
				int tmp;
				scanf("%d", &tmp);
				Mem.Rtype = v0;
				Mem.result = tmp;
				Mem.low = 5;
				preReg = v0;
				break;
			}
			case 8: {
				char c = getchar();
				if (c == '\n') c = getchar();
				if (a0 == preReg) WB();
				int i = reg[a0];
				while (c != '\0' && c != '\n' && c != -1) {
					Memory[i++] = c;
					c = getchar();
				}
				Memory[i++] = '\0';
				Mem.Rtype = a1;
				Mem.result = i - reg[a0];
				Mem.low = 8;
				preReg = a1;
				break;
			}
			case 9: {
				Mem.Rtype = v0;
				Mem.result = heapLen;
				Mem.low = 9;
				if (a0 == preReg) WB();
				heapLen += reg[a0];
				preReg = v0;
				break;
			}
			case 10:
			case 17: {
				Mem.MUL = true;
				Mem.low = 17;
				preReg = none;
				break;
			}
			}
			break;
		}
		default: {
			Mem.Itype = nop;
			preReg = none;
			hReg = false;
			break;
		}
		}
		memo.push(Mem);
		comm.pop();
	}
}

inline void MA()
{
	if (!memo.empty()) {
		Mem = memo.front();
		switch (Mem.Itype) {
		case lb: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos];
			break;
		}
		case lh: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos] | Memory[Mem.pos + 1] << 8;
			break;
		}
		case lw: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.result = Memory[Mem.pos] | Memory[Mem.pos + 1] << 8 | Memory[Mem.pos + 2] << 16 | Memory[Mem.pos + 3] << 24;
			break;
		}
		case sb: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 1; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case sh: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 2; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case sw: {
			int num = reg[Mem.Rtype];
			for (int i = 0; i < 4; ++i) {
				if (Mem.MUL) Memory[Mem.pos + i] = num & 255;
				else Memory[Mem.pos + i] = num & 255;
				num >>= 8;
			}
			Reg.Itype = Mem.Itype;
			break;
		}
		case nop: {
			Reg.Itype = Mem.Itype;
			break;
		}
		default: {
			Reg.Itype = Mem.Itype;
			Reg.Rtype = Mem.Rtype;
			Reg.MUL = Mem.MUL;
			Reg.result = Mem.result;
			Reg.low = Mem.low;
			Reg.high = Mem.high;
			break;
		}
		}
		regi.push(Reg);
		memo.pop();
	}
}

inline void WB()
{
	if (!regi.empty()) {
		Reg = regi.front();
		switch (Reg.Itype) {
		case add:
		case addu:
		case addiu:
		case sub:
		case subu: {
			reg[Reg.Rtype] = Reg.result;;
			break;
		}
		case mul:
		case mulu:
		case Div:
		case divu: {
			if (Reg.MUL) {
				reg[lo] = Reg.low;
				reg[hi] = Reg.high;
			}
			else reg[Reg.Rtype] = Reg.result;
			break;
		}
		case Xor:
		case xoru:
		case neg:
		case negu:
		case rem:
		case remu:
		case li:
		case seq:
		case sge:
		case sgt:
		case sle:
		case slt:
		case sne:
		case la:
		case lb:
		case lh:
		case lw:
		case Move:
		case mfhi:
		case mflo: {
			reg[Reg.Rtype] = Reg.result;
			break;
		}
		case b:
		case j:
		case jr: {
			reg[pc] = Reg.result;
			break;
		}
		case beq:
		case bne:
		case bge:
		case ble:
		case bgt:
		case blt:
		case beqz:
		case bnez:
		case blez:
		case bgez:
		case bgtz:
		case bltz: {
			if (Reg.MUL) reg[pc] = Reg.result;
			break;
		}
		case jal:
		case jalr: {
			reg[pc] = Reg.result;
			reg[ra] = Reg.low;
			break;
		}
		case syscall: {
			switch (Reg.low) {
			case 5:
			case 8:
			case 9: {
				reg[Reg.Rtype] = Reg.result;
				break;
			}
			case 10:
			case 17: {
				if (Mem.MUL) shutdown = true;
				Reg.Itype = Mem.Itype;
				break;
			}
			}
			break;
		}
		default:
			break;
		}
		regi.pop();
	}
}

#endif