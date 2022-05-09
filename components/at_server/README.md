# AT指令

本文将详细描述每个 AT 指令的语法格式，功能及使用。

**所有指令**均为**立即生效**， **AT 指令** 可以通过超级终端等串口程序进行输入，也可以通过编程输入。



## AT指令格式

`AT+` 指令采用基于 ASCII 码的命令行，格式如下：

### 说明

+ a) <>：表示必须包含的部分
+ b) []：表示可选的部分


### 命令格式

```
AT[+CMD][op][para-1,para-2,para-3,......]\r
a) AT: 命令消息前缀
b) +CMD: 指令字符串
c) [op]: 指令操作符。可以是：
    1. "=": 表示执行指令，或参数设置，或查询
    3. "无":表示执行指令，或查询
d) [para-n]: 表示设置的参数值，或指定查询的参数，或设置的参数值并执行
e) \r: 回车结束符，ASCII码值为0x0d
```

### 消息格式

```
[\r\n][MSGTYPE:][para-1,para-2,para-3,......]<\r\n>
a) \r:：回车结束符，ASCII码值为0x0d
b) \n ：换行符，ASCII码值为0x0a
c) [MSGTYPE:] ：相关消息的前缀,如CMDRSP：、EVT：
d) [para-n] ：具体的消息
```

#### 命令响应格式

任何命令的执行均由所响应，任何无效的数据输入也将进行响应的响应，消息将在收到\r\n、\r或\n三个中的一个之后被执行。

若输入数据为非数据，其第一个字符为非‘A’,第二个字符为非'T'，则将响应错误[\r\n]CMDRSP:ERROR\r\n。

命令响应的格式：

```
[\r\n]<CMDRSP:>[para-1,para-2,para-3,......]<\r\n>
a) \r:：回车结束符，ASCII码值为0x0d
b) \n ：换行符，ASCII码值为0x0a
c) ”CMDRSP:“：命令的响应前缀
d) [para-n] ：具体的命令执行的相关信息的
```

##### 命令执行结束消息

任何命令的执行结束由下面两条消息中的一个指示。

```
CMDRSP:ERROR\r\n   :命令执行失败或者发生了错误
CMDRSP:OK\r\n      :命令执行成功并且结束，可进行下一条执行的执行
```

#### EVENT格式

EVENT描述当前的状态，如 WiFi/BT/BLE 功能及其状态、网络连接状态、TCP/UDP等网络的状态。

例如：STATION、SOFTAP连接状态，TCP、UDP连接情况等等。

事件消息格式：

```
[\r\n]<EVENT>[para-1,para-2,para-2,...,para-n]\r\n
• <EVENT>：事件消息
• [para-n]：具体事件消息，详见EVENT事件的描述
• \r: 回车符，ASCII码值为0x0d
• \n: 换行符，ASCII码值为0x0a
```

事件主要有以下几个分类：

    1. 上电是就绪的  \r\nready\r\n
    2.  运行时的具体事件： [\r\n]<EVT:>[para-1,para-2,para-2,...,para-n]<\r\n>





## AT指令集

AT指令集 “ATSVR-01.01” 主要包括：

1. [基础AT指令集](###基础AT指令集)
2. [wifi指令集](###wifi指令集)
3. [TCP/UDP通信指令集](###TCP/UDP通信指令集)



### 基础AT指令集

下表列举了支持的相关AT命令。

###### 表1 基础AT指令集

| Order | Command    | Description                                        |
| ----- | ---------- | -------------------------------------------------- |
| 1     | AT         | Used to verify that the command response is normal |
| 2     | AT+HELP    | Query supported directives                         |
| 3     | AT+VERSION | Query the version of AT                            |
| 4     | AT+ECHO    | Configure input data echo                          |
| 5     | AT+RST     | Restart the system                                 |
|       |            |                                                    |






### wifi指令集



###### 表2 wifi指令集

| Order | Command      | Description                         |
| ----- | ------------ | ----------------------------------- |
| 1     | AT+WLMAC     | Gets the MAC address of the station |
| 2     | AT+STADHCP   |                                     |
| 3     | AT+STAAPINFO |                                     |
| 4     | AT+STASTOP   |                                     |
| 5     | AT+STASTATIC |                                     |
| 6     | AT+STAST     |                                     |
| 7     | AT+WSCAN     |                                     |
| 8     | AT+SOFTAP    |                                     |
| 9     | SOFTAPSTOP   |                                     |
| 10    | AT+APSTATIC  |                                     |
| 11    | AT+APSTATE   |                                     |
|       |              |                                     |



### TCP/UDP通信指令集



###### 表3 TCP/UDP通信指令集

| Order | Command     | Description                                                 |
| ----- | ----------- | ----------------------------------------------------------- |
| 1     | AT+CIPSTART | AT+CIPSTART=<id>,<type>,[domain],[remote_port],[local_port] |
| 2     | AT+CIPSTOP  | AT+CIPSTOP=<id>[,<remote_port>]                             |
| 3     |             |                                                             |
| 4     |             |                                                             |
|       |             |                                                             |
|       |             |                                                             |





## EVENT事件

根据不同的功能，EVENT 事件通知包括：



###### 表 EVENT事件集

| Order | Command                   | Description                                |
| ----- | ------------------------- | ------------------------------------------ |
| 1     | \r\nready\r\n             | Notification system ready                  |
| 2     | EVT:WLAN DISCONNECTED\r\n | Notification station role was disconnected |
| 3     | EVT:WLAN CONNECTED        | Notification station role was connected    |
| 4     | EVT:GOT-IP                | Notification station role was got ip       |
|       |                           |                                            |
|       |                           |                                            |






