# SKYline新版在线联飞服务器后端API文档（v1.0）

issued on : 2023-02-23 20:00

last update : 2023-02-23 11:51

last modified by : 2287



## 总体介绍

新版服务器后端处理请求以WebSocket为基础

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
        onFrequency: '128.765',
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
        realName: "Chunhao Jin",
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

聊天信息基于WebSocket实时通信，下面为数据格式

```js
{
   sender:{
      cid: "2287",
      callsign: "PRC_FSS",
      type:"System" // "System" 系统级别消息，只有ADM允许发送; "BroadCast" 广播，ADM & SUP可以发送；"Chat" 聊天，均可
   },
   to: "DLH1398",
   sendTime:  1655437600,
   content:"Welcome to SKYline!"
}
```



## 授权验证

系统全局使用JSON Web Token技术进行授权验证

用户在联飞软件或雷达页面登录后会利用AES算法生成一个本地证书文件certificate，里面包含用户名称，密码，时间，签发域名等信息（因为是AES加密的，大可不必担心数据安全性）

携带证书访问对应接口进行授权验证，如果成功，系统会签发有效的JWT文件（有效期60分钟），过期后携带证书再次访问，JWT会进行数据校验与签名，内部信息为可信任的，可以直接使用

【软件策略】，使用软件必须先登录，登录后才可签发有效的JWT进行后续操作

<b>除了登录系统，其余接口均需要有效的JWT授权，请在访问时携带以下请求头</b>：

```
Authorization : <Access_Token>
```

示例 

```js
const ws = new WebSocket('ws://localhost:3000/chat');
ws.send('Hello, world!', {
  'Authorization': 'access_token goes here'
});
```



### 登录接口详细说明

certificate为AES加密过的字符串，解密后内部具体数据如下

```json
{
    "cid":"2287",
    "password":"114514",
    "email":"3084895416@qq.com",
    "domain":"https://sim.skylineflyleague.cn/",
    "time":1654425500,
    "random":"随机字符串"
}
```

后端收到certificate后尝试解密，如果解密失败返回登录失败

解密成功后首先需要判断请求来源(referer)与证书的domain是否符合，如果不返回，登录接口返回信息为

```
signature do not match your certificate
```

如果访问来源全部验证完成，则进行登录校验，成功后生成JWT，返回给客户（见于下方用户登录接口）



## 主要接口

接口全局HOST: ```https://api.skylineflyleague.cn/online/```

下文连接省略这个全局路径

### 用户登录

适用于Client Of SKYline与SKYScope使用

```http
WebSocket /connect/login
```

发送数据示例

```json
{
    "certificate":"xxx略"
}
```

| 参数        | 数据类型 | 介绍                 | 示例 |
| ----------- | -------- | -------------------- | ---- |
| certificate | string   | 用户登录所生成的证书 | 略   |

如登录成功，返回数据如下

```json
{
    "code": 200,
    "login": true,
    "token": "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6IjA0X3JsNjNvV2ZBSVc3WEd4UWUzQzVEY3dkTSIsImtpZCI6IjA0X3JsNjNvV2ZBSVc3WEd4UWUzQzVEY3dkTSJ9.eyJpc3MiOiJodHRwczovL2lkZW50aXR5LmFwaS5uYXZpZ3JhcGguY29tIiwiYXVkIjoiaHR0cHM6Ly9pZGVudGl0eS5hcGkubmF2aWdyYXBoLmNvbS9yZXNvdXJjZXMiLCJleHAiOjE2MTcyODAwNTIsIm5iZiI6MTYxNzI3NjQ1MiwiY2xpZW50X2lkIjoid2Vic2l0ZS13ZWI.siLCJzY29wZSI6WyJvcGVuaWQiLCJlbWFpbCIsInVzZXJpbmZvIiwicm9sZXMiLCJvZmZsaW5lX2FjY2VzcyJdLCJzdWIiOiI4YWI2YWM2NS1kZGNhLTQ1N2UtOGIzNy1lZjFlZjZjMWZhNTUiLCJhdXRoX3RpbWUiOjE2MTcyNzY0NTEsImlkcCI6Imlkc3J2IiwiZm1zZGF0YSI6InJlYWQiLCJhbXIiOl",
    "expires_in": 3600 //JWT过期时间
}
```

