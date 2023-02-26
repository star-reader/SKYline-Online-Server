# SKYline新版在线联飞服务器后端API文档（v1.0）

issued on : 2023-02-23 20:00

last update : 2023-02-23 11:51

last modified by : 2287



## 总体介绍

新版服务器后端处理请求以TCP / HTTPS为基础，包含Authorization、请求转发等技术

主要系统包含三部分：客户端、服务端、管制端

### 	客户端(Client Of SKYline)

​		从模拟器读取飞机信息，发送至服务端，并在必要时刻从服务端获取信息展示，是飞行员联飞的软件

### 服务端(SKYline Technical Server)

​		汇总数据，分发数据，使用接口处理请求与数据

### 	管制端(SKYScope)

​		用于替代EuroScope，是管制员使用的软件，要获取数据，发送对机组数据的修改，实时通信等功能

本文档将介绍服务端相应数据结构和准备、相应接口部署和操作等



## 数据结构

使用数组等数据类型存储在线机组/管制员信息

### 机组在线数据

对于机组信息而言，<b>包含四个部分：user_data、flight_plan, simobject_data、temp_data</b>

其中user_data为用户账户信息，simobject_data为机组实时飞机信息、temp_data为管制员对其进行的操作等临时信息（如备注、temp_altitude等）

详细需要的数据格式如下：

```js
const clientData = {
    isConnected: true,
    connectTime: 16554212000,
    updatedTime: 1655430800,
    //只有用户访问连接服务器的接口后才为true，因为用户需要在连线前提交飞行计划，此时应该为false
    user_data:{
        cid: "2287",
        rating: 12,
        realName: "Chunhao Jin",
        remark: "Fly with Client Of SKYline in Technical Server",
        voiceCapacity:"v" 
        //可选值 v :voice ; r: receive; t: text-only,
    },
    flight_plan:{
      callsign:"CES2287",
      departure: "ZUCK",
      arrival:"ZYHB",
      route:"ZUCK DCT ZYHB",
      alternative:"ZYMD",
      ETD:"1230",
      CruiseAltitude: "FL321",
      planFuel:"0435",
      flightRule:"IFR" // 可选值 IFR VFR
    },
    simobject_data:{
        latitude: 40.2433,
        longitude: 123.98712,
        altitude: 32891,
        speed: 361,
        squawk: 7700,
        squawkMode: 'C', // 'C' | 'S' | 'I'
        pitch: 0,
        bank: 0,
        heading: 114,
        aircraft: "A321",
        tailNumber:"B-7161",
        AITitle:"Airbus 321",
        livery: "China Eastern Official"
    },
    temp_Data:{
        ATC_Altitude:"38100",
        ATC_Speed:"",
        ATC_Heading:"",
        handledBy:"PRC_FSS" //HandledBy 此飞机被谁接管，如无则为空字符串,
        requestHandleBy:"";
        ATC_Remark:"锤子2287"
    }
}
```

### 管制员在线信息

管制员软件目前只支持实时管制，模拟机等依然需要在EuroScope操作

```js
const atcData = {
    isConnected: true,
    connectTime: 16554212000,
    updatedTime: 1655430800,
    user_data:{
        cid: "2287",
        rating: 12,
        realName: "Chunhao Jin",
        remark: "testing for new software",
        voiceCapacity:"v" //只有v可选择
    },
   	online_data:{
        callsign: "ZYHB_05_APP",
        realName: "Chunhao Jin"
        frequency: "128.75",
        range: 400,
        latitude: 44.981,
        longitude: 127.276,
        expectOffline: "1230",
        info_line: [
            "Callsign Harbin Approach",
            "Using new SKYScope(beta)",
            "enjoy your flight!"
        ]
    }
}
```



### 实时聊天信息

```js
const communicationData = {
    broadcast:[
        // SUP ADM等发送的全局广播消息
        {
            sender:{
                cid: "2287",
                callsign: "System" // 只有ADM拥有“System”权限
            },
            sendTime:  1655437600,
           	content:"Welcome to SKYline!"
        }，
        {
            sender:{
                cid: "1398",
                callsign: "Broadcast" // ADM & SUP 有"Baordcast"权限
            },
            sendTime:  1655436900,
           	content:"Official event tonight! Fly with us for ZBAA - ZHCC !"
        },
        //以此类推
    ],
    common:[
        //与上方数据类型一致
    ]
}
```



