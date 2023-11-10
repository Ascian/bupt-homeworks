from PIL import ImageColor
import math

class ImageDraw:
    def __init__(self, image):
        self._image = image

    # draw line
    # start: the start point of the line, tuple as (x, y)
    # end: the end point of the line, tuple as (x, y)
    # width: line width, integer
    # color: line color, string
    # type: line type, string, default as 'solid', choices=['solid', 'dotted', 'dashed', 'dashed-dotted']
    def line(self, start, end, width=1, color='black', type='solid'):
        # if the coodinates of start and end are not integer or the width is not integer
        if not isinstance(start[0], int) or not isinstance(start[1], int) or not isinstance(end[0], int) or not isinstance(end[1], int) or not isinstance(width, int):
            raise RuntimeError('Invalid coordinates or width, the coodinates and width should be integer')
        
        # using state transferring to draw line type
        states = []
        if type == 'solid':
            states = [True]
        elif type == 'dotted':
            states = [True] + [False] * width * 2
        elif type == 'dashed':
            states = [True] * width * 2 + [False] * width * 2
        elif type == 'dashed-dotted':
            states = [True] + [False] * width * 2 + [True] * width * 2 + [False] * width * 2
        else:
            raise RuntimeError('Invalid line type')
        
        # if the absolute value of slope is bigger than 1
        is_exchange = False
        if abs(end[1] - start[1]) > abs(end[0] - start[0]):
            is_exchange = True
            end = end[1], end[0]
            start = start[1], start[0]

        # if delta_x is less than zero
        if end[0] - start[0] < 0:
            start, end = end, start

        # if delta_y is less than zero
        is_decrease = False
        if end[1] - start[1] < 0:
            is_decrease = True
            
        xs, ys = start
        xe, ye = end
        delta_x = xe - xs
        delta_y = abs(ye - ys)
        e = - delta_x
        x, y = xs, ys 

        state = 0
        for i in range(0, delta_x + 1):
            if states[state]:
                # if the absolute value of slope is bigger than 1, the x and y are exchanged
                if is_exchange:
                    for i in range(0, width):
                        for j in range(0, width):
                            self._image.putpixel((y + i, x + j), ImageColor.getrgb(color))
                else:
                    for i in range(0, width):
                        for j in range(0, width):
                            self._image.putpixel((x + i, y + j), ImageColor.getrgb(color))

            if e >= 0:
                # if delta_y is less than zero, y should decrease
                if is_decrease:
                    y -= 1
                else:
                    y += 1
                e -= 2 * delta_x            
            x += 1
            e += 2 * delta_y

            # state transferring
            state = (state + 1) % len(states)


    # draw circle
    # xy: the center of the circle, tuple as (x, y)
    # r: radius, integer
    # width: line width, integer
    # color: line color, string
    def circle(self, xy, r, width=1, color='black'):
        # if the coodinates of xy and radius are not integer or the width is not integer
        if not isinstance(xy[0], int) or not isinstance(xy[1], int) or not isinstance(r, int) or not isinstance(width, int):
            raise RuntimeError('Invalid coordinates or radius or width, the coodinates, radius and width should be integer')
        
        d = 3 - 2 * r
        x, y = 0, r
        while x <= y:
            for i in range(width):
                for j in range(width):
                    # using 8-way symmetry
                    self._image.putpixel((xy[0] + x + i, xy[1] + y + j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] + y + i, xy[1] + x + j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] - x - i, xy[1] + y + j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] - y - i, xy[1] + x + j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] + x + i, xy[1] - y - j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] + y + i, xy[1] - x - j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] - x - i, xy[1] - y - j), ImageColor.getrgb(color))
                    self._image.putpixel((xy[0] - y - i, xy[1] - x - j), ImageColor.getrgb(color))
            if d < 0:
                d = d + 4 * x + 6
            else:
                d = d + 4 * (x - y) + 10
                y -= 1
            x += 1

    # draw ellipse
    # start: the start point of the bounding box of the ellipse, tuple as (x, y)
    # end: the end point of the bounding box of the ellipse, tuple as (x, y)
    # delta_t: granularity of the ellipse, the smaller the delta_t, the smoother the ellipse
    # width: line width, integer
    # color: line color, string
    def ellipse(self, start, end, delta_t=0.1, width=1, color='black'):
        # if the coodinates of start and end are not integer or the width is not integer
        if not isinstance(start[0], int) or not isinstance(start[1], int) or not isinstance(end[0], int) or not isinstance(end[1], int) or not isinstance(width, int):
            raise RuntimeError('Invalid coordinates or width, the coodinates and width should be integer') 
        
        a = abs(end[0] - start[0]) / 2
        b = abs(end[1] - start[1]) / 2
        center_x = (start[0] + end[0]) / 2
        center_y = (start[1] + end[1]) / 2
        
        t = 0
        x = a * math.cos(t)
        y = b * math.sin(t)
        while t + delta_t < math.pi / 2:
            # increase t by delta_t
            t += delta_t

            # using 4-way symmetry
            self.line((int(x + center_x), int(y + center_y)), (int(a * math.cos(t) + center_x), int(b * math.sin(t) + center_y)), width, color)
            self.line((int(x + center_x), int(-y + center_y)), (int(a * math.cos(t) + center_x), int(-b * math.sin(t) + center_y)), width, color)
            self.line((int(-x + center_x), int(y + center_y)), (int(-a * math.cos(t) + center_x), int(b * math.sin(t) + center_y)), width, color)
            self.line((int(-x + center_x), int(-y + center_y)), (int(-a * math.cos(t) + center_x), int(-b * math.sin(t) + center_y)), width, color)
            x = a * math.cos(t)
            y = b * math.sin(t)
        self.line((int(x + center_x), int(y + center_y)), (int(center_x), int(b + center_y)), width, color)
        self.line((int(x + center_x), int(-y + center_y)), (int(center_x), int(-b + center_y)), width, color)
        self.line((int(-x + center_x), int(y + center_y)), (int(center_x), int(b + center_y)), width, color)
        self.line((int(-x + center_x), int(-y + center_y)), (int(center_x), int(-b + center_y)), width, color)

        

    # fill polygon with hatch
    # xys: coordinates of the polygon, list as x1 y1 x2 y2 x3 y3 ...
    # alpha: angle between the hatch and the x axis
    # h: distance between two adjacent hatch lines
    # width: line width, integer
    # color: line color, string
    def fill_with_hatch(self, xys, alpha, h, width=1, color='black'):
        if not isinstance(width, int):
            raise RuntimeError('Invalid width, the width should be integer')

        if len(xys) % 2 != 0:
            raise RuntimeError('Invalid coordinates, the number of coordinates should be even')

        # turn the coordinates into pairs (x, y)
        new_xys = [(xys[i], xys[i+1]) for i in range(0, len(xys), 2)]
        xys = new_xys

        # the slope of the hatch line and the intercept between hatch lines
        k = math.tan(alpha)
        delta_d = abs(h / math.cos(alpha))

        # calculate the intercept of the line with the slope k and the coordinates of the sides
        sides_d = []
        min_d, max_d = float('inf'), float('-inf')
        for i in range(len(xys)):
            start, end = xys[i], xys[(i+1) % len(xys)]
            start_d, end_d = start[1] - k * start[0], end[1] - k * end[0]
            min_d, max_d = min(min_d, start_d), max(max_d, start_d)

            # if the line is not parallel to the hatch line
            if start_d != end_d: 
                next_end_d = xys[(i+2) % len(xys)][1] - k * xys[(i+2) % len(xys)][0]
                # select the lower one as the start point:w
                if start_d > end_d:
                    # if the two line that pass the same point are on the different sides of the hatch line:w
                    if next_end_d < end_d:
                        end_d += 1
                    start, end = end, start
                    start_d, end_d = end_d, start_d
                else:
                    if next_end_d > end_d:
                        end_d -= 1

            sides_d.append((start_d, end_d))
        
        # update d with delta_d, and find the intersection of the line with the slope k and the sides
        d = min_d + delta_d
        while d < max_d:
            points = []
            for i in range(len(sides_d)):
                if int(sides_d[i][0]) <= int(d) <= int(sides_d[i][1]):
                    start, end = xys[i], xys[(i+1) % len(xys)]
                    point_x = (start[0] * end[1] - start[1] * end[0] + d * (end[0] - start[0])) / (end[1] - start[1] - k * (end[0] - start[0]))
                    point_y = k * point_x + d
                    points.append((int(point_x), int(point_y)))
            points.sort(key=lambda x:x[0])
            # draw the hatch line between every two points
            for i in range(0, len(points), 2):
                self.line(points[i], points[i+1], width, color)
            d += delta_d

    # fill polygon with color
    # xys: coordinates of the polygon, list as x1 y1 x2 y2 x3 y3 ...
    # color: line color, string
    def fill_with_color(self, xys, color='black'):
        if len(xys) % 2 != 0:
            raise RuntimeError('Invalid coordinates, the number of coordinates should be even')
        
        new_xys = [[xys[i], xys[i+1]] for i in range(0, len(xys), 2)]
        xys = new_xys

        edge_table = []
        min_y, max_y = float('inf'), float('-inf')
        for i in range(len(xys)):
            min_y, max_y = min(min_y, xys[i][1]), max(max_y, xys[i][1]) 

            start, end = xys[i], xys[(i+1) % len(xys)]
            # if the line is not parallel to the scanning line
            if start[1] != end[1]: 
                # select the lower one as the start point
                if start[1] > end[1]:
                    # if the two line that pass the same point are on the different sides of the scanning line
                    if xys[(i+2) % len(xys)][1] < xys[(i+1) % len(xys)][1]:
                        end[1] += 1
                    start, end = end, start
                else:
                    if xys[(i+2) % len(xys)][1] > xys[(i+1) % len(xys)][1]: 
                        end[1] -= 1
                delta_x = (start[0] - end[0]) / (start[1] - end[1])

            # store the edge(upper_y, lower_x, delta_x) in the edge table with the lower y coordinate as the key
            if len(edge_table) < start[1] + 1:
                edge_table.extend([[] for num in range(start[1] + 1 - len(edge_table))])
            edge_table[start[1]].append((end[1], start[0], delta_x))
        
        active_table = []
        for y in range(min_y, max_y + 1):
            # update the active table with the lines that pass the scanning line
            if len(edge_table) > y and len(edge_table[y]) > 0:
                active_table.extend(edge_table[y])
                active_table.sort(key=lambda x:(x[1], x[2]))

            new_active_table = []
            for i in range(0, len(active_table), 2):
                for j in range(int(active_table[i][1]), int(active_table[i+1][1])):
                    self._image.putpixel((j, y), ImageColor.getrgb(color))
                # if the line is no longer active, remove it from the active table
                # meanwhile, update x with delta_x
                if active_table[i][0] != y:
                    new_active_table.append((active_table[i][0], active_table[i][1] + active_table[i][2], active_table[i][2]))
                if active_table[i+1][0] != y:
                    new_active_table.append((active_table[i+1][0], active_table[i+1][1] + active_table[i+1][2], active_table[i+1][2]))
            active_table = new_active_table