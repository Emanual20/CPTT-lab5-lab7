/*
	I'm level 1 test. without pointer.
*/
//struct Matrix{
//	int id;
//	int arr[10][10];
//} m1,m2,m3;
const int len = 10;
int t = 0x21;
int t = 077;
int len = t - 0x15 + 077;
void main(){
	//m1.id = 1, m2.id = 2, m3.id=3;
	for(int i=0;i<len;i+=1){
		for(int j=0;j<len;j+=1){
			//m1.arr[i][j]=i;
			//m2.arr[i][j]=j;
			//m3.arr[i][j]=m1.arr[i][j]+m2.arr[i][j];
			;
		}
	}

	for(int i=0;i<len;i+=1){
		for(int j=0;j<len;j+=1){
			//printf("<%d>[%d][%d] %d\n",m3.id,i,j,m3.arr[i][j]);
			;
		}
		//printf("\n");
	}
}