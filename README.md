# IntelligentHome

## 一、back_end

### 1. 配置文件

#### api_config ：API服务的配置
host ：API服务的IP地址

port ：API服务的端口号

mode ：API服务的运行模式，release或debug

#### redis_config ：Redis的配置

    host ：Redis的IP地址

    port ：Redis的端口号

    username ：Redis的用户名

    password ：Redis的密码

    db ：Redis的数据库

    key_name ：Redis的键名

    max_data ：Redis中存储的最大数据量(单位：秒)

#### chuan_kou_config ：串口的配置

    name ：串口的名称

    baud ：串口的波特率

#### 配置文件示例
```json
{
    "api_config": {
        "host": "127.0.0.1",
        "port": "1314",
        "mode": "release"
    },
    "redis_config": {
        "host": "localhost",
        "port": "6379",
        "username": "",
        "password": "",
        "db": 0,
        "key_name": "ihome",
        "max_data": 600
    },
    "chuan_kou_config": {
        "name": "COM2",
        "baud": 9600
    }
}
```

### 2. 运行

#### 启动API服务

```shell
go run .
```
#### 构建二进制文件

```shell
go build .
```

## 二、front_end

### 1. 安装依赖

```shell
npm install
```

### 2. 运行

```shell
npm run dev
``` 

### 3. 打包

```shell
npm run build
``` 
