# SKYline新版在线联飞服务器后端API文档（v1.0）

issued on : 2023-02-23 20:00

last update : 2023-02-23 11:51

last modified by : 2287



## 总体介绍

新版服务器后端处理请求以TCP / HTTPS为基础，包含Authorization、请求转发等技术

主要系统包含三部分：客户端、服务端、管制端

### 	客户端(Client Of SKYline)

​		从模拟器读取飞机信息，发送至服务端，并在必要时刻从服务端获取信息展示，是飞行员联飞的软件

		### 	服务端(SKYline Technical Server)

​		汇总数据，分发数据，使用接口处理请求与数据

### 	管制端(SKYScope)

​		用于替代EuroScope，是管制员使用的软件，要获取数据，发送对机组数据的修改，实时通信等功能

本文档将介绍服务端相应数据结构和准备、相应接口部署和操作等



## 数据结构

使用数组等数据类型存储在线机组/管制员信息

对于机组信息而言，<b>包含四个部分：user_data、flight_plan, simobject_data、temp_data</b>

其中user_data为用户账户信息，simobject_data为机组实时飞机信息、temp_data为管制员对其进行的操作等临时信息（如备注、temp_altitude等）

详细需要的数据格式如下：

```js
const clientData = {
    User_data:{
        Cid: "2287",
        Rating: 12,
        RealName: "Chunhao Jin",
        Remark: "Fly with Client Of SKYline in Technical Server",
        VoiceCapacity:"v" 
        //可选值 v :voice ; r: receive; t: text-only,
    },
    Flight_plan:{
      Callsign:"CES2287",
      Departure: "ZUCK",
      Arrival:"ZYHB",
      Route:"ZUCK DCT ZYHB",
      Alternative:"ZYMD",
      ETD:"1230",
      PlanFuel:"0435",
      FlightRule:"IFR" // 可选值 IFR VFR
    },
    Simobject_data:{
        Latitude: 40.2433,
        Longitude: 123.98712,
        Altitude: 32891,
        Speed: 361,
        Squawk: 7700,
        Pitch: 0,
        Bank: 0,
        Heading: 114,
        Aircraft: "A321",
        TailNumber:"B-7161",
        AITitle:"Airbus 321",
        Livery: "China Eastern Official"
    },
    Temp_Data:{
        ATC_Altitude:"38100",
        ATC_Speed:"",
        ATC_Heading:"",
        HandledBy:"PRC_FSS" //HandledBy 此飞机被谁接管，如无则为空字符串
        ATC_Remark:"锤子2287"
    }
}
```





​		