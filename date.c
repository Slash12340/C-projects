/*Assignment 1: Dates*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Create variables*/
int dFeb;

/*Initialize function*/
int dateInput(int i, int d[], int m[], int y[]);
int dateDiff(int d[], int m[], int y[], int n, int dTotal[], int mDays[], int yDays[]);				
int bubbleSort(int dTotal[], int n);
int dateSort(int d[], int m[], int y[], int dTotal[], int mDays[], int yDays[], int n);

main()
{
	int i, n;
	scanf("%i", &n);				/*Number of dates input*/
	int d[n], m[n], y[n], dTotal[n], mDays[n], yDays[n];
	
	/*Initialize the array with 0 elements*/
	memset(mDays, 0, n * sizeof(int));
	memset(yDays, 0, n * sizeof(int));
	
	for (i=0; i<n; i++)
	{
		dateInput(i, d, m, y);
	}
	
	dateDiff(d, m, y, n, dTotal, mDays, yDays);
	
	dateSort(d, m, y, dTotal, mDays, yDays, n);
}



int dateInput(int i, int d[], int m[], int y[])
{
	char x, z;	

	/*Input*/
	scanf("%i%c%i%c%i", &d[i], &x, &m[i], &z, &y[i]);
	
	/*Check for valid year*/
	while (y[i] > 10000 || y[i] < 1)
	{
		fprintf(stderr, "re-enter date: ");
		scanf("%i%c%i%c%i", &d[i], &x, &m[i], &z, &y[i]);
	}
	

	/*Check for leap year*/
	if (y[i] % 4 == 0)
	{
		dFeb = 29;
	}
	else
	{
		dFeb = 28;
	}
	
	/*Check if date, month and year are within limits*/
	while (((m[i]==1 || m[i]==3 || m[i]==5 || m[i]==7 || m[i]==8 || m[i]==10 || m[i]==12) && d[i] > 31)	/*Months with 31 days*/
		|| ((m[i]==4 || m[i]==6 || m[i]==9 || m[i]==11) && d[i] > 30)									/*Months with 30 days*/
		|| ((m[i]==2) && d[i] > dFeb)																	/*Feburary*/
		|| (d[i] < 1) || (m[i] > 12 || m[i] < 1)														/*Check for valid dates*/	
		|| (x != '-' && x != '/') || (z != '-' && z != '/') 											/*Format check*/
		|| (z == '-' && x == '/') || (z == '/' && x == '-'))
	{
		fflush(stdin);
		fprintf(stderr, "re-enter date: ");
		scanf("%i%c%i%c%i", &d[i], &x, &m[i], &z, &y[i]);
	}
}

int dateSort(int d[], int m[], int y[], int dTotal[], int mDays[], int yDays[], int n)
{
	int i, j;
	
	bubbleSort(dTotal, n);
	
	/*Loop through sorted days and print out the corresponding dates*/
	for (j = 0; j < n; j++)
	{
		for (i = 0; i < n; i++)
		{
			if (dTotal[j] == d[i] + mDays[i] + yDays[i])
			{
				printf("%i/%i/%i\n", d[i], m[i], y[i]); 
			}
		}
	}
}

int dateDiff(int d[], int m[], int y[], int n, int dTotal[], int mDays[], int yDays[])
{
	int months[12] = {31, dFeb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int i, t, x;
	
	/*Calculate the days in date*/
	for (t = 0; t<n; t++)
	{
		for (i=0; i < m[t]; i++)
		{
			mDays[t] += months[i];
		}
		
		yDays[t] = (y[t] * 365);
		dTotal[t] = d[t] + mDays[t] + yDays[t];  							/*Total days in date*/
	}
}

int bubbleSort(int dTotal[], int n)
{
	int i, sorted = 0, temp;
	
	while (!sorted)
	{
		for (i = 1, sorted =1; i < n; i++)
		{
			if (dTotal[i-1] > dTotal[i])
			{
				sorted = 0;
				temp = dTotal[i-1];
				dTotal[i-1] = dTotal[i];
				dTotal[i] = temp;
			}
		}
	}
}



