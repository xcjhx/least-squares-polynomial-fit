# least-squares-polynomial-fit
最小二乘法拟合多项式的c语言实现

## 注意事项

1.拟合数据以及对应数组需使用double，不然精度会有损失，导致拟合结果与matlab的polyfit函数有所偏差；

2.该工程在单片机上运行，输入的x，y数据最好在小数点前两位以内，否则拟合阶数一高容易导致数据溢出；