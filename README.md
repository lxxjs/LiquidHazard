# LiquidHazard

基于 [Google Liquidfun](https://github.com/google/liquidfun) 的 OOP 大作业 **`LiquidHazard`**

[Google Liquidfun](https://github.com/google/liquidfun) 을 이용하여 만든 OOP 수업 프로젝트 **`LiquidHazard`** 입니다.

作业与文档编写时使用环境为：`Windows 10` + `VScode` + `Visual Studio 2019`

작업 환경: `Windows 10` + `VScode` + `Visual Studio 2019`

## 编译运行指南 실행 방법

用Visual Studio打开.sln文件，将Testbed设置为启动首选项后调试

Visual Studio로 liquidhazard.sln 파일을 실행 후, Testbed 프로젝트를 디버깅하시면 됩니다.

## 基础要素介绍 구성 요소

游戏一开始生成一个 `Player`（横长条版） 、一个`Body`（弹球）之后生成随机数个 `Obstacle`（游戏界面中浮在空中的砖块）。另外还有两类`Particle`（红色软球与蓝色海洋）可视界面外有绿色围墙限制游戏区域。

게임을 시작하면 GUI에 '플레이어'(긴 막대)와 공이 생성되고, 일정 시간마다 랜덤으로 블록이 생성됩니다. 외에 2가지 'Particle' 개체가 존재합니다(빨간색 공, 물). 초록색 벽으로 감싼 구역이 게임 구역입니다.

### `Player`

默认情况下，控制键位如下：

조종키 기본 값은 아래와 같습니다 :

| 控制按键 | 向左移动 | 向右移动 | 
| :------: | :----: | :----: | 
| Player  |   A    |   D    | 

| 조종키 | 좌로 이동 | 우로 이동 | 
| :------: | :----: | :----: | 
| Player  |   A    |   D    | 

游戏目标为在弹球触碰到红色软球或是蓝色海洋之前尽可能多的击碎砖块，砖块会随时间重新生成，并且颜色越深分数越高。

게임의 목표는 공을 튕겨 물에 빠뜨리거나 빨간색 공에 닿지 않고 최대한 많은 블록을 깨는 것입니다. 블록은 일정 시간마다 랜덤으로 생성되며, 색이 진할수록 얻을 수 있는 점수가 높습니다.

`Tips: 利用横长条版左右两侧击打弹球可获得更高的速度`
`Tips: 긴 막대(플레이어)의 모서리로 공을 튕기면 더 높이 빠르게 튕깁니다.`

### `Obstacle`

初始时屏幕上无`ObStacle`，之后每间隔一定时间会在可视区域上方指定区域内随机位置生成 `Obstacle`。

처음 시작 시 블록이 나타나지 않습니다. 이후에 일정 시간마다 플레이어 위 구역에 랜덤하게 나타납니다.