## 授权验证

系统全局使用JSON Web Token技术进行授权验证

用户在联飞软件或雷达页面登录后会利用AES算法生成一个本地证书文件certificate，里面包含用户名称，密码，时间，签发域名等信息（因为是AES加密的，大可不必担心数据安全性）

携带证书访问对应接口进行授权验证，如果成功，系统会签发有效的JWT文件（有效期60分钟），过期后携带证书再次访问，JWT会进行数据校验与签名，内部信息为可信任的，可以直接使用

【软件策略】，使用软件必须先登录，登录后才可签发有效的JWT进行后续操作

<b>除了登录系统，其余接口均需要有效的JWT授权，请在访问时携带以下请求头</b>：

```
Authorization : Bearer <Access_Token>
```



## 主要接口

接口全局HOST: ```https://api.skylineflyleague.cn/online/```

下文连接省略这个全局路径

### 用户登录

适用于Client Of SKYline与SKYScope使用

```http
HTTPS POST /connect/login
```

参数列表

| 参数        | 数据类型 | 介绍                 | 示例 |
| ----------- | -------- | -------------------- | ---- |
| certificate | string   | 用户登录所生成的证书 | 略   |

如登录成功，返回数据如下

```json
{
    "code": 200,
    "login": true,
    "token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6IjA0X3JsNjNvV2ZBSVc3WEd4UWUzQzVEY3dkTSIsImtpZCI6IjA0X3JsNjNvV2ZBSVc3WEd4UWUzQzVEY3dkTSJ9.eyJpc3MiOiJodHRwczovL2lkZW50aXR5LmFwaS5uYXZpZ3JhcGguY29tIiwiYXVkIjoiaHR0cHM6Ly9pZGVudGl0eS5hcGkubmF2aWdyYXBoLmNvbS9yZXNvdXJjZXMiLCJleHAiOjE2MTcyODAwNTIsIm5iZiI6MTYxNzI3NjQ1MiwiY2xpZW50X2lkIjoid2Vic2l0ZS13ZWI.siLCJzY29wZSI6WyJvcGVuaWQiLCJlbWFpbCIsInVzZXJpbmZvIiwicm9sZXMiLCJvZmZsaW5lX2FjY2VzcyJdLCJzdWIiOiI4YWI2YWM2NS1kZGNhLTQ1N2UtOGIzNy1lZjFlZjZjMWZhNTUiLCJhdXRoX3RpbWUiOjE2MTcyNzY0NTEsImlkcCI6Imlkc3J2IiwiZm1zZGF0YSI6InJlYWQiLCJhbXIiOl",
    "expires_in": 3600
}
```

如登录失败，返回数据如下

```json
{
    "code": 403,
    "login": false,
    token: null,
    "expires_in": 3600
}
```



### 提交飞行计划

提交用户飞行计划(此计划长期有效，除非连线后触发断线)

```http
HTTPS POST /sim/plan
```

参数列表

| 参数     | 类型       | 说明     | 示例      |
| -------- | ---------- | -------- | --------- |
| callsign | string     | 呼号     | "CES2287" |
| plan     | FlightPlan | 飞行计划 | 略        |

如果成功，服务器返回

```json
{
    "code": 200,
    "status": "success"
}
```

否则，返回

```json
{
    "code": 200,
    "status": "failed"
}
```





### 连接联飞软件

使用Client Of SKYline连接联飞服务器

```http
HTTPS POST /sim/connect
```

参数列表

| 参数     | 类型          | 说明                 | 示例      |
| -------- | ------------- | -------------------- | --------- |
| cid      | string        | 用户cid              | "2287"    |
| callsign | string        | 用户呼号             | "CES2287" |
| simdata  | SimObjectData | 用户初始化模拟器数据 | 略        |

服务器返回连接状态(status)，若成功，则将在线数据一并返回（注：为尽量减少接口请求，连接成功后即一并将数据返回，减少二次请求获取数据，update接口同理）

如果失败

