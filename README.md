# 2024华为软件精英挑战赛
正式赛得分: 375067

# 机器人
## 寻路
算法: A*算法
启发函数: f = h + g
h = abs(x1 - x2) + abs(y1 - y2)
g = 机器人走过步数

## 避障
机器人使用A*算法生成路径时，对于child.g = g将以下类型的点视为障碍
1. 已生成路径的机器人中，path[index + g]的点。即已有路径的机器人后g帧所在位置
2. 已生成路径的机器人中，path[index + g - 1]的点，避免对撞的情况
3. 对于没有路径的机器人，将机器人位置视作障碍

## 机器人选择货物
选择价值 / 距离最大的货物
即 cargo->val / dist 最大的货物
由于准确距离只能通过寻路得到，这里的距离使用估算距离
即dist = abs(x1 - x2) + abs(y1 - y2)
## 机器人选择泊位
直接选择最近的泊位，同样只能使用估算距离
# 船
## 船选择泊位
每个泊位维护一个selected_ship_num, 记录已经选择这个泊位的船只数量
通过这个变量，船只在选择泊位时可以估算其到达这个泊位时，能够获得的货物价值

其他船只拿走的货物数量k = 已经选择这个泊位的船只数量 * 船只容量
估计价值 = 当前泊位货物总价 - 泊位前k个货物价值

船只选择估计价值最大的泊位即可