#include<stdio.h>


#define POLY 0x04c11db7 
#define INIT 0xffffffff 
#define XOROUT 0xffffffff 


/*
将输入的数按位倒序
*/
unsigned int reverse(unsigned int input)
{
	unsigned int output = 0;
	for (unsigned int i = 1; i != 0; i <<= 1)//注意i与input类型一致，保证正确的循环次数
	{
		output <<= 1;//将output左移使上一次循环中确定的位变为高位，同时在本次循环中确定LSB
		if (input & 1)//根据当前input的LSB确定当前output的LSB
		{
			output |= 1;
		}
		input >>= 1;//将input右移以便在下一次循环中获取下一个高位
	}
	return output;
}


/*
按照CRC-32/ISO-HDLC算法模型计算CRC码
*/
unsigned int crc32(unsigned char* addr, unsigned int num)
{
	unsigned int crc = INIT;
	while (num-- > 0)//根据输入数据的数量依次计算
	{
		crc ^= reverse(*addr++);//因refin为真，故将输入逆向
		for (int i = 0; i < 8; i++)//对每次输入的八位数进行计算
		{
			if (crc & 0x80000000)//若首位是1则进行左移并与多项式进行异或运算
			{
				crc = (crc << 1) ^ POLY;
			}
			else//否则继续左移
			{
				crc <<= 1;
			}
		}
		crc &= 0xffffffff;//确保每次计算结果均为32位数值
	}
	return reverse(crc ^ XOROUT);//因refout为真，故将输出逆向
}


int main()
{
	unsigned char input[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	printf("check=0x%08x\n", crc32(input, sizeof(input)));
}
