# LiquidHazard

基于 [Google Liquidfun](https://github.com/google/liquidfun) 的 OOP 大作业 **`LiquidHazard`**

作业与文档编写时使用环境为：`Windows 10` + `VScode` + `Visual Studio`

## 编译运行指南

用Visual Studio打开.sln文件，将testbed设置为启动首选项后调试

## 基础要素介绍

游戏一开始生成一个 `Player`（横长条版） 、一个`Body`（弹球）之后生成随机数个 `Obstacle`（游戏界面中浮在空中的砖块）。另外还有两类`Particle`（红色软球与蓝色海洋）可视界面外有绿色围墙限制游戏区域。

### `Player`

默认情况下，控制键位如下：

| 控制按键 | 向左移动 | 向右移动 | 
| :------: | :----: | :----: | 
| Player  |   A    |   D    | 

游戏目标为在弹球触碰到红色软球或是蓝色海洋之前尽可能多的击碎砖块，砖块会随时间重新生成，并且颜色越深分数越高。

`Tips: 利用横长条版左右两侧击打弹球可获得更高的速度`

### `Obstacle`

初始时屏幕上无`ObStacle`，之后每间隔一定时间会在可视区域上方指定区域内随机位置生成 `Obstacle`。
