#!/usr/bin/env python2

import cherrypy

class HelloWorld(object):
    def index(self):
        return "Hello, world!"
    index.exposed = True

cherrypy.quickstart(HelloWorld())


