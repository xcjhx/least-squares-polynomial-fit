/*********************************************************************************************************
* 模块名称: PolynomialFit.c
* 摘    要: 多项式拟合
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2019 SZLY. All rights reserved.)
* 完成日期: 2019年08月01日
* 内    容: 
* 注    意: 
**********************************************************************************************************
* 取代版本: 
* 作    者: 
* 完成日期: 
* 修改内容: 
* 修改文件: 
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "PolynomialFit.h"
#include "stdio.h"
#include <math.h>

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define OLDER_MAX 10  //不宜太大，否则SRAM没有足够的内存使用
#define ACCURACY 10    //精度

#define ROW 8
#define COLUMN 9
/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
double b[15];
double zz[ROW][COLUMN] = {0};
double fz[ROW] = { 0 };
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: CalcPolynomial
* 函数功能: 计算多项式结果
* 输入参数: a-多项式系数，按照升幂排序，x-自变量，older-多项式阶数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注		意:
*********************************************************************************************************/
double CalcPolynomial(double* a, double x, unsigned char older)
{
  double result = 0;
  int i = 0;
  b[0] = 1;
  //求x的N次幂
  for (i = 1; i <= older; i++)
  {
    b[i] = b[i - 1] * x;
  }

  //求多项式结果
  for (i = 0; i <= older; i++)
  {
    result += a[i] * b[i];
  }

  return result;
}

/*********************************************************************************************************
* 函数名称: CalcPolyIntegral
* 函数功能: 计算多项式积分
* 输入参数: a-多项式系数，按照升幂排序，x1、x2-积分区域，older-多项式阶数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注		意: 最大支持199次幂
*********************************************************************************************************/
double CalcPolyIntegral(double* a, double x1, double x2, unsigned char older)
{
  double i = 0;
  double y1;
  double y2;
  double result;

  //求原函数系数
  b[0] = 0;
  for (i = 1; i <= (double)older + 1; i = i + 1)
  {
    b[(int)i] = a[(int)(i - 1)] / i;
  }

  //求解积分
  y1 = CalcPolynomial(b, x1, older + 1);
  y2 = CalcPolynomial(b, x2, older + 1);
  result = y2 - y1;

  return result;
}

/*********************************************************************************************************
* 函数名称: FindMaxInPolynomial
* 函数功能: 查找多项式最大值位置
* 输入参数: a-多项式系数，按照升幂排序，x1、x2-查找区域，older-多项式阶数，gap-查找间距
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注		意: 
*********************************************************************************************************/
double FindMaxInPolynomial(double* a, double x1, double x2, unsigned char older, double gap)
{
  double maxIndex = 0;  //最大值所处位置
  double maxOld = 0;
  double maxNew = 0;
  double i = 0;
  double temp;

  //从小到大查找
  if (x1 > x2)
  {
    temp = x1;
    x1 = x2,
      x2 = temp;
  }

  for (i = x1; i <= x2; i = i + gap)
  {
    maxNew = CalcPolynomial(a, i, older);

    //查找最大值
    if (maxNew > maxOld)
    {
      maxOld = maxNew;
      maxIndex = i;
    }
  }
  
  return maxIndex;
}

/*********************************************************************************************************
* 函数名称: FindFirstMaxInPolynomial
* 函数功能: 查找曲线第一个峰值
* 输入参数: a-多项式系数，按照升幂排序，x1、x2-查找区域，older-多项式阶数，gap-查找起始位置
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注		意:
*********************************************************************************************************/
double FindFirstMaxInPolynomial(double* a, double x1, double x2, unsigned char older, double gap)
{
  double i = 0;
  double temp;

  double pre, now, next;

  //从小到大查找
  if (x1 > x2)
  {
    temp = x1;
    x1 = x2,
    x2 = temp;
  }

  for (i = x1 + gap; i <= x2; i = i + gap)
  {
	pre = CalcPolynomial(a, i-gap, older);
	now = CalcPolynomial(a, i, older);
	next = CalcPolynomial(a, i + gap, older);
//	printf("\r\n");
//	printf("i = %f maxNew = %f\r\n", i,now);
	if (now > pre && now > next)
	{
		break;
	}
  }
  return i;
}
/*********************************************************************************************************
* 函数名称: Polynomia_y_to_x
* 函数功能: 
* 输入参数: a-多项式系数，按照升幂排序，x1-左边界，x2-右边界，older-多项式阶数，y,gap-步长，pre-误差范围
* 输出参数: void
* 返 回 值: x
* 创建日期: 2018年01月01日
* 注		意: outPut按照升序排序，每次拟合之前outPut缓冲区应初始化为0，否则拟合会出现异常
*********************************************************************************************************/
double Polynomia_y_to_x(double* a, float x1, float x2,unsigned char older,double y,float gap,float pre)
{
  double i = 0;
  double temp_x;
  
  if (x1 < x2)
  {
    for (i = x1; i <= x2; i = i + gap)
    {
      if(CalcPolynomial(a, i, older) >= y - pre && CalcPolynomial(a, i, older)<= y + pre)
      {
        break;
      }
    }
  }
  else
  {
    for (i = x1; i >= x2; i = i - gap)
    {
      if(CalcPolynomial(a, i, older) >= y - pre && CalcPolynomial(a, i, older)<= y + pre)
      {
        break;
      }
    }
  }
  
  return i;
}