如登录失败，返回数据如下

```json
{
    "code": 403,
    "login": false,
    "token": null,
    "expires_in": 3600,
    "message":"错误提示"
}
```



### 提交飞行计划

提交用户飞行计划(此计划长期有效，除非连线后触发断线)

```http
WebSocket /sim/plan
```

发送数据示例

```json
{
    "callsign":"CES2287",
    "plan":{
      callsign:"CES2287",
      departure: "ZUCK",
      arrival:"ZYHB",
      route:"ZUCK DCT ZYHB",
      alternative:"ZYMD",
      ETD:"1230",
      CruiseAltitude: "",
      planFuel:"",
      flightRule:"IFR"
    }
}
```

| 参数     | 类型       | 说明     | 示例      |
| -------- | ---------- | -------- | --------- |
| callsign | string     | 呼号     | "CES2287" |
| plan     | FlightPlan | 飞行计划 | 略        |

【此接口无需响应】



### 连接联飞软件

使用Client Of SKYline连接联飞服务器

<b style="color:red">【更新】注：取消更新飞机信息的接口，更新与连线均使用此接口完成</b>

```http
WebSocket /sim/connect?type='<模式，第一次为connect，后续更新数据为update>'
```

发送示例：

````json
{
    "cid": "2287",
    "callsign":"CES2287",
    "simdata": {
        latitude: 40.2433,
        longitude: 123.98712,
        altitude: 32891,
        speed: 361,
        squawk: 7700,
        squawkMode: 'C',
        pitch: 0,
        bank: 0,
        heading: 114,
        onFrequency: '128.765',
        aircraft: "A321",
        tailNumber:"B-7161",
        AITitle:"Airbus 321",
        livery: "China Eastern Official"
    },
    otherData:{
        realName: "Chunhao Jin",
        remark: "Fly with Client Of SKYline in Technical Server",
        voiceCapacity:"v" ,
    }
}
````

| 参数      | 类型          | 说明                                      | 示例      |
| --------- | ------------- | ----------------------------------------- | --------- |
| cid       | string        | 用户cid                                   | "2287"    |
| callsign  | string        | 用户呼号                                  | "CES2287" |
| simdata   | SimObjectData | 用户初始化模拟器数据                      | 略        |
| otherData | OtherData     | 也是用户其他数据，只不过不是simObject里的 | 略        |

如果type=update，说明之前已经连接过了，本次为更新数据，服务器无需响应，否则为第一次连接，则服务器返回连接状态(status)

如果失败

```json
{
    "code": 200,
    "connected": false,
    "status": "Invalid Account"
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
    "connected": true
}
```

### 更新联飞信息

不再单独设置接口，使用/sim/connect

### 应答机设置

手动设置应答机模式

```http
WebSocket /sim/squawk
```

发送示例：

```json
{
    "callsign":"CES2287".
    "type": "S"
}
```

| 参数     | 类型              | 说明 | 示例      |
| -------- | ----------------- | ---- | --------- |
| callsign | string            | 呼号 | "CES2287" |
| type     | "S" \| "C" \| "I" | 类型 | C         |

【此接口无需响应】



### 用户断线

触发断线的条件有以下几种：手动访问断线接口（如用户手动断线、客户端检测到模拟器退出等）、服务端对应数据15秒钟没有被更新等情况

```http
WebSocket /sim/disconnect
```

参数列表

| 参数     | 类型   | 说明 | 示例      |
| -------- | ------ | ---- | --------- |
| callsign | string | 呼号 | “CES2287” |

【此接口无需响应】

### 获取在线数据

```http
WebSocket /sim/get
```

无需参数, 用户访问后循环2-3秒发送一次数据

注：

机组数据只返回cid, callsign与simobject_data即可，其他信息无需返回

管制员可以全部返回

返回数据

```json
{
    "pilots":[
        {
            cid: "2287",
            callsign: "CES2287",
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
                onFrequency: '128.765',
                aircraft: "A321",
                tailNumber:"B-7161",
                AITitle:"Airbus 321",
                livery: "China Eastern Official"
            }
        }
    ],
    "atcs":[
        {
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
                realName: "Chunhao Jin",
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
    ]
}
```



### 发送信息

机组/管制员通用，发送信息给其他人

```http
HTTPS POST /message/send
```

