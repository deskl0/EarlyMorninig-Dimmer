# EarlyMorninig-Dimmer
Automatic light alarm/自动灯光唤醒
<H3>This is an automatic dimmer for morning alarm clock.And a typical Arduino multi-Thread example. Completed with Arduino scoop library.<br>这是一个自动的清晨灯光闹钟, 也是一个典型的Arduino多线程实例. 使用Ardiono scoop 库实现.
  </H3>
<br>Currenty I have two versions of it. The released one is based on Atmaga32u4 with arduino Leonardo. Another one is based on STM32F1, and it is undergoing.<br>
目前有两个版本. 发布的这一个基于Atmega32u4 使用Arduino Leonardo. 另一个基于STM32F1的版本正在制作.
<H2>Usage</H2>
<br> User interface is a 0.91' OLED and an rotatry encoder.
<br> 用户界面是一个0.91英寸的OLED屏幕和一个旋转编码器
<H3>Encoder Behaviour<br>编码器行为</H3>
1.  Single Click: Move cursor to the next position<br>
1.  单击: 移动光标至下一位置<br>
2.  Long Click: Step into next configure stage.<br>
2.  长点击: 进入下一个配置阶段<br>
3.  Rotate: Change value. CW: Increase. CCW: Decrease.<br>
3.  旋转: 顺时针:增加. 逆时针:减少<br>
<H3>Screen Behaviour<br>屏幕行为</H3>
Information feedback only.<br>仅作信息反馈
<H3>System Configuration Stages<br>系统配置阶段</H3>
0.  Show Current Time, Date and Temprature<br>
0.  显示当前时间,日期和温度<br>
1.  Set start time<br>
1.  设置起始时间<br>
2.  Set duration and steps.(The LED you connected will be lighted gradually)<br>
2.  设置持续时间和步进(你接入的LED灯将会逐渐被点亮)<br>
3.  Set Portable power bank automatic shutdown time. (If the power source won't deactive automatically, configure it to maximum value)<br>
3.  设置移动电源自动断电时间(如果不会自动关闭, 配置它到最大值)<br>
4.  Set onboard LED1 RGB<br>
4.  设置板载LED1 RGB<br>
5.  Set onboard LED2 RGB<br>
5.  设置板载LED2 RGB<br>
6.  System actived. Screen off, LED off, PWM standby.<br>
6.  系统启动. 屏幕关闭, LED关闭, PWM就绪.
<H6>Onboard LEDs will start blinking at 2 mins before the start time<br>板载LED将会在开始时间前两分钟闪烁</H6>
<H2>If you disconnected the battery of RTC, you have to re-program the RTC!<br> Power deception current is 500mA, active periodly, depend on system config stage 3(minimal value is 20s). For each deceptive current actives, it last for 120ms.<br>如果你断开了RTC和电池, 你必须重新编程RTC!<br>电源欺骗电流500mA, 周期性启动, 取决于系统配置第三阶段(最小值20s). 对于每个欺骗电流启动, 持续120ms
</H2>
