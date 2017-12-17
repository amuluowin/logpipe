��־�ɼ�����(logpipe)
====================
<!-- TOC -->

- [1. ����](#1-����)
- [2. ��װ](#2-��װ)
    - [2.1. Դ����밲װ](#21-Դ����밲װ)
        - [2.1.1. ���밲װlogpipe](#211-���밲װlogpipe)
        - [2.1.2. ���밲װ�Դ�logpipe���](#212-���밲װ�Դ�logpipe���)
        - [2.1.3. ȷ�ϰ�װ](#213-ȷ�ϰ�װ)
- [3. ʹ��](#3-ʹ��)
    - [3.1. ����A](#31-����a)
        - [3.1.1. ����鼯��](#311-����鼯��)
        - [3.1.2. ����ɼ���](#312-����ɼ���)
        - [3.1.3. ����](#313-����)
        - [3.1.4. ֹͣlogpipe](#314-ֹͣlogpipe)
- [4. �ο�](#4-�ο�)
    - [4.1. logpipe](#41-logpipe)
    - [4.2. �Դ����](#42-�Դ����)
        - [4.2.1. logpipe-input-file](#421-logpipe-input-file)
        - [4.2.2. logpipe-output-file](#422-logpipe-output-file)
        - [4.2.3. logpipe-input-tcp](#423-logpipe-input-tcp)
        - [4.2.4. logpipe-output-tcp](#424-logpipe-output-tcp)
- [5. �������](#5-�������)
    - [5.1. ������](#51-������)
    - [5.2. ������](#52-������)
- [6. ����ѹ��](#6-����ѹ��)
    - [6.1. flume-ng](#61-flume-ng)
    - [6.2. logpipe](#62-logpipe)
    - [6.3. �ܽ�](#63-�ܽ�)
- [7. ���](#7-���)

<!-- /TOC -->
# 1. ����

�ڼ�Ⱥ���������־�ɼ�����Ҫ������ʩ��

��Դ������������ǻ���flume-ng������ʵ��ʹ���з���flume-ng����������⣬����flume-ng��spoolDir�ɼ���ֻ�ܶ��ļ���ת����Ĵ�С���ܱ仯��������־�ļ����вɼ�����������ɼ�ʱЧ��Ҫ�����Ҫ�ɼ����ڱ�����׷�ӵ���־�ļ���ֻ����exec�ɼ�������tail -F�����tail -F�����ֲ���ͨ��Ŀ��Ŀ¼�н���������δ֪�ļ������������������logstash������JAVA�������ڴ�ռ�ú����ܶ����ܴﵽ���š�

��Ϊһ����־�ɼ��ı��ش������ڴ�ռ��Ӧ��С���ܿأ�����Ӧ�ø�Ч���ķ�CPU�Ͷ�Ӧ��Ӱ�쾡����С��Ҫ���첽ʵʱ׷����־�ļ�������ĳЩӦ�û���Ŀ��Ŀ¼�²��������־�ļ��������ڲ���ȷ����������־�ļ������ܹ���Ҫ֧�ֶ�����������ʽ��־�ɼ����䣬Ϊ�˴�������������о������輼��������ʵ���ѶȲ����ߣ���������logpipe��

logpipe��һ���ֲ�ʽ���߿��õ����ڲɼ������䡢�Խ���ص���־���ߣ������˲�����Ŀ�ܽṹ��ƣ�֧�ֶ��������������������������ʽ��־�ռ��ܹ����޵�����������

![logpipe.png](logpipe.png)

logpipe������ʵ��ʹ�÷��㡢���ü�����û����sink��һ��������ʡ�

logpipe�����ɸ�input���¼����ߺ����ɸ�output��ɡ�����logpipe��������(monitor)������һ����������(worker)����ع������̱����������������̡���������װ�����ü������ɸ�input��������ɸ�output����������¼�ѭ������һinput���������Ϣ�����������output�����

logpipe�Դ���4���������󽫿��������������ֱ��ǣ�
* logpipe-input-file ��inotify�첽ʵʱ�����־Ŀ¼��һ�����ļ��½����ļ������¼�������ע�⣺������������ѯ�ļ��޸�ʱ��ʹ�С�������������ļ����Ͷ�ȡ�ļ�׷�����ݡ��ò��ӵ���ļ���Сת�����ܣ��������Ӧ����־���Ӧ���ܣ����Ӧ����־��д��־���ܡ��ò��֧������ѹ����
* logpipe-output-file һ������������Ϣ����������ͬ���ļ�������ļ����ݡ��ò��֧�����ݽ�ѹ��
* logpipe-input-tcp ����TCP���������ˣ����տͻ������ӣ�һ���ͻ���������������Ϣ������������ȡ��
* logpipe-output-tcp ����TCP�ͻ��ˣ����ӷ���ˣ�һ������������Ϣ����������������ӡ�

ʹ���߿ɸ����������󣬰��ղ�������淶���������Ʋ������IBMMQ��������HDFS�������ȡ�

logpipe���ò���JSON��ʽ����η�������д��࣬��ʾ����

```
{
	"log" : 
	{
		"log_file" : "/tmp/logpipe_case1_collector.log" ,
		"log_level" : "INFO"
	} ,
	
	"inputs" : 
	[
		{ "plugin":"so/logpipe-input-file.so" , "path":"/home/calvin/log" , "compress_algorithm":"deflate" }
	] ,
	
	"outputs" : 
	[
		{ "plugin":"so/logpipe-output-tcp.so" , "ip":"127.0.0.1" , "port":10101 }
	]
}

```

# 2. ��װ

## 2.1. Դ����밲װ

### 2.1.1. ���밲װlogpipe

��[��Դ�й�](https://gitee.com/calvinwilliams/logpipe)��[github](https://github.com/calvinwilliams/logpipe)��¡����������Դ������ŵ����Դ�����Ŀ¼�н⿪�����¼�����Ĳ���ϵͳ��Linux��

����`src`Ŀ¼������õ���ִ�г���`logpipe`�Ͷ�̬��`liblogpipe_api.so`��

```
$ cd src
$ make -f makefile.Linux
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c list.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c rbtree.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c fasterjson.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c LOGC.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c config.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c env.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c util.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c output.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o liblogpipe_api.so list.o rbtree.o fasterjson.o LOGC.o config.o env.o util.o output.o -shared -L. -L/home/calvin/lib -rdynamic -ldl -lz 
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c main.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c monitor.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99  -c worker.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o logpipe main.o monitor.o worker.o -L. -L/home/calvin/lib -rdynamic -ldl -lz  -llogpipe_api
```

��ִ�г���logpipe������־�ɼ����ش�������̬��liblogpipe_api.so����������á�

Ȼ��װ����Ŀ�꣬Ĭ��`logpipe`��װ��`$HOME/bin`��`liblogpipe_api.so`��װ��`$HOME/lib`��`logpipe_api.h`��һЩͷ�ļ���װ��`$HOME/include/logpipe`�������Ҫ�ı䰲װĿ¼���޸�`makeinstall`���`_HDERINST`��`_LIBINST`��`_BININST`��

```
$ make -f makefile.Linux install
rm -f /home/calvin/bin/logpipe
cp -rf logpipe /home/calvin/bin/
rm -f /home/calvin/lib/liblogpipe_api.so
cp -rf liblogpipe_api.so /home/calvin/lib/
rm -f /home/calvin/include/logpipe/rbtree.h
cp -rf rbtree.h /home/calvin/include/logpipe/
rm -f /home/calvin/include/logpipe/LOGC.h
cp -rf LOGC.h /home/calvin/include/logpipe/
rm -f /home/calvin/include/logpipe/fasterjson.h
cp -rf fasterjson.h /home/calvin/include/logpipe/
rm -f /home/calvin/include/logpipe/rbtree_tpl.h
cp -rf rbtree_tpl.h /home/calvin/include/logpipe/
rm -f /home/calvin/include/logpipe/logpipe_api.h
cp -rf logpipe_api.h /home/calvin/include/logpipe/
```

### 2.1.2. ���밲װ�Դ�logpipe���

����`src`ͬ����`src-plugins`��������

```
$ cd ../src-plugins
$ make -f makefile.Linux 
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99 -I/home/calvin/include/logpipe  -c logpipe-input-file.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o logpipe-input-file.so logpipe-input-file.o -shared -L. -L/home/calvin/so -L/home/calvin/lib -llogpipe_api -rdynamic 
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99 -I/home/calvin/include/logpipe  -c logpipe-output-file.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o logpipe-output-file.so logpipe-output-file.o -shared -L. -L/home/calvin/so -L/home/calvin/lib -llogpipe_api -rdynamic 
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99 -I/home/calvin/include/logpipe  -c logpipe-input-tcp.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o logpipe-input-tcp.so logpipe-input-tcp.o -shared -L. -L/home/calvin/so -L/home/calvin/lib -llogpipe_api -rdynamic 
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -I. -I/home/calvin/include -std=gnu99 -I/home/calvin/include/logpipe  -c logpipe-output-tcp.c
gcc -g -fPIC -O2 -Wall -Werror -fno-strict-aliasing -o logpipe-output-tcp.so logpipe-output-tcp.o -shared -L. -L/home/calvin/so -L/home/calvin/lib -llogpipe_api -rdynamic 
```

Ȼ��װ����Ŀ�꣬Ĭ���Դ������װ��`$HOME/so`�������Ҫ�ı䰲װĿ¼���޸�`makeinstall`���`_LIBINST`��

```
$ make -f makefile.Linux install
rm -f /home/calvin/so/logpipe-input-file.so
cp -rf logpipe-input-file.so /home/calvin/so/
rm -f /home/calvin/so/logpipe-output-file.so
cp -rf logpipe-output-file.so /home/calvin/so/
rm -f /home/calvin/so/logpipe-input-tcp.so
cp -rf logpipe-input-tcp.so /home/calvin/so/
rm -f /home/calvin/so/logpipe-output-tcp.so
cp -rf logpipe-output-tcp.so /home/calvin/so/
```

### 2.1.3. ȷ�ϰ�װ

ȷ��`$HOME/bin`�Ѿ����뵽`$PATH`�У���������ִ��`logpipe`�����������Ϣ��ʾԴ����밲װ�ɹ�

```
$ logpipe
USAGE : logpipe -v
        logpipe -f (config_file) [ --no-daemon ] [ --start-once-for-env "(key) (value)" ]
```

�ò���`-v`���Բ鿴��ǰ�汾��

```
$ logpipe -v
logpipe v0.7.0 build Dec 17 2017 20:02:46
```

# 3. ʹ��

## 3.1. ����A

�첽ʵʱ�ɼ�`ecif@158.1.0.56:~/log/*`��`ecif@158.1.0.57:~/log/*`��`ecif@158.1.0.58:~/log/*`�µ��½���׷����־�ļ����鼯��`iblog@158.1.0.55:~/log`�¡�

### 3.1.1. ����鼯��

��ecif@158.1.0.55:~/etc�½������ļ�logpipe.conf

```
{
        "log" : 
        {
                "log_file" : "/tmp/logpipe_iblog.log" ,
                "log_level" : "INFO"
        } ,

        "inputs" : 
        [
                { "plugin":"so/logpipe-input-tcp.so" , "ip":"158.1.0.55" , "port":5151 }
        ] ,

        "outputs" : 
        [
                { "plugin":"so/logpipe-output-file.so" , "path":"/home/iblog/log" }
        ]
}
```

����logpipe

```
$ logpipe -f $HOME/etc/logpipe.conf
2017-12-17 20:19:21.358464 | INFO  | 36318:logpipe-output-file.c:29 | path[/home/iblog/log]
2017-12-17 20:19:21.358513 | INFO  | 36318:logpipe-output-file.c:44 | uncompress_algorithm[(null))]
2017-12-17 20:19:21.358515 | DEBUG | 36318:config.c:321 | [so/logpipe-output-file.so]->pfuncLoadOutputPluginConfig ok
2017-12-17 20:19:21.358520 | INFO  | 36318:logpipe-input-tcp.c:214 | ip[158.1.0.55]
2017-12-17 20:19:21.358522 | INFO  | 36318:logpipe-input-tcp.c:221 | port[5151]
2017-12-17 20:19:21.358525 | DEBUG | 36318:config.c:338 | [so/logpipe-input-tcp.so]->pfuncLoadInputPluginConfig ok
```

�ص������н��棬�����Զ�ת��Ϊ�ػ����̣�ȷ�Ͻ���

```
$ ps -ef | grep logpipe | grep -v grep
...
iblog   36320     1  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
iblog   36321 36320  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
...
```

�鿴��־��ȷ����ERROR�����ϵȼ���־����

```
$ cat /tmp/logpipe_iblog.log
2017-12-17 20:19:21.358944 | INFO  | 36320:monitor.c:149 | --- monitor begin ---------
2017-12-17 20:19:21.359079 | INFO  | 36320:monitor.c:91 | parent : [36320] fork [36321]
2017-12-17 20:19:21.359142 | INFO  | 36320:monitor.c:85 | child : [36320] fork [36321]
2017-12-17 20:19:21.359176 | INFO  | 36321:worker.c:32 | epoll_create ok , epoll_fd[1]
2017-12-17 20:19:21.359244 | INFO  | 36321:logpipe-input-tcp.c:281 | listen[158.1.0.55:5151][2] ok
2017-12-17 20:19:21.359259 | INFO  | 36321:env.c:54 | epoll_ctl[1] add input plugin fd[2] EPOLLIN ok
2017-12-17 20:19:21.359264 | INFO  | 36321:worker.c:44 | InitEnvironment ok
2017-12-17 20:19:21.359268 | INFO  | 36321:worker.c:59 | epoll_ctl[1] add quit pipe fd[0] ok
2017-12-17 20:19:21.359270 | INFO  | 36321:worker.c:67 | epoll_wait[1] ...
2017-12-17 20:19:23.663377 | INFO  | 36321:worker.c:84 | epoll_wait[1] return[1]events
2017-12-17 20:19:23.663438 | INFO  | 36321:env.c:162 | epoll_ctl[1] add input plugin fd[3] ok
2017-12-17 20:19:23.663446 | INFO  | 36321:worker.c:67 | epoll_wait[1] ...
```

### 3.1.2. ����ɼ���

��`ecif@158.1.0.56:~/etc`��`ecif@158.1.0.57:~/etc`��`ecif@158.1.0.58:~/etc`�½������ļ�`logpipe.conf`

```
{
        "log" : 
        {
                "log_file" : "/tmp/logpipe_ecif.log" ,
                "log_level" : "INFO"
        } ,

        "inputs" : 
        [
                { "plugin":"so/logpipe-input-file.so" , "path":"/home/ecif/log" }
        ] ,

        "outputs" : 
        [
                { "plugin":"so/logpipe-output-tcp.so" , "ip":"158.1.0.55" , "port":5151 }
        ]
}
```

�ֱ�����logpipe

```
$ logpipe -f $HOME/etc/logpipe.conf
2017-12-17 20:19:23.662483 | INFO  | 36322:logpipe-output-tcp.c:73 | ip[158.1.0.55]
2017-12-17 20:19:23.662533 | INFO  | 36322:logpipe-output-tcp.c:80 | port[5151]
2017-12-17 20:19:23.662537 | DEBUG | 36322:config.c:321 | [so/logpipe-output-tcp.so]->pfuncLoadOutputPluginConfig ok
2017-12-17 20:19:23.662549 | INFO  | 36322:logpipe-input-file.c:366 | path[/home/ecif/log]
2017-12-17 20:19:23.662551 | INFO  | 36322:logpipe-input-file.c:369 | file[(null)]
2017-12-17 20:19:23.662553 | INFO  | 36322:logpipe-input-file.c:387 | exec_before_rotating[(null)]
2017-12-17 20:19:23.662554 | INFO  | 36322:logpipe-input-file.c:394 | rotate_size[0]
2017-12-17 20:19:23.662556 | INFO  | 36322:logpipe-input-file.c:412 | exec_after_rotating[(null)]
2017-12-17 20:19:23.662557 | INFO  | 36322:logpipe-input-file.c:427 | compress_algorithm[(null)]
2017-12-17 20:19:23.662559 | DEBUG | 36322:config.c:338 | [so/logpipe-input-file.so]->pfuncLoadInputPluginConfig ok
```

�ص������н��棬�����Զ�ת��Ϊ�ػ����̣�ȷ�Ͻ���

```
$ ps -ef | grep logpipe | grep -v grep
...
ecif   36324     1  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
ecif   36325 36324  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
...
```

�鿴��־��ȷ����ERROR�����ϵȼ���־����

```
$ cat /tmp/logpipe_ecif.log
2017-12-17 20:19:23.662927 | INFO  | 36324:monitor.c:149 | --- monitor begin ---------
2017-12-17 20:19:23.663119 | INFO  | 36324:monitor.c:91 | parent : [36324] fork [36325]
2017-12-17 20:19:23.663150 | INFO  | 36324:monitor.c:85 | child : [36324] fork [36325]
2017-12-17 20:19:23.663181 | INFO  | 36325:worker.c:32 | epoll_create ok , epoll_fd[1]
2017-12-17 20:19:23.663334 | INFO  | 36325:logpipe-output-tcp.c:51 | connect[158.1.0.55:5151][2] ok
2017-12-17 20:19:23.663357 | INFO  | 36325:logpipe-input-file.c:449 | start_once_for_full_dose[0]
2017-12-17 20:19:23.663395 | INFO  | 36325:logpipe-input-file.c:467 | inotify_add_watch[/home/ecif/log] ok , inotify_fd[3] inotify_wd[1]
2017-12-17 20:19:23.690558 | INFO  | 36325:env.c:54 | epoll_ctl[1] add input plugin fd[3] EPOLLIN ok
2017-12-17 20:19:23.690575 | INFO  | 36325:worker.c:44 | InitEnvironment ok
2017-12-17 20:19:23.690581 | INFO  | 36325:worker.c:59 | epoll_ctl[1] add quit pipe fd[0] ok
2017-12-17 20:19:23.690583 | INFO  | 36325:worker.c:67 | epoll_wait[1] ...
```

### 3.1.3. ����

��`ecif@158.1.0.56:~/log`���½���׷�Ӳ�������־�ļ�

```
$ cd $HOME/log
$ echo "Hello logpipe in 158.1.0.56" >test.log
```

�����`iblog@158.1.0.55:~/log`�¿����ɼ���������־�ļ�

```
$ cd $HOME/log
$ ls
test.log
$ cat test.log
Hello logpipe 158.1.0.56
```

������`ecif@158.1.0.57:~/log`��`ecif@158.1.0.58:~/log`���½���׷�Ӳ�������־�ļ�

```
$ cd $HOME/log
$ echo "Hello logpipe 158.1.0.57" >test.log
```

```
$ cd $HOME/log
$ echo "Hello logpipe 158.1.0.58" >test.log
```

�����`iblog@158.1.0.55:~/log`�¿����ɼ���������־�ļ�

```
$ cd $HOME/log
$ ls
a.log
$ cat test.log
Hello logpipe 158.1.0.56
Hello logpipe 158.1.0.57
Hello logpipe 158.1.0.58
```

### 3.1.4. ֹͣlogpipe

��ѯ`logpipe`pid����������̷���TERM�źż��� 

```
$ ps -ef | grep logpipe | grep -v grep
...
calvin   36320     1  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
calvin   36321 36320  0 20:19 ?        00:00:00 logpipe -f logpipe.conf
...
$ kill 36320
$ ps -ef | grep logpipe | grep -v grep
$
```

# 4. �ο�

## 4.1. logpipe

```
$ logpipe
USAGE : logpipe -v
        logpipe -f (config_file) [ --no-daemon ] [ --start-once-for-env "(key) (value)" ]
```

����˵����

* -v : ��ʾ��ǰ�汾��
* -f : ָ�������ļ����������·�������·������ѡ
* --no-daemon : �Է��ػ�����ģʽ���У������󲻻�ص������н��棻��ѡ
* --start-once-for-env "(key) (value)" : ����Ϊ�����������ݸ����в���������������һ�Ρ���ѡ������logpipe-input-file.so�н��ջ�������`start_once_for_full_dose`�Ծ����������Ƿ�ɼ��Ѵ���ȫ����־�ļ���ȱʡΪ���ɼ�������־��ֻ�ɼ������ļ�����׷���ļ����ݡ�

```
$ logpipe -f $HOME/etc/logpipe.conf --start-once-for-env "start_once_for_full_dose 1"
```

## 4.2. �Դ����

### 4.2.1. logpipe-input-file

������

* `path` : �ܵ���ص�Ŀ¼������½��ļ��¼����ļ���׷�������¼��������þ���·������ѡ
* `rotate_size` : �ļ���Сת����ֵ�����ܼ���ļ���С�����ô�Сʱ�Զ�����Ϊ"_(ԭ�ļ���-����_ʱ��)"�������أ������0Ϊ�رգ���ѡ
* `exec_before_rotating` : �����ļ���Сת��ǰҪִ�е���������г��ֵ�`"`��`\"`ת�壬��ʹ�����û���������ͬ��ִ�У���ѡ
* `exec_after_rotating` : �����ļ���Сת����Ҫִ�е���������г��ֵ�`"`��`\"`ת�壬��ʹ�����û���������ͬ��ִ�У���ѡ
* `compress_algorithm` : �ɼ����ݺ�ѹ����Ŀǰ�㷨ֻ��"deflate"����ѡ

������`exec_before_rotating`��`exec_after_rotating`�����û�������

* `LOGPIPE_ROTATING_PATHNAME` : �ܵ���ص�Ŀ¼������·��
* `LOGPIPE_ROTATING_OLD_FILENAME` : ת��ǰ�ļ���������·��
* `LOGPIPE_ROTATING_NEW_FILENAME` : ת�����ļ���������·��

��������

* `start_once_for_full_dose` : �������Ƿ�ɼ��Ѵ���ȫ����־�ļ�

ʾ��

```
{ "plugin":"so/logpipe-input-file.so" , "path":"/home/ecif/log" , "compress_algorithm":"deflate" }
```

```
{ "plugin":"so/logpipe-input-file.so" , "path":"/home/calvin/log" , "exec_before_rotating":"echo \"BEFORE ROTATING ${LOGPIPE_ROTATING_OLD_FILENAME}\">>/tmp/logpipe_case2_collector.log" , "rotate_size":10 , "exec_after_rotating":"echo \"AFTER ROTATING ${LOGPIPE_ROTATING_NEW_FILENAME}\">>/tmp/logpipe_case2_collector.log" , "compress_algorithm":"deflate" }
```

### 4.2.2. logpipe-output-file

������

* `path` : �ܵ���ص�Ŀ¼������½��ļ��¼����ļ���׷�������¼��������þ���·������ѡ
* `uncompress_algorithm` : �������ǰ��ѹ��Ŀǰ�㷨ֻ��"deflate"����ѡ

ʾ��

```
{ "plugin":"so/logpipe-output-file.so" , "path":"/home/iblog/log" , "uncompress_algorithm":"deflate" }
```

### 4.2.3. logpipe-input-tcp

������

* `ip` : ���������IP����ѡ
* `port` : ���������PORT����ѡ

ʾ��

```
{ "plugin":"so/logpipe-input-tcp.so" , "ip":"158.1.0.55" , "port":5151 }
```

### 4.2.4. logpipe-output-tcp

������

* `ip` : ���ӷ����IP����ѡ
* `port` : ���ӷ����PORT����ѡ

ʾ��

```
{ "plugin":"so/logpipe-output-tcp.so" , "ip":"158.1.0.55" , "port":5151 }
```

# 5. �������

`logpipe`�Ĳ�������֧�ֶ�����˺Ͷ�����ˣ��Դ����ʵ�������������Ҳ��Ϊ�������ʾ����ʹ���߿ɸ����������󿪷��Լ��Ĳ�������õ�`logpipe`�С�

�����������ͷ�ļ���`$HOME/include/logpipe/*.h`���������������ļ���`$HOME/lib/liblogpipe_api.so`�����һ�㶼�����`$HOME/so/logpipe-(input|output)-(�Զ�������).so`��

���Դ�����ļ�һ��Ϊһ��.c�ļ���һ�����һ�����һ������ʵ��������+һ��ص�������

## 5.1. ������

��������������ģ�����£�

```
#include "logpipe_api.h"

char	*__LOGPIPE_INPUT_FILE_VERSION = "0.1.0" ;

/* ��������ṹ */
struct InputPluginContext
{
	...
} ;

funcLoadInputPluginConfig LoadInputPluginConfig ;
int LoadInputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , struct LogpipePluginConfigItem *p_plugin_config_items , void **pp_context , int *p_fd )
{
	struct InputPluginContext	*p_plugin_ctx = NULL ;
	char				*p = NULL ;
	
	/* �����ڴ��Դ�Ų�������� */
	p_plugin_ctx = (struct InputPluginContext *)malloc( sizeof(struct InputPluginContext) ) ;
	if( p_plugin_ctx == NULL )
	{
		ERRORLOG( "malloc failed , errno[%d]" , errno );
		return -1;
	}
	memset( p_plugin_ctx , 0x00 , sizeof(struct InputPluginContext) );
	
	/* ����������� */
	p_plugin_ctx->... = QueryPluginConfigItem( p_plugin_config_items , "..." ) ;
	INFOLOG( "...[%s]" , p_plugin_ctx->... )
	
	...

	/* ���ò������������ */
	(*pp_context) = p_plugin_ctx ;
	
	return 0;
}

funcInitInputPluginContext InitInputPluginContext ;
int InitInputPluginContext( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , void *p_context , int *p_fd )
{
	struct InputPluginContext	*p_plugin_ctx = (struct InputPluginContext *)(p_context) ;
	
	...
	
	/* �������������� */
	(*p_fd) = ... ;
	
	return 0;
}

funcOnInputPluginEvent OnInputPluginEvent ;
int OnInputPluginEvent( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , void *p_context )
{
	struct InputPluginContext	*p_plugin_ctx = (struct InputPluginContext *)p_context ;
	
	...
	
	return 0;
}

funcReadInputPlugin ReadInputPlugin ;
int ReadInputPlugin( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , void *p_context , uint32_t *p_block_len , char *block_buf , int block_bufsize )
{
	struct InputPluginContext	*p_plugin_ctx = (struct InputPluginContext *)p_context ;
	
	...
	
	return 0;
}

funcCleanInputPluginContext CleanInputPluginContext ;
int CleanInputPluginContext( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , void *p_context )
{
	struct InputPluginContext	*p_plugin_ctx = (struct InputPluginContext *)p_context ;
	
	...
	
	free( p_plugin_ctx->inotify_read_buffer );
	
	return 0;
}

funcUnloadInputPluginConfig UnloadInputPluginConfig ;
int UnloadInputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeInputPlugin *p_logpipe_input_plugin , void **pp_context )
{
	struct InputPluginContext	**pp_plugin_ctx = (struct InputPluginContext **)pp_context ;
	
	/* �ͷ��ڴ��Դ�Ų�������� */
	free( (*pp_plugin_ctx) ); (*pp_plugin_ctx) = NULL ;
	
	return 0;
}
```

˵����

* �ص�����`LoadInputPluginConfig`����װ������ʱ�����������ĺͽ������ò�����������ģ�����Ĳ�������ı������ص�����ʹ�ã�����ڻص�����`UnloadInputPluginConfig`���ͷš������������Ӧ���Զ��塣
* �ص�����`InitInputPluginContexth`��`CleanInputPluginContext`�����ʼ���������ڲ�������
* �ص�����`OnInputPluginEvent`���¼������Ϸ������ڸò�����¼�ʱ�����á����������`WriteAllOutputPlugins`������ܴ�����Ϣ�������߼����£�

```
��������������
	������������BeforeWriteOutputPlugin
ѭ��
	������������ReadInputPlugin
	��������������
		������������WriteOutputPlugin
��������������
	������������AfterWriteOutputPlugin
```

* �ص�����`ReadInputPlugin`�ڴ�����ܴ�����Ϣʱ���������ã�ֱ������`LOGPIPE_READ_END_OF_INPUT`��

ע�⣺��ʼ���������������¼������֣���ܻ���뵽�¼������С�
ע�⣺�������лص�����������`ReadInputPlugin`����ֵ`LOGPIPE_READ_END_OF_INPUT`�⣩��������ֵ����0ʱ�жϱ�����Ϣ���ݣ�������ֵС��0ʱ����logpipe�������̡�

## 5.2. ������

��������������ģ�����£�

```
#include "logpipe_api.h"

char	*__LOGPIPE_OUTPUT_FILE_VERSION = "0.1.0" ;

struct OutputPluginContext
{
	...
} ;

funcLoadOutputPluginConfig LoadOutputPluginConfig ;
int LoadOutputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , struct LogpipePluginConfigItem *p_plugin_config_items , void **pp_context )
{
	struct OutputPluginContext	*p_plugin_ctx = NULL ;
	
	p_plugin_ctx = (struct OutputPluginContext *)malloc( sizeof(struct OutputPluginContext) ) ;
	if( p_plugin_ctx == NULL )
	{
		ERRORLOG( "malloc failed , errno[%d]" , errno );
		return -1;
	}
	memset( p_plugin_ctx , 0x00 , sizeof(struct OutputPluginContext) );
	
	p_plugin_ctx->... = QueryPluginConfigItem( p_plugin_config_items , "..." ) ;
	INFOLOG( "...[%s]" , p_plugin_ctx->... )
	
	...
	
	/* ���ò������������ */
	(*pp_context) = p_plugin_ctx ;
	
	return 0;
}

funcInitOutputPluginContext InitOutputPluginContext ;
int InitOutputPluginContext( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	return 0;
}

funcBeforeWriteOutputPlugin BeforeWriteOutputPlugin ;
int BeforeWriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context , uint16_t filename_len , char *filename )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	...
	
	return 0;
}

funcWriteOutputPlugin WriteOutputPlugin ;
int WriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context , uint32_t block_len , char *block_buf )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	...
	
	return 0;
}

funcAfterWriteOutputPlugin AfterWriteOutputPlugin ;
int AfterWriteOutputPlugin( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	struct OutputPluginContext	*p_plugin_ctx = (struct OutputPluginContext *)p_context ;
	
	...
	
	return 0;
}

funcCleanOutputPluginContext CleanOutputPluginContext ;
int CleanOutputPluginContext( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void *p_context )
{
	return 0;
}

funcUnloadOutputPluginConfig UnloadOutputPluginConfig ;
int UnloadOutputPluginConfig( struct LogpipeEnv *p_env , struct LogpipeOutputPlugin *p_logpipe_output_plugin , void **pp_context )
{
	struct OutputPluginContext	**pp_plugin_ctx = (struct OutputPluginContext **)pp_context ;
	
	/* �ͷ��ڴ��Դ�Ų�������� */
	free( (*pp_plugin_ctx) ); (*pp_plugin_ctx) = NULL ;
	
	return 0;
}
```

˵����

* �ص�����`LoadOutputPluginConfig`����װ������ʱ�����������ĺͽ������ò�����������ģ�����Ĳ�������ı������ص�����ʹ�ã�����ڻص�����`UnloadoutputPluginConfig`���ͷš������������Ӧ���Զ��塣
* �ص�����`InitOutputPluginContexth`��`CleanOutputPluginContext`�����ʼ���������ڲ�������
* �ص�����`WriteOutputPlugin`�ڴ�����ܴ�����Ϣʱ���������á�

ע�⣺�������лص�������������ֵ����0ʱ�жϱ�����Ϣ���ݣ�������ֵС��0ʱ����logpipe�������̡�

# 6. ����ѹ��

ѹ�⻷����

WINDOWS 10����װ��VMware Workstation 12��VMware����װ��Red Hat Enterprise Linux Server release 7.3 (Maipo)��

Ӳ��������

```
$ cat /proc/cpuinfo | grep "model name"
model name      : Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz
model name      : Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz
model name      : Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz
model name      : Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz
$ free -m
              total        used        free      shared  buff/cache   available
Mem:            984          90         186           1         708         723
Swap:          2047          52        1995
$ df -m
�ļ�ϵͳ                     1M-��  ����  ���� ����% ���ص�
/dev/mapper/rhel_rhel73-root 17394  7572  9823   44% /
devtmpfs                       482     0   482    0% /dev
tmpfs                          493     0   493    0% /dev/shm
tmpfs                          493     7   486    2% /run
tmpfs                          493     0   493    0% /sys/fs/cgroup
/dev/sda1                     1014   139   876   14% /boot
tmpfs                           99     0    99    0% /run/user/0
tmpfs                           99     0    99    0% /run/user/1000
```

��־�ռ��ܹ���

* `flume-ng`����ܹ�Ϊ��һ��Ŀ¼�вɼ���־�ļ�Լ100MB������channelΪmemory����ص���һ��Ŀ¼�С�
* `logpipe`����ܹ�Ϊ�Ӹ�һ��Ŀ¼�вɼ���־�ļ�Լ100MB������TCP���䣨����ѹ��������ص���һ��Ŀ¼�С�

## 6.1. flume-ng

Ԥ�ȿ�����־�ļ�

```
$ cd $HOME/log
$ cp ../a.log.2 a.log
```

�鿴flume-ng����

```
$ cat conf/test.conf
a1.sources = s1
a1.channels = c1
a1.sinks = k1

a1.sources.s1.type = spooldir
a1.sources.s1.channels = c1
a1.sources.s1.spoolDir = /home/calvin/log
# a1.sources.s1.fileHeader = true

a1.channels.c1.type = memory
a1.channels.c1.capacity = 1000 
a1.channels.c1.transactionCapacity = 100

a1.sinks.k1.type = file_roll
a1.sinks.k1.channel = c1
a1.sinks.k1.sink.directory = /home/calvin/log3
```

����flume-ng�ɼ�

```
$ ./bin/flume-ng agent -n a1 -c conf -f conf/test.conf -Dflume.root.logger=INFO,console
Info: Including Hive libraries found via () for Hive access
+ exec /home/calvin/expack/jdk1.8.0_152/bin/java -Xmx20m -Dflume.root.logger=INFO,console -cp '/home/calvin/expack/apache-flume-1.8.0-bin/conf:/home/calvin/expack/apache-flume-1.8.0-bin/lib/*:/lib/*' -Djava.library.path= org.apache.flume.node.Application -n a1 -f conf/test.conf
2017-12-17 23:33:15,347 (lifecycleSupervisor-1-0) [INFO - org.apache.flume.node.PollingPropertiesFileConfigurationProvider.start(PollingPropertiesFileConfigurationProvider.java:62)] Configuration provider starting
2017-12-17 23:33:15,351 (conf-file-poller-0) [INFO - org.apache.flume.node.PollingPropertiesFileConfigurationProvider$FileWatcherRunnable.run(PollingPropertiesFileConfigurationProvider.java:134)] Reloading configuration file:conf/test.conf
2017-12-17 23:33:15,357 (conf-file-poller-0) [INFO - org.apache.flume.conf.FlumeConfiguration$AgentConfiguration.addProperty(FlumeConfiguration.java:1016)] Processing:k1
2017-12-17 23:33:15,357 (conf-file-poller-0) [INFO - org.apache.flume.conf.FlumeConfiguration$AgentConfiguration.addProperty(FlumeConfiguration.java:930)] Added sinks: k1 Agent: a1
2017-12-17 23:33:15,357 (conf-file-poller-0) [INFO - org.apache.flume.conf.FlumeConfiguration$AgentConfiguration.addProperty(FlumeConfiguration.java:1016)] Processing:k1
2017-12-17 23:33:15,357 (conf-file-poller-0) [INFO - org.apache.flume.conf.FlumeConfiguration$AgentConfiguration.addProperty(FlumeConfiguration.java:1016)] Processing:k1
2017-12-17 23:33:15,364 (conf-file-poller-0) [INFO - org.apache.flume.conf.FlumeConfiguration.validateConfiguration(FlumeConfiguration.java:140)] Post-validation flume configuration contains configuration for agents: [a1]
2017-12-17 23:33:15,364 (conf-file-poller-0) [INFO - org.apache.flume.node.AbstractConfigurationProvider.loadChannels(AbstractConfigurationProvider.java:147)] Creating channels
2017-12-17 23:33:15,369 (conf-file-poller-0) [INFO - org.apache.flume.channel.DefaultChannelFactory.create(DefaultChannelFactory.java:42)] Creating instance of channel c1 type memory
2017-12-17 23:33:15,371 (conf-file-poller-0) [INFO - org.apache.flume.node.AbstractConfigurationProvider.loadChannels(AbstractConfigurationProvider.java:201)] Created channel c1
2017-12-17 23:33:15,372 (conf-file-poller-0) [INFO - org.apache.flume.source.DefaultSourceFactory.create(DefaultSourceFactory.java:41)] Creating instance of source s1, type spooldir
2017-12-17 23:33:15,378 (conf-file-poller-0) [INFO - org.apache.flume.sink.DefaultSinkFactory.create(DefaultSinkFactory.java:42)] Creating instance of sink: k1, type: file_roll
2017-12-17 23:33:15,384 (conf-file-poller-0) [INFO - org.apache.flume.node.AbstractConfigurationProvider.getConfiguration(AbstractConfigurationProvider.java:116)] Channel c1 connected to [s1, k1]
2017-12-17 23:33:15,389 (conf-file-poller-0) [INFO - org.apache.flume.node.Application.startAllComponents(Application.java:137)] Starting new configuration:{ sourceRunners:{s1=EventDrivenSourceRunner: { source:Spool Directory source s1: { spoolDir: /home/calvin/log } }} sinkRunners:{k1=SinkRunner: { policy:org.apache.flume.sink.DefaultSinkProcessor@4224eda0 counterGroup:{ name:null counters:{} } }} channels:{c1=org.apache.flume.channel.MemoryChannel{name: c1}} }
2017-12-17 23:33:15,394 (conf-file-poller-0) [INFO - org.apache.flume.node.Application.startAllComponents(Application.java:144)] Starting Channel c1
2017-12-17 23:33:15,395 (conf-file-poller-0) [INFO - org.apache.flume.node.Application.startAllComponents(Application.java:159)] Waiting for channel: c1 to start. Sleeping for 500 ms
2017-12-17 23:33:15,432 (lifecycleSupervisor-1-2) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.register(MonitoredCounterGroup.java:119)] Monitored counter group for type: CHANNEL, name: c1: Successfully registered new MBean.
2017-12-17 23:33:15,432 (lifecycleSupervisor-1-2) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.start(MonitoredCounterGroup.java:95)] Component type: CHANNEL, name: c1 started
2017-12-17 23:33:15,900 (conf-file-poller-0) [INFO - org.apache.flume.node.Application.startAllComponents(Application.java:171)] Starting Sink k1
2017-12-17 23:33:15,901 (lifecycleSupervisor-1-0) [INFO - org.apache.flume.sink.RollingFileSink.start(RollingFileSink.java:110)] Starting org.apache.flume.sink.RollingFileSink{name:k1, channel:c1}...
2017-12-17 23:33:15,901 (conf-file-poller-0) [INFO - org.apache.flume.node.Application.startAllComponents(Application.java:182)] Starting Source s1
2017-12-17 23:33:15,903 (lifecycleSupervisor-1-4) [INFO - org.apache.flume.source.SpoolDirectorySource.start(SpoolDirectorySource.java:83)] SpoolDirectorySource source starting with directory: /home/calvin/log
2017-12-17 23:33:15,904 (lifecycleSupervisor-1-0) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.register(MonitoredCounterGroup.java:119)] Monitored counter group for type: SINK, name: k1: Successfully registered new MBean.
2017-12-17 23:33:15,904 (lifecycleSupervisor-1-0) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.start(MonitoredCounterGroup.java:95)] Component type: SINK, name: k1 started
2017-12-17 23:33:15,908 (lifecycleSupervisor-1-0) [INFO - org.apache.flume.sink.RollingFileSink.start(RollingFileSink.java:142)] RollingFileSink k1 started.
2017-12-17 23:33:15,962 (lifecycleSupervisor-1-4) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.register(MonitoredCounterGroup.java:119)] Monitored counter group for type: SOURCE, name: s1: Successfully registered new MBean.
2017-12-17 23:33:15,962 (lifecycleSupervisor-1-4) [INFO - org.apache.flume.instrumentation.MonitoredCounterGroup.start(MonitoredCounterGroup.java:95)] Component type: SOURCE, name: s1 started
2017-12-17 23:33:23,822 (pool-3-thread-1) [INFO - org.apache.flume.client.avro.ReliableSpoolingFileEventReader.readEvents(ReliableSpoolingFileEventReader.java:324)] Last read took us just up to a file boundary. Rolling to the next file, if there is one.
2017-12-17 23:33:23,823 (pool-3-thread-1) [INFO - org.apache.flume.client.avro.ReliableSpoolingFileEventReader.rollCurrentFile(ReliableSpoolingFileEventReader.java:433)] Preparing to move file /home/calvin/log/a.log to /home/calvin/log/a.log.COMPLETED
```

�鿴�ɼ�Ŀ¼�͹鼯Ŀ¼

```
$ cd $HOME/log
$ ls -l 
������ 109896
-rw-rw-r-- 1 calvin calvin 112530011 12�� 17 23:33 a.log.COMPLETED
$ cd $HOME/log3
$ ls -l
������ 131008
-rw-rw-r-- 1 calvin calvin 112530011 12�� 17 23:33 1513524795383-1
```

�鿴�ڴ�ռ��

```
$ ps aux | grep java
calvin   39254  2.4  8.0 3136484 81028 pts/8   Sl+  23:33   0:15 /home/calvin/expack/jdk1.8.0_152/bin/java -Xmx20m -Dflume.root.logger=INFO,console -cp /home/calvin/expack/apache-flume-1.8.0-bin/conf:/home/calvin/expack/apache-flume-1.8.0-bin/lib/*:/lib/* -Djava.library.path= org.apache.flume.node.Application -n a1 -f conf/test.conf
```

�ɼ�ʱ����flume-ng�����ȡ��"23:33:15,962"��"23:33:23,822"��Լ8�롣

## 6.2. logpipe

�鿴�ɼ��˺͹鼯�������ļ�

```
$ cat $HOME/etc/logpipe_case1_collector.conf
{
        "log" :
        {
                "log_file" : "/tmp/logpipe_case1_collector.log" ,
                "log_level" : "INFO"
        } ,

        "inputs" :
        [
                { "plugin":"so/logpipe-input-file.so" , "path":"/home/calvin/log" }
        ] ,

        "outputs" :
        [
                { "plugin":"so/logpipe-output-tcp.so" , "ip":"127.0.0.1" , "port":10101 }
        ]
}
```

```
$ cat $HOME/etc/logpipe_case1_dump.conf
{
        "log" :
        {
                "log_file" : "/tmp/logpipe_case1_dump.log" ,
                "log_level" : "INFO"
        } ,

        "inputs" :
        [
                { "plugin":"so/logpipe-input-tcp.so" , "ip":"127.0.0.1" , "port":10101 }
        ] ,

        "outputs" :
        [
                { "plugin":"so/logpipe-output-file.so" , "path":"/home/calvin/log3" }
        ]
}
```

�����鼯�˺Ͳɼ���

```
$ logpipe -f logpipe_case1_dump.conf 
logpipe -f logpipe_case1_dump.conf 
2017-12-17 23:45:33.378112 | INFO  | 39309:logpipe-output-file.c:30 | path[/home/calvin/log3]
2017-12-17 23:45:33.378161 | INFO  | 39309:logpipe-output-file.c:45 | uncompress_algorithm[(null)]
2017-12-17 23:45:33.378163 | DEBUG | 39309:config.c:307 | [so/logpipe-output-file.so]->pfuncLoadOutputPluginConfig ok
2017-12-17 23:45:33.378167 | INFO  | 39309:logpipe-input-tcp.c:46 | ip[127.0.0.1]
2017-12-17 23:45:33.378169 | INFO  | 39309:logpipe-input-tcp.c:53 | port[10101]
2017-12-17 23:45:33.378172 | DEBUG | 39309:config.c:324 | [so/logpipe-input-tcp.so]->pfuncLoadInputPluginConfig ok
$ logpipe -f logpipe_case1_collector.conf
2017-12-17 23:45:34.449968 | INFO  | 39313:logpipe-output-tcp.c:74 | ip[127.0.0.1]
2017-12-17 23:45:34.450018 | INFO  | 39313:logpipe-output-tcp.c:81 | port[10101]
2017-12-17 23:45:34.450022 | DEBUG | 39313:config.c:307 | [so/logpipe-output-tcp.so]->pfuncLoadOutputPluginConfig ok
2017-12-17 23:45:34.450038 | INFO  | 39313:logpipe-input-file.c:367 | path[/home/calvin/log]
2017-12-17 23:45:34.450040 | INFO  | 39313:logpipe-input-file.c:370 | file[(null)]
2017-12-17 23:45:34.450041 | INFO  | 39313:logpipe-input-file.c:388 | exec_before_rotating[(null)]
2017-12-17 23:45:34.450043 | INFO  | 39313:logpipe-input-file.c:395 | rotate_size[0]
2017-12-17 23:45:34.450044 | INFO  | 39313:logpipe-input-file.c:413 | exec_after_rotating[(null)]
2017-12-17 23:45:34.450046 | INFO  | 39313:logpipe-input-file.c:428 | compress_algorithm[(null)]
2017-12-17 23:45:34.450047 | DEBUG | 39313:config.c:324 | [so/logpipe-input-file.so]->pfuncLoadInputPluginConfig ok
$ ps -ef | grep "logpipe -f" | grep -v grep
calvin   39311     1  0 23:45 ?        00:00:00 logpipe -f logpipe_case1_dump.conf
calvin   39312 39311  0 23:45 ?        00:00:00 logpipe -f logpipe_case1_dump.conf
calvin   39315     1  0 23:45 ?        00:00:00 logpipe -f logpipe_case1_collector.conf
calvin   39316 39315  0 23:45 ?        00:00:00 logpipe -f logpipe_case1_collector.conf
```

������־�ļ�

```
$ cd $HOME/log
$ cp ../a.log.2 a.log
$ ls -l
������ 131008
-rw-rw-r-- 1 calvin calvin 112530011 12�� 17 23:45 a.log
```

�۲�ɼ��˺͹鼯�˴�����־

```
$ head /tmp/logpipe.log
2017-12-17 23:45:43.714113 | INFO  | 39316:worker.c:84 | epoll_wait[1] return[1]events
2017-12-17 23:45:43.715549 | INFO  | 39316:logpipe-input-file.c:539 | read inotify[3] ok , [32]bytes
2017-12-17 23:45:43.715603 | INFO  | 39316:logpipe-input-file.c:289 | inotify_add_watch[/home/calvin/log/a.log] ok , inotify_fd[3] inotify_wd[2] trace_offset[0]
2017-12-17 23:45:43.715682 | INFO  | 39316:logpipe-output-tcp.c:158 | send comm magic and filename ok , [8]bytes
2017-12-17 23:45:43.715915 | INFO  | 39316:logpipe-input-file.c:669 | read file ok , [102400]bytes
2017-12-17 23:45:43.715961 | INFO  | 39316:logpipe-output-tcp.c:181 | send block len to socket ok , [4]bytes
2017-12-17 23:45:43.716071 | INFO  | 39316:logpipe-output-tcp.c:193 | send block data to socket ok , [102400]bytes
2017-12-17 23:45:43.716284 | INFO  | 39316:logpipe-input-file.c:669 | read file ok , [102400]bytes
2017-12-17 23:45:43.716319 | INFO  | 39316:logpipe-output-tcp.c:181 | send block len to socket ok , [4]bytes
2017-12-17 23:45:43.716337 | INFO  | 39316:logpipe-output-tcp.c:193 | send block data to socket ok , [102400]bytes
2017-12-17 23:45:43.716494 | INFO  | 39316:logpipe-input-file.c:669 | read file ok , [102400]bytes
2017-12-17 23:45:43.716521 | INFO  | 39316:logpipe-output-tcp.c:181 | send block len to socket ok , [4]bytes
2017-12-17 23:45:43.716557 | INFO  | 39316:logpipe-output-tcp.c:193 | send block data to socket ok , [102400]bytes
```

```
$ tail /tmp/logpipe.log
2017-12-17 23:45:44.471734 | INFO  | 39312:logpipe-input-tcp.c:305 | recv block length from accepted session sock ok , [4]bytes
2017-12-17 23:45:44.471747 | INFO  | 39312:logpipe-input-tcp.c:334 | recv block from accepted session sock ok , [102400]bytes
2017-12-17 23:45:44.471812 | INFO  | 39312:logpipe-output-file.c:103 | write block data to file ok , [102400]bytes
2017-12-17 23:45:44.471817 | INFO  | 39312:logpipe-input-tcp.c:305 | recv block length from accepted session sock ok , [4]bytes
2017-12-17 23:45:44.471823 | INFO  | 39312:logpipe-input-tcp.c:334 | recv block from accepted session sock ok , [33371]bytes
2017-12-17 23:45:44.471849 | INFO  | 39312:logpipe-output-file.c:103 | write block data to file ok , [33371]bytes
2017-12-17 23:45:44.471856 | INFO  | 39312:logpipe-input-tcp.c:305 | recv block length from accepted session sock ok , [4]bytes
2017-12-17 23:45:44.471858 | INFO  | 39312:output.c:45 | [accepted_session]->pfuncReadInputPlugin done
2017-12-17 23:45:44.471860 | INFO  | 39312:logpipe-output-file.c:180 | close file
2017-12-17 23:45:44.471863 | INFO  | 39312:worker.c:67 | epoll_wait[1] ...
```

�鿴�鼯��Ŀ¼

```
$ cd $HOME/log3
$ ls -l
������ 129600
-rwxrwxrwx 1 calvin calvin 112530011 12�� 17 23:45 a.log
```

```
$ ps aux | grep "logpipe -f"
calvin   39311  0.0  0.0  14744   400 ?        S    23:45   0:00 logpipe -f logpipe_case1_dump.conf
calvin   39312  0.2  0.0  14952   644 ?        S    23:45   0:00 logpipe -f logpipe_case1_dump.conf
calvin   39315  0.0  0.0  14756   424 ?        S    23:45   0:00 logpipe -f logpipe_case1_collector.conf
calvin   39316  0.3  1.6  31348 17080 ?        S    23:45   0:00 logpipe -f logpipe_case1_collector.conf
```

�ɼ�ʱ���ɲɼ��˺͹鼯�˴�����־��ȡ��"23:10:51.950398"��"23:10:52.576147"��Լ0.6�롣

## 6.3. �ܽ�

|  | flume-ng | logpipe |
|---|---|---|
| ��������־�ļ���С | 100MB | 100MB |
| �ڴ�ռ��(RSS) | 81MB | 17MB |
| ��ע |  | ����TCP���䣻δ����ѹ�� |
| ��ʱ | 8�� | 0.6�� |

���ۣ� logpipe������ȫ��ѹflume-ng��

# 7. ���

��ӭʹ��logpipe�������ʹ��������������������ң�лл ^_^

Դ���йܵ�ַ : [��Դ�й�](https://gitee.com/calvinwilliams/logpipe)��[github](https://github.com/calvinwilliams/logpipe)

�������� : [����](mailto:calvinwilliams@163.com)��[Gmail](mailto:calvinwilliams.c@gmail.com)