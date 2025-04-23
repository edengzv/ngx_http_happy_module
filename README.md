ngx_http_happy_module

#usage
* 把模块编译进 [nginx](http://nginx.com/)（或者 [tengine](http://tengine.taobao.org/)）
````
./configure --add-module=PATH`  PATH 为包含模块的目录
make
make install
````

* 添加模块配置
在/usr/local/nginx/conf/nginx.conf 对应server{} 添配置
````
location /happycoding{
  happy;
}
````
* 重启nginx
````
sudo /usr/local/nginx/sbin/nginx -s stop
sudo /usr/local/nginx/sbin/nginx -s start
````
* 访问`localhost/happycoding`
![happy coding](-)

**Just for fun :)**



