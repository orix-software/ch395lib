
一、需设置的参数
1）本地IP地址、网关、子网掩码
2）CH395与MCU接口方式及中断引脚
3）登陆用户名，密码

二、提供四个接口函数
1）登陆CH395_FTPLogin——完成整个登陆过程
2）搜查文件CH395_FTPSearch——搜查指定目录下的文件
3）下载文件CH395_FTPGetFile——下载指定文件
4）上传文件CH395_FTPPutFile——上传文件至服务器

三、MAIN.C为主函数
1）进行变量初始化；
2）查询中断；
3）查询FTP命令执行状态。

四、CH395.C为CH395命令代码。

五、CH395FTP.C文件为FTP命令执行状态，以及数据传输状态；
1）建立连接；
2）发送数据；
3）接收数据；
4）数据传输完成状态。

六、CH395FTPCMD.C为FTP命令协议部分；
包含基本命令，能保证基本的通信。

七、CH395FTPFILE.C为数据处理部分
本Demo程序
1）搜查根目录下一个指定的文件夹，搜查到文件夹则打开文件夹搜索指定文件，搜索到文件则下载文件。
2）上传文件：在根目录下创建一个文件夹，将上传的文件保存在这个文件中。若文件夹存在，则更换另外一个名字创建，直至创建成功。
