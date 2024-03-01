# 卓越项目后端

## 在本地使用Windows开发

确保`Windows`系统下已经安装了`Docker Desktop`和`JDK 21`.

打开`Windows`的`Terminal(Power Shell)`, 运行

```sh
./mvnw.cmd clean package
```

这会生成一个`target`文件夹, 并将构建好的`jar`包放在`target`目录下.

> 如果这个命令超过20秒没反应, 就是网络问题, 这时应该在`idea`的帮助下构建`jar`包, 步骤如下:
> - 在idea中打开右侧的`m`小窗口, 展开`LifeCycle`
> - 双击`clean`
> - 双击`package`
> 这同样会在`target`文件夹下生成`jar`包

在进行下一步之前, 确保`target`目录存在, 并且`target`目录下存在类似于`YunNanEmployBackendMvn-0.0.1-SNAPSHOT.jar`的`jar`包, 如不存在, 重复上面的步骤.

使用`docker`部署项目, 运行:

```sh
docker-compose -f docker-compose-dev.yml up
```

如果控制台最后一行输出类似

```sh
employment-backend-app-1    | 2023-10-31T12:02:01.569Z  INFO 1 --- [           main] o.e.y.YunNanEmploymentBackendApplication : St
arted YunNanEmploymentBackendApplication in 15.736 seconds (process running for 16.224)
```
的消息, 说明运行成功, 否则, 请再试一次:

```sh
docker-compose -f docker-compose-dev.yml up
```

> 注意: 每次构建运行前都需要完整执行上面的步骤(从头开始)

## 部署

确保服务器已经安装`docker compose`, 执行:

```sh
docker compose up
```

进行自动化构建和部署, 服务器应当开放`80`端口.

## API

### /user

#### /user/login

用户登录, 要求发送`post`请求, 例如:

```json
{
  "userId": "mxy123",
  "password": "123456"
}
```

得到的响应:

```json
{
  "success": true,
  "message": "success",
  "data": true
}
```

`data`字段是布尔值, 表示是否**登录**成功, 如果不成功, 失败原因通过`message`字段返回.

#### /user/register

用户注册, 要求发送`post`请求, 例如:

```json
{
  "userId": "mxy123",
  "password": "123456"
}
```
得到的响应:

```json
{
  "success": true,
  "message": "success",
  "data": true
}
```

`data`字段是布尔值, 表示是否**注册**成功, 如果不成功, 失败原因通过`message`字段返回.