import wx
import sys

# UI's ID
ID_RUN = 101
ID_EXIT = 110

def draw(dc, balls, rays, mirrors, bounding_box):
    dc.Clear()

    minx, miny, maxx, maxy = [
        bounding_box['x1'],
        bounding_box['y2'],
        bounding_box['x2'],
        bounding_box['y1']
        ]

    border = 10
    pw = maxx - minx
    ph = maxy - miny

    c_scale = 1

    w,h = dc.GetSize()
    w -= 2*border
    h -= 2*border

    p_scale = min(float(w)/pw,float(h)/ph)
    def translate(x,y):
        return (border + int((x - minx)*p_scale),
                border + int((maxy - y)*p_scale))

    for b in balls:

        if b.t == 0:
            dc.SetPen(wx.Pen("BLACK", 1))
            dc.SetBrush(wx.Brush("WHITE"))
        elif b.t == 1:
            dc.SetPen(wx.Pen("BLACK", 1))
            dc.SetBrush(wx.Brush("GREEN"))
        else:
            dc.SetPen(wx.Pen("BLACK", 1))
            dc.SetBrush(wx.Brush("#007000"))

        x,y = translate(b.x,b.y)
        r = b.r*p_scale
        if r<1:
            r = 1
        dc.DrawCircle(x,y,r)

    for r in rays:
        ox, oy = translate(r[0][0],r[0][1])
        for x,y in r[1:]:
            xx,yy = translate(x,y)
            dc.DrawLine(ox,oy,xx,yy)
            ox,oy = xx,yy

    for m in mirrors:
        dc.SetPen(wx.Pen("BLUE", 2))
        x1,y1 = translate(m[0],m[1])
        x2,y2 = translate(m[2],m[3])
        dc.DrawLine(x1,y1,x2,y2)

class Ball:
    pass

class MainWindow(wx.Frame):
    """ We simply derive a new class of Frame. """
    def __init__(self, parent, id, title, 
                 balls, rays, mirrors,
                 bounding_box):

        self.balls = balls
        self.rays = rays
        self.mirrors = mirrors
        self.bounding_box = bounding_box
        
        wx.Frame.__init__(self, parent, id, title, size=(500,400))
        self.panel = wx.Panel(self)
        self.panel.Bind(wx.EVT_PAINT, self.OnPaint)

        # Setting up the menu.
        filemenu= wx.Menu()
        filemenu.Append(ID_EXIT,"E&xit"," Terminate the program")
        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File") 
        self.SetMenuBar(menuBar)  

        wx.EVT_MENU(self, ID_EXIT, self.OnExitMenu)

        self.Show(True)

    def OnExitMenu(self,event):
        self.Close(True)

    def OnPaint(self,event):
        dc = wx.PaintDC(event.GetEventObject())
        draw(dc, self.balls, self.rays, 
             self.mirrors, self.bounding_box)

def read_config(config_filename):
    fp = open(config_filename)
    bounding_box = [int(i) for i in fp.readline().strip().split()]
    bcount = int(fp.readline().strip())
    balls = []
    for i in range(bcount):
        items = fp.readline().strip().split()
        if len(items)!=4:
            print >> sys.stderr, "Config format error"
        b = Ball()
        b.x = float(items[0])
        b.y = float(items[1])
        b.r = float(items[2])
        b.t = int(items[3])
        balls.append(b)
    return {'balls': balls,
            'bounding_box':
                {'x1': bounding_box[0],
                 'y1': bounding_box[1],
                 'x2': bounding_box[2],
                 'y2': bounding_box[3]}}
        
def read_rays(ray_filename):
    fp = open(ray_filename)
    rcount = int(fp.readline().strip())
    rays = []
    for i in range(rcount):
        r = []
        pcount = int(fp.readline().strip())
        for j in range(pcount):
            x,y = [float(f) for f in fp.readline().strip().split(',')]
            r.append((x,y))
        rays.append(r)

    fp.readline()
    try:
        mcount = int(fp.readline().strip())
    except:
        mcount = 0
    mirrors = []
    for i in range(mcount):
        m = [float(x) for x in fp.readline().strip().split()]
        mirrors.append(m)

    return { 'rays': rays,
             'mirrors': mirrors }

def read_input(config_filename, ray_filename):
    c = read_config(config_filename)
    r = read_rays(ray_filename)
    return {'balls': c['balls'],
            'bounding_box': c['bounding_box'],
            'rays': r['rays'],
            'mirrors': r['mirrors']}

def main():
    if len(sys.argv) < 3:
        print 'usage: viewer.py [config_file] [ray_file]'
        sys.exit(0)

    i = read_input(sys.argv[1], sys.argv[2])
    app = wx.PySimpleApp()
    frame=MainWindow(None, wx.ID_ANY, 'Game viewer', 
                     i['balls'], i['rays'], 
                     i['mirrors'], i['bounding_box'])
    app.MainLoop()

if __name__ == "__main__":
    main()

