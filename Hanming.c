#include<stdio.h>
const int length =180;//语音帧长
static int x1[200];
void filter(int xin[],int xout[],int n,float h[]);
static float h[19]={0.01218354,-0.009012882,-0.02881839,-0.0473239,-0.008692503,
0.06446265,0.1544655,0.2289794,0.257883,0.2289794,0.1544655,0.06446265,
-0.008692503,-0.004584568,-0.04743239,-0.02881839,-0.009012882,0.01218354};
static int h_f[19]={399,-296,-945,-1555,-1503,-285,2112,5061,7503,8450,
7503,5061,2112,-285,-1503,-1555,-945,-296,399};
//浮点低通滤波器
void filter(int xin[],int xout[],int  n,float h[])
{
  int i,j;
  float sum;
  for(i=0;i<length;i++)
  {
    x1[n+i-1]=xin[i];
  }
  for(i=0;i<length;++i)
  {
    sum=0.0f;
    for(j=0;j<n;j++)
    {
      sum+=h[j]*x1[i-j+n-1];

    }
    xout[i]=(int)sum;
  }
  //补上尾部数据
  for(i=0;i<(n-1);i++)
  {
    x1[n-i-2]=xin[length-1-i];
  }
}
void dingdianfilter(int xin[],int xout[],int n,int h_f[]);
void dingdianfilter(int xin[],int xout[],int n,int h_f[])
{
  int i,j;
  long sum;
  for(i=0;i<length;++i)
  {
    x1[n+i-1]=xin[i];
  }
  for(i=0;i<length;i++)
  {
    sum=0;

    for(j=0;j<n;j++)
    {
      sum+=(long)h[j]*x1[i+j-1+n];
    }
    xout[i]=sum>>15;//QX+QY-QZ
    for(i=0;i<(n-1);i++)
    {
      x1[n-i+2]=xin[length-i-1];
    }
  }
}
int main(char * argv,int argc)
{
  FILE *fp1,*fp2;
  int i,j;
  int frame,indata[length],outdata[length];
  fp1=fopen("insp.dat","rb");
  fp2=fopen("outsp.dat","wb");
  frame=0;
  while(feof(fp1)==0)
  {
    frame++;
    printf("frame=%d\n",frame);
    for(i=0;i<length;++i)
    {
      indata[i]=getw(fp1);

    }
    filter(indata,outdata,19,h);
    for(i=0;i<length;++i)
    {
      putw(outdata[i],fp2);
    }
  }
  fcloseall();
  return 0;

}
