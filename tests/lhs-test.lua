function startHttpServer(node)
    -- HttpServer指定的监听目录
    local server_dir = cc.FileUtils:getInstance():getWritablePath()

    -- HttpServer指定的IP，不填默认本机
    local server_ip = ''

    -- HttpServer指定的端口，默认16888
    local server_port = '16888'

    -- 创建一个HttpServer实例
    local server = cc.HttpServer:create(server_dir, server_ip, server_port)
    if tolua.isnull(server) then
        print('cc.HttpServer 创建失败')
        return
    end
    print('cc.HttpServer 已创建')

    -- 输出HttpServer基础信息
    print('== cc.HttpServer基础信息 ==')
    print('cc.HttpServer.dir ', server:dir())
    print('cc.HttpServer.ip  ', server:ip())
    print('cc.HttpServer.port', server:port())
    print('-------------------------')

    -- 启动HttpServer，此时网页可以访问了
    server:start()

    -- 查看HttpServer运行状态
    print('== cc.HttpServer运行状态 ==')
    print('cc.HttpServer.running', server:running())
    print('cc.HttpServer.paused', server:paused())
    print('-------------------------')

    -- 用延时来观察HttpServer暂停/恢复/关闭情况
    local time_handler_1 = function()
        server:pause()
        print('== cc.HttpServer.pause 暂停监听 ==')
        print('cc.HttpServer.running', server:running())
        print('cc.HttpServer.paused ', server:paused())
        print('-------------------------')
    end
    local time_handler_2 = function()
        server:resume()
        print('== cc.HttpServer.resume 恢复监听 ==')
        print('cc.HttpServer.running', server:running())
        print('cc.HttpServer.paused', server:paused())
        print('-------------------------')
    end
    local time_handler_3 = function()
        server:shutdown()
        print('== cc.HttpServer.shutdown 请求关闭 ==')
        print('-------------------------')
    end
    local time_handler_4 = function()
        if tolua.isnull(server) then
            print('cc.HttpServer 已关闭')
            print('-------------------------')
            return
        end
    end
    performWithDelay(node, time_handler_1, 10) -- 10秒后暂停，网页将不可访问
    performWithDelay(node, time_handler_2, 20) -- 20秒后恢复，网页访问恢复
    performWithDelay(node, time_handler_3, 30) -- 30秒后关闭，网页不可访问
    performWithDelay(node, time_handler_4, 40) -- 40秒后检测，此时server无效了
end
