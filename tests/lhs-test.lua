-- HttpServer指定的监听目录
local server_dir = cc.FileUtils:getInstance():getWritablePath()

-- HttpServer指定的IP，不填默认本机
local server_ip = ''

-- HttpServer指定的端口，默认16888
local server_port = '16888'

-- 创建一个HttpServer实例
local server = cc.HttpServer:create(server_dir, server_ip, server_port)
if not tolua.isnull(server) then
    print('cc.HttpServer 创建失败')
    return
end
print('cc.HttpServer 已创建')

-- 输出HttpServer基础信息
print('cc.HttpServer.dir', server:dir())
print('cc.HttpServer.ip', server:ip())
print('cc.HttpServer.port', server:port())

-- 启动HttpServer，此时网页可以访问了
server:start()

-- 查看HttpServer运行状态
print('cc.HttpServer.running', server:running())
print('cc.HttpServer.paused', server:paused())

-- 用延时来观察HttpServer暂停/恢复/关闭情况
local scene = cc.Director:getInstance():getRunningScene()
local time_handler_1 = handler(server, server.pause)
local time_handler_2 = handler(server, server.resume)
local time_handler_3 = handler(server, server.shutdown)
performWithDelay(scene, time_handler_1, 10) -- 10秒后暂停，网页将不可访问
performWithDelay(scene, time_handler_2, 30) -- 30秒后恢复，网页访问恢复
performWithDelay(scene, time_handler_3, 60) -- 60秒后关闭，网页不可访问
