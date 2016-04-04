/*
Queue 자료구조를 구현하여 테스트
*/
int f, r;
void push(int q[], int i)
{
	q[f] = i;
	f++;
}

int pop(int q[])
{
	int data;
	data = q[r];
	r++;
	return data;
}

void main()
{
	int q[100], i, data;
	f = 0;
	r = 0;
	i = 0;
	while (i < 10)
	{
		push(q, i);
		i++;
	}

	i = 0;
	while (i < 10)
	{
		data = pop(q);
		write(data);
		i++;
	}
}