数据形式见于“数据结构” - “实时聊天信息”

```json
{
   sender:{
      cid: "2287",
      callsign: "PRC_FSS",
      type:"System" // "System" 系统级别消息，只有ADM允许发送; "BroadCast" 广播，ADM & SUP可以发送；"Chat" 聊天，均可
   },
   to: "DLH1398",
   sendTime:  1655437600,
   content:"Welcome to SKYline!"
}
```

将消息转发时，将原消息直接发送即可，无需后端进行其他处理，无需响应

| 类型      | 说明                                               | 权限         |
| --------- | -------------------------------------------------- | ------------ |
| System    | 发送系统基本消息，所有人均能接收，标题显示"System" | ADM only     |
| BroadCast | 发送广播，所有人均能接收，优先级低于System         | ADM & SUP    |
| Chat      | 私人聊天                                           | 不为封禁均可 |

【此接口无需响应】

### 公共频道信息

发送消息到公共频道

```http
WebSocket /message/channel
```

发送数据

```json
{
   sender:{
      cid: "2287",
      callsign: "ZYCC_APN",
      type: "public"
   },
   to: "129.870",
   sendTime:  1655437600,
   content:"Welcome to 129.870, conteoller online~"
}
```

将此消息发送到该频道内的全部用户即可，无需响应结果

【此接口无需响应】



### 管制员连线

```http
WebSocket /control/connect
```

发送示例

```json
{
    "cid": "2287",
    "callsign": "ZSQD_TWR",
   	"online_data":{
        callsign: "ZYHB_05_APP",
        realName: "Chunhao Jin",
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
    },
    "other_data":{
        realName: "Chunhao Jin",
        remark: "testing for new software",
        voiceCapacity:"v" //只有v可选择
    }
}
```



参数列表

| 参数        | 类型                | 说明                         | 示例          |
| ----------- | ------------------- | ---------------------------- | ------------- |
| cid         | string              | 用户cid                      | "2287"        |
| callsign    | string              | 用户呼号                     | "ZYHB_05_APP" |
| online_data | ControllerLoginData | 管制员在线信息               | 略            |
| other_data  | ControllerLoginData | 也是管制员信息，但是别的内容 | 略            |

也会返回在线列表数据，格式与“连接联飞软件”的几乎相同，此处省略



### 管制员接管飞机

```http
WebSocket /control/accept
```

参数列表：

| 参数       | 类型   | 说明           | 示例         |
| ---------- | ------ | -------------- | ------------ |
| controller | string | 管制员callsign | "ZGGG_W_TWR" |
| flight     | string | 机组callsign   | "CCA4154"    |

成功接管，无需返回信息

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
WebSocket /control/modify
```

发送示例

```json
{
    "cid": "ZSPD_TWR",
    "flight": "CES2287",
    "type": "alt",
    "data": "9800 - FL321"
}
```

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

修改成功，无需响应

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
WebSocket /control/handle
```

JSON参数列表

| 参数       | 类型              | 说明                                         | 示例       |
| ---------- | ----------------- | -------------------------------------------- | ---------- |
| conteoller | string            | 管制员callsign                               | "ZBPE_CTR" |
| flight     | string            | 机组callsign                                 | "CCA4154"  |
| mode       | "handle" \| "end" | 模式(移交/取消接管)                          | handle     |
| handleTo   | string \| null    | 申请移交给的管制员callsign （end模式不需要） | "ZSPD_CTR" |

成功，无需响应，失败，返回

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
WebSocket /control/kill
```

参数列表：

| 参数     | 类型             | 说明              | 示例      |
| -------- | ---------------- | ----------------- | --------- |
| type     | "atc" \| "pilot" | 使机组/管制员断线 | pilot     |
| callsign | string           | 指定呼号          | "DLH1398" |

如果成功，无需响应

否则，返回结果

```json
{
    "code": 200,
    "status": "invalid account" // "callsign not found" 未找到呼号  "invalid account"等级不足
}
```



### 气象信息(本功能开发待定)

SKYScope用，COS也可能用？

```http
WebSocket /tool/weather?icao=<机场ICAO>
```

服务器返回

```json
{
    "code": 200,
    "data": "METER HOMO 114514Z 114514MPS 191V981 0000 11/M45 Q1419 NOSIG="
}
```