```json
{
    "code": 200,
    "connected": false,
    "status": "Invalid Account",
    "online":[
        "pilots":[],
    	"atcs":[]
    ]//如果连接失败，始终返回空数组
}
```

status对应内容如下

| status           | 说明                                          |
| ---------------- | --------------------------------------------- |
| Callsign In Use  | 呼号使用中                                    |
| Invalid Callsign | 不合法的呼号（e.g.使用被封存的呼号)           |
| Empty SimObject  | 无法初始化模拟器数据                          |
| Invalid Account  | 不合法的账户（e.g.账户被封禁)                 |
| Multiple Request | 此账户已经连接                                |
| No Flight Plan   | 无飞行计划（COS要求必须先提交飞行计划再连线） |

如果成功

```json
{
    "code": 200,
    "connected": true,
    "status": "Enjoy your flight in new SKYline Technical Server",
    online":[
        "pilots":[],
    	"atcs":[]
    ]//返回数据，格式如上方“数据结构”中在线数据的一样
}
```

### 更新联飞信息

COS里面每隔几秒钟访问一次此接口，用于更新用户模拟器位置等信息，同时将在线信息一并返回

```http
HTTPS POST /sim/update
```

参数列表

| 参数     | 类型          | 说明                 | 示例      |
| -------- | ------------- | -------------------- | --------- |
| callsign | string        | 用户callsign         | "CES2287" |
| simdata  | SimObjectData | 用户初始化模拟器数据 | 略        |

返回数据与连接接口的一样，此处省略

### 应答机设置

```http
HTTPS POST /sim/squawk
```

参数列表

| 参数     | 类型              | 说明 | 示例      |
| -------- | ----------------- | ---- | --------- |
| callsign | string            | 呼号 | "CES2287" |
| type     | "S" \| "C" \| "I" | 类型 | C         |

如果成功，服务器返回status="success"，否则返回详细信息



### 用户断线

触发断线的条件有以下几种：手动访问断线接口（如用户手动断线、客户端检测到模拟器退出等）、服务端对应数据15秒钟没有被更新等情况

```http
HTTPS POST /sim/disconnect
```

参数列表

| 参数     | 类型   | 说明 | 示例      |
| -------- | ------ | ---- | --------- |
| callsign | string | 呼号 | “CES2287” |

返回数据

```json
{
    "code": 200,
    "disconnect": true,
    "totalOnline": 128200,//共计在线时间，格式为时间戳
}
```



### 获取在线数据

```http
HTTP GET /sim/get
```

此接口无需参数

返回数据

```json
{
    "pilots":[],
    "atcs":[]
}
```

### 发送信息

机组/管制员通用，发送信息给其他人

```http
HTTPS POST /message/send
```

参数列表

| 参数            | 类型   | 说明          | 示例           |
| --------------- | ------ | ------------- | -------------- |
| sender_cid      | string | 发送消息者cid | "2287"         |
| sender_callsign | string | 发送者呼号    | "ZBAA_13_APP"  |
| to_callsign     | string | 接受者呼号    | "DLH1398"      |
| message         | string | 消息内容      | "晚上来上管不" |

如果发送成功，返回

```json
{
    "code": 200,
    "status": "success"
}
```

否则，返回

```json
{
    "code": 200,
    "status": "callsign not found" 
    // "callsign not found" 未找到呼号 "server busy" 服务器忙 "invalid send"不合法的发送请求
}
```



### 管制员连线

```http
HTTPS POST /control/connect
```

参数列表

| 参数     | 类型                | 说明           | 示例      |
| -------- | ------------------- | -------------- | --------- |
| cid      | string              | 用户cid        | "2287"    |
| callsign | string              | 用户呼号       | "CES2287" |
| data     | ControllerLoginData | 管制员登录信息 | 略        |

也会返回在线列表数据，格式与“连接联飞软件”的几乎相同，此处省略



### 管制员接管飞机

```http
HTTPS POST /control/accept
```

参数列表：

| 参数       | 类型   | 说明           | 示例         |
| ---------- | ------ | -------------- | ------------ |
| controller | string | 管制员callsign | "ZGGG_W_TWR" |
| flight     | string | 机组callsign   | "CCA4154"    |

成功接管，服务器返回

```json
{
    "code": 200,
    "status": "success"
}
```