/*********************************************************************************************************
* 函数名称: PolynomialFit
* 函数功能: 最小二乘法拟合多项式
* 输入参数: x-数据自变量
*           y-数据因变量
*           outPut-输出系数，按照升幂排序
*           maxn-拟合数据量
*           older-拟合阶数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注		意: outPut按照升序排序，每次拟合之前outPut缓冲区应初始化为0，否则拟合会出现异常
*********************************************************************************************************/
void PolynomialFit(double* x, double* y, double* output, int maxn, unsigned char older)
{
  int i;
	int j;
	int k;
	int n;
	double temp = 0;
  
  double m = 0;
	double max = 0;
	int m_max;
  
  int s_row = older+1;
  int s_col = older+2;
  
  //清空缓存区
  for(i = 0;i<ROW;i++)
  {
    fz[i] = 0;
    for(j = 0;j<COLUMN;j++)
    {
      zz[i][j] = 0;
    }
  }
  //建立法方程组
	for (i = 0; i < maxn; i++)
	{
		for (j = 0; j < s_row; j++)
		{
			for (k = 0; k < s_row; k++)
			{
				temp = 1;
				for (n = 0; n < j + k; n++)
				{
					temp = temp*x[i];
				}
				zz[j][k] += temp;
			}
			temp = y[i];
			for (n = 0; n < j; n++)
			{
				temp = temp * x[i];
			}
			fz[j] += temp;
		}
	}
	for (j = 0; j < s_row; j++)
	{
		zz[j][s_col-1] = fz[j];
	}
  
  //从左到右按列遍历
	for (j = 0; j < s_col - 2; j++)
	{
		//寻找列主元素（从上到下，行遍历，找列中最大的）
		max = zz[j][j];
		m_max = 0;
		for (k = j; k < s_row; k++)
		{
			if (zz[k][j] > max)
			{
				max = zz[k][j];
				m_max = k;//记录最大的所在行
			}
		}
		//换行,(列遍历)
		for (k = 0; k < s_col; k++)
		{
			temp = zz[m_max][k];
			zz[m_max][k] = zz[j][k];
			zz[j][k] = temp;
		}
		//从上到下，行消去
		for (i = j + 1; i <  s_row; i++)
		{
			
			m = zz[i][j] / zz[j][j];
			for (k = j; k < s_col; k++)
			{
				zz[i][k] = zz[i][k] - m * zz[j][k];
			}
		}
	}
  
  //回代求解
	//从下往上逐个求解
	for (i = s_row - 1; i >= 0; i--)
	{
		temp = zz[i][s_col-1];
		for (j = s_col - 2; j > i;j--)
		{
			temp -= zz[i][j] * output[j];
		}
		output[j] = temp / zz[i][j];
	}
  
  printf("\r\n");
	printf("work\r\n");
	for (i = 0; i < s_row; i++)
	{
		printf("%+f.*x.^%d", output[i], i);
	}
  printf("\r\n");
}

///*********************************************************************************************************
//* 函数名称: GaussianFit
//* 函数功能: 高斯拟合
//* 输入参数: x-数据自变量
//*           y-数据因变量
//*           maxn-拟合数据量
//*           x_max-峰值横坐标
//*           y_max-峰值纵坐标
//*           s-半峰值
//* 输出参数: void
//* 返 回 值: void
//* 创建日期: 2018年01月01日
//* 注		意: outPut按照升序排序，每次拟合之前outPut缓冲区应初始化为0，否则拟合会出现异常
//*********************************************************************************************************/
//void GaussianFit(double* x, double* y,int maxn,double* x_max,double* y_max,double* s)
//{
//  double coofe[3] = { 0 };//升幂排序
//  double y_log[maxn];
//  for (int j = 0; j < maxn; j++)
//	{
//		y_log[j] = log(y[j]);
//	}

//	PolynomialFit(x, y_log, coofe, maxn, 2);
//	
//	*s = -1/coofe[2];
//	*x_max = coofe[1] * (*s) / 2;
//	*y_max = exp(coofe[0] + (*x_max) * (*x_max) / (*s));
//}

