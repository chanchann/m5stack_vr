# m5stack_vr

m5stack + mpu9250 

https://docs.m5stack.com/#/zh_CN/arduino/arduino_home_page

http://pages.switch-science.com/letsiot/rotateM5Stack/

## 解决延迟和颤抖问题

卡尔曼滤波

rtimulib

## Introduction to VR

VR设备分为

- 主机式VR

HTC Vive、Oculus Rift、索尼PS VR。

- 移动端VR

暴风魔镜、三星Gear VR眼镜

- 一体式VR

大朋VR M2一体机、博思尼的X1。

https://zhuanlan.zhihu.com/p/23793360

## openvr

OpenVR为VR游戏提供统一的数据接口，从而使得VR游戏不用直接与厂商的SDK打交道。因此，它可独立于游戏来添加扩展新外设。也就是说假如要添加一个新手柄或者头显时，只需要修改OpenVR端即可，而不用再去改已经开发好的VR游戏，如果直接使用厂商SDK的话，则每添加一个厂商的外设就需要改一下VR游戏内的接口。

OpenVR的API都是以C++接口类方式呈现的，而且都是纯虚函数。当应用程序初始化系统时，它会返回该应用程序所使用到的SDK头文件内相匹配的接口。

可通过为使用OpenVR API为SteamVR编写一个驱动使HMD或手柄在SteamVR上正常使用。使用OpenVR开发的游戏程序都将会在硬件上正常运行，而不需要游戏开发者再另外做额外的工作。


## steamvr

SteamVR是一个runtime，它是通过Steam安装在电脑上的，OpenVR是提供API的，通过SteamVR，不同的VR系统（Unity，UE等）调用OpenVR的API来开发应用程序，还可在OpenVR中添加驱动来对接VR硬件。

SteamVR平台主要是为Vive服务的，但是它也兼容了osvr, Rift和Touch的驱动。还有些人编写了Razer Hydra的驱动来匹配旧的雷蛇手柄。

也就是说，SteamVR是依赖OpenVR的。


## openVR之十二大驱动接口

https://blog.csdn.net/dabenxiong666/article/details/68954398?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param

目前市场上有大量的VR SDK,大多数的HMD厂商都推出自己的VR平台(如oculus, psvr,deepon,3glass等),与这些特定HMD平台有所不同的是,有一些开放的VR平台,例如OSVR,steamVR.他们的目标是能够接入所有的厂家设备(各种型号的HMD、眼球追踪、控制器、定位追踪器、摄像头等等)和游戏引擎（U3D,UE4等）.从接入内容\硬件的数量和质量来看,steamVR优势明显.那么硬件和应用程序如何接入steamVR呢?那就需要通过steam开源的SDK—openVR来接入

## IServerTrackedDeviceProvider

vrserver用来查询追踪设备或关于追踪设备的一些信息,驱动必须实现的接口

IServerTrackedDeviceProvider这个接口必须实现,vrserver用来查询追踪设备或关于追踪设备的一些信息.驱动动态库必须实现它.

```cpp
HmdError Init( IDriverLog *pDriverLog, vr::IServerDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir )
```

初始化驱动.这个函数将会在所有其他方法之前被调用.如果初始化返回的不是HmdError_None,那么驱动动态库将会被卸载.

pchUserDriverConfigDir - 驱动保存用户配置文件的文件夹的绝对路径.例如:C:\Program Files (x86)\Steam\config\your_plugin_name

pchDriverInstallDir - 你的驱动的根目录的绝对路径,例如:C:\Program_Files (x86)\Steam\steamapps\common\SteamVR\drivers\your_plugin_name

- void Cleanup()

在卸载之前将驱动程序清理干净.

- uint32_t GetTrackedDeviceCount()

驱动所管理的物理上连接着的追踪设备的个数.在启动时用来初始化驱动程序的追踪设备列表.

- TrackedDeviceServerDriver *GetTrackedDeviceDriver( uint32_t unWhich )

返回单个追踪设备驱动.

- ITrackedDeviceServerDriver* FindTrackedDeviceDriver( const char *pchId )

通过串号(pchId)返回单个追踪设备驱动.

- void RunFrame()
允许驱动程序在服务器的主循环中进行一些工作,可以避免在你的驱动主线程中处理这些工作.
bool ShouldBlockStandbyMode()
如果驱动希望阻塞待机模式,返回true.
void EnterStandby()
系统进入待机模式时此函数将会被调用,驱动需要同样进入低功耗模式.
void LeaveStandby()
系统退出待机模式时会调用该函数,驱动需要切换到正常工作模式.


## IClientTrackedDeviceProvider
用来处理应用场景中各种各样的客户端操作,驱动必须实现的接口(后续会淘汰,但是目前还是要用),详细见这里.
IServerDriverHost
这个接口由vrserver提供,用来允许驱动向系统发送通知事件,详细见这里.
IClientDriverHost
这个接口由vrclient提供,允许驱动回调和查询驱动各种各样的信息,详细见这里.
IVRSettings
用来设置、获取、修改配置信息的接口,详细见这里.
ITrackedDeviceServerDriver
接口代表VR系统中的单个追踪设备,为vrserver提供获取设备姿态、控制使能、某些特殊功能、属性获取等接口,详细见这里.
IVRDisplayComponent
追踪设备显示相关的接口,详细见这里.
IVRDriverDirectModeComponent
直接渲染相关的组件(目前看来是专为oculus提供的,见详细说明),详细见这里.
IVRControllerComponent
控制器接口,详细见这里.
IVRCameraComponent
摄像头接口,详细见这里.
ICameraVideoSinkCallback
摄像头视频接口,详细见这里.
IDriverLog
驱动日志接口,详细见这里.
