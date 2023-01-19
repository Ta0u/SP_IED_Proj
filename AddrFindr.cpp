#include <iostream>

int il = 0 , yl = 0;
int AddrList[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
int mohg = 7;

int main()
{
    for (int il = 0; il < 3; il++)
	{
		for (int yl = 0; yl < 3; yl++)
		{
			if (AddrList[il][yl] != mohg)
			{}
			else 
			{
				    std :: cout << il<< ","  << yl << std :: endl;
					return 0;
			}
			
		}
	}
}