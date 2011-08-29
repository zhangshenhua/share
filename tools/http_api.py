# -*- coding: utf-8 -*-
import os,sys
reload(sys)
sys.setdefaultencoding('utf-8')
import urllib2


def getResponse(requestStr,coding):
    opener = urllib2.build_opener()
    f = opener.open(requestStr)
    s =  f.read()
    f.close()
    return s.decode(coding)

if __name__== '__main__':
    #print getResponse('http://openapi.3g.youku.com/videos/hot?pid=1&format=4&pz=4','unicode-escape')
    #sys.stdout.write( getResponse('http://openapi.3g.youku.com/videos/hot?pid=1&format=4&pz=4','unicode-escape'))
    coding = 'unicode-escape'
    requestStr = ''

    for arg in sys.argv[1:]:
        if arg[:2]=='-c':
            coding = arg[2:]
        elif arg[0]!='-':
            requestStr = arg
        

    sys.stdout.write(getResponse(requestStr,coding));
            
            
            
    
