#include <stdio.h>
#include <math.h>

int main()
{
	double v[] = {0, 0.314159, 0.628314, 0.942464, -1.743396};
	int i;
	double deviation = 0;
	double average = 0;
	for(i = 0; i < 5; ++i)
	{
		v[i] = roundf(v[i]*100)/100;
		printf("%.2lf\n", v[i]);
	}
	for(i = 0; i < 5; ++i)
	{
		average += v[i];
	}
	average /= 5;
	printf("average: %.2lf ", average);
	for(i = 0; i < 5; ++i)
	{
		deviation += pow((v[i] - average), 2);
	}
	deviation /= 4;
	deviation = sqrt(deviation);
	printf("deviation: %.2lf\n", deviation);
	return 0;
}