否则，返回

```json
{
    "code": 200,
    "status": "invalid callsign"
    //"invalid callsign" 呼号未找到   "already handled by others" 飞机已经被其他人接管  "invalid account" 权限不足
}
```





### 管制员修改机组临时数据

在雷达操作时候，管制员给机组设置临时数据（如目标高度、目标速度等内容，需要进行上传与共享，这样其他管制员就页可以看到数据

```http
HTTPS POST /control/modify
```

参数列表

| 参数   | 类型                    | 说明                       | 示例           |
| ------ | ----------------------- | -------------------------- | -------------- |
| cid    | string                  | 管制员callsign             | “ZSPD_TWR”     |
| flight | string                  | 机组callsign               | "CES2287"      |
| type   | string -> ModifyType    | 要修改的数据，详见下方表格 | alt            |
| data   | any -> string \| number | 修改后的数值               | “9800 - FL321” |

ModifyType数据详情结构

| 可选类型     | 说明             | 对应数据类型   | 示例               |
| ------------ | ---------------- | -------------- | ------------------ |
| alt          | 目标高度         | string         | "9800 - FL321"     |
| heading      | 目标航向         | number         | 180                |
| speed        | 目标速度         | string         | "321" 或 ".76"     |
| remark       | 备注             | string         | "这家伙没油了"     |
| route        | 飞行计划航路     | string         | "DCT"              |
| crz          | 预计巡航高度     | string         | "FL321" 或 "32100" |
| rule         | 飞行规则         | "IFR" \| "VFR" | "IFR"              |
| alternative  | 备降机场         | string         | "ZSJN"             |
| dep          | 起飞机场         | string         | "ZYHB"             |
| arr          | 落地机场         | string         | "ZSQD"             |
| pilot_remark | 机组备注信息     | string         | " "                |
| etd          | 预计起飞时间     | string         | "1225"             |
| fuel         | 预计油量使用时间 | string         | "0340"             |

修改成功，返回

```json
{
    "code": 200,
    "status": "success"
}
```

修改失败，返回

```json
{
    "code": 200,
    "status": "callsign not found" 
    // "callsign not found" 未找到呼号 "unhandled" 当前飞机未被你所接管 "invalid send"不合法的发送请求
}
```



### 移交/取消接管

```http
HTTPS POST /control/handle
```

参数列表

| 参数       | 类型              | 说明                                         | 示例       |
| ---------- | ----------------- | -------------------------------------------- | ---------- |
| conteoller | string            | 管制员callsign                               | "ZBPE_CTR" |
| flight     | string            | 机组callsign                                 | "CCA4154"  |
| mode       | "handle" \| "end" | 模式(移交/取消接管)                          | handle     |
| handleTo   | string \| null    | 申请移交给的管制员callsign （end模式不需要） | "ZSPD_CTR" |

成功，返回

```json
{
    "code": 200,
    "status": "success"
}
```

失败，返回

```json
{
    "code": 200,
    "status": "callsign not found" 
    // "callsign not found" 未找到呼号 "controller not found" 目标管制员没找到
}
```



### 强行断线

只有SUP与ADM权限才允许使用本接口，该接口可以使机组/管制员强行断线

```http
HTTPS POST /control/kill
```

参数列表：

| 参数     | 类型             | 说明              | 示例      |
| -------- | ---------------- | ----------------- | --------- |
| type     | "atc" \| "pilot" | 使机组/管制员断线 | pilot     |
| callsign | string           | 指定呼号          | "DLH1398" |

如果成功，服务器返回

```json
{
    "code": 200,
    "status": "success"
}
```

否则，返回结果

```json
{
    "code": 200,
    "status": "invalid account" // "callsign not found" 未找到呼号  "invalid account"等级不足
}
```



注（彩蛋），如果触发本接口且invalid account，服务器会给发送者发送一个warning(客户端&管制软件)，虽然有域名保护，一般人进不去



### 气象信息

SKYScope用，COS也可能用？

```http
HTTPS GET /tool/weather?icao=<机场ICAO>
```

服务器返回

```json
{
    "code": 200,
    "data": "METER HOMO 114514Z 114514MPS 191V981 0000 11/M45 Q1419 NOSIG="
}
```

