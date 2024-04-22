#include<stdio.h>


#define POLY 0x04c11db7 
#define INIT 0xffffffff 
#define XOROUT 0xffffffff 



unsigned int reverse(unsigned int input)
{
	unsigned int output = 0;
	for (unsigned int i = 1; i != 0; i <<= 1)
	{
		output <<= 1;
		if (input & 1)
		{
			output |= 1;
		}
		input >>= 1;
	}
	return output;
}



unsigned int crc32(unsigned char* addr, unsigned int num)
{
	unsigned int crc = INIT;
	while (num-- > 0)
	{
		crc ^= reverse(*addr++);
		for (int i = 0; i < 8; i++)
		{
			if (crc & 0x80000000)
			{
				crc = (crc << 1) ^ POLY;
			}
			else
			{
				crc <<= 1;
			}
		}
		crc &= 0xffffffff;
	}
	return reverse(crc ^ XOROUT);
}


int main()
{
	unsigned char input[] = { '1', '1', '4', '5', '1', '4', '6', '6', '6' };
	printf("check=0x%08x\n", crc32(input, sizeof(input)));
}
