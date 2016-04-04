/*
ucode 인터프리터가 인자 전달을 처리하기 위한 방법중
call by address 에 대한 처리방법
*/
void abca(int &n)
{
	n = 10;
}

void abcb(int &n)
{
	write(n);
}

void main()
{
	int n;
	abca(n);
	abcb(n);
